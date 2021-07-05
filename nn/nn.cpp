#include "nn.hpp"
#include "mtl/algorithm.hpp"
#include "mtl/dyn_array.hpp"
#include "mtl/iterator.hpp"
#include "mtl/result.hpp"
#include "mtl/static_array.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <random>
#include <utility>

namespace nn {

using mtl::Err;
using mtl::Ok;

FpType Nn::sigmoid_activation(FpType neuron) {
    /// @todo where do these numbers comefrom
    if (neuron < -45.0) {
        return 0;
    } else if (neuron > 45.0) {
        return 1;
    } else {
        return 1.0 / (1 + std::exp(-neuron));
    }
}

FpType Nn::sigmoid_activation_cached(FpType neuron) {
    assert(!std::isnan(neuron));

    if (neuron < sigmoid_dom_min_) {
        return *(lookup_table_.cbegin());
    }
    if (neuron >= sigmoid_dom_max_) {
        return *(lookup_table_.cend());
    }

    const auto est = static_cast<std::size_t>((neuron - sigmoid_dom_min_) * table_interval_ + 0.5);
    if (est > lookup_table_.size()) {
        return *(lookup_table_.cend());
    }
    return lookup_table_[est];
}
FpType Nn::sigmoid_activation_linear(FpType neuron) { return neuron; }
FpType Nn::sigmoid_activation_threshold(FpType neuron) { return neuron > 0; }

mtl::Maybe<Nn> Nn::make_nn(std::size_t n_inputs, std::size_t n_hidden_layers, std::size_t n_hidden,
                           std::size_t n_outputs, ActivationFuncKind hidden_activation_func,
                           ActivationFuncKind output_activation_func) {
    if (n_inputs < 1)
        return mtl::None{};
    if (n_outputs < 1)
        return mtl::None{};
    if (n_hidden_layers > 0 && n_hidden < 1)
        return mtl::None{};

    return mtl::Maybe<Nn>::some(n_inputs, n_hidden_layers, n_hidden, n_outputs,
                                hidden_activation_func, output_activation_func);
}

Nn::Nn(std::size_t n_inputs, std::size_t n_hidden_layers, std::size_t n_hidden,
       std::size_t n_outputs, ActivationFuncKind hidden_activation_func,
       ActivationFuncKind output_activation_func)
    : n_inputs_(n_inputs), n_hidden_layers_(n_hidden_layers), n_hidden_(n_hidden),
      n_outputs_(n_outputs), hidden_activation_func_(hidden_activation_func),
      output_activation_func_(output_activation_func) {

    std::size_t n_hidden_weights = 0;
    if (n_hidden_layers > 0) {
        n_hidden_weights =
            (n_inputs + 1) * n_hidden + (n_hidden_layers - 1) * (n_hidden + 1) * n_hidden;
    }

    std::size_t n_output_weights = (n_inputs + 1) * n_outputs;
    if (n_hidden_layers > 0) {
        n_output_weights = n_hidden + 1;
    }

    n_total_weights_ = n_hidden_weights + n_output_weights;

    n_total_neurons_ = (n_inputs + n_hidden * n_hidden_layers + n_outputs);

    /// list of weights (size of n_weights)
    weights_.set_capacity(n_total_weights_);
    /// list of outputs (input array and output for each of the n_total_neurons)
    outputs_.set_capacity(n_total_neurons_);
    /// deltas for each hidden and output neuron (n_total_neurons - n_inputs)
    deltas_.set_capacity(n_total_neurons_ - n_inputs_);
}

void Nn::randomize() {
    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::uniform_real_distribution<FpType> dis(-0.5, 0.5);

    // Iterate thru weights_, randomizing
    for (auto& elem : weights_) {
        elem = dis(gen);
    }
}

void Nn::init_sigmoid_lookup() {
    const FpType table_idx_fp = static_cast<FpType>(lookup_table_.size());
    const FpType coef = (sigmoid_dom_max_ - sigmoid_dom_min_) / table_idx_fp;

    /// @todo Interval needs to be a member, and what is the table_interval_ for? Lookup table
    /// table_interval_?
    table_interval_ = table_idx_fp / (sigmoid_dom_max_ - sigmoid_dom_min_);

    for (std::size_t i = 0; i < lookup_table_.size(); ++i) {
        lookup_table_[i] = sigmoid_activation(sigmoid_dom_min_ + coef * static_cast<FpType>(i));
    }
}
FpType Nn::activation_hidden(FpType neuron) {
    switch (hidden_activation_func_) {
    case ActivationFuncKind::SIGMOID: {
        return sigmoid_activation(neuron);
    }
    case ActivationFuncKind::SIGMOID_CACHED: {
        return sigmoid_activation_cached(neuron);
    }
    case ActivationFuncKind::SIGMOID_LINEAR: {
        return sigmoid_activation_linear(neuron);
    }
    case ActivationFuncKind::SIGMOID_THRESHOLD: {
        return sigmoid_activation_threshold(neuron);
    }
    }
    return sigmoid_activation(neuron);
}

FpType Nn::activation_output(FpType neuron) {
    switch (output_activation_func_) {
    case ActivationFuncKind::SIGMOID: {
        return sigmoid_activation(neuron);
    }
    case ActivationFuncKind::SIGMOID_CACHED: {
        return sigmoid_activation_cached(neuron);
    }
    case ActivationFuncKind::SIGMOID_LINEAR: {
        return sigmoid_activation_linear(neuron);
    }
    case ActivationFuncKind::SIGMOID_THRESHOLD: {
        return sigmoid_activation_threshold(neuron);
    }
    }
    return sigmoid_activation(neuron);
}

mtl::Result<mtl::Ok, mtl::Error> Nn::run_without_hidden_layers(RunState& state) {

    for (std::size_t i = 0; i < n_outputs_; ++i) {
        FpType sum = *(state.weight_iter) * -1.0;
        state.weight_iter.next();

        for (const auto& input_elem : inputs_) {
            sum += *(state.weight_iter) * input_elem;
            state.weight_iter.next();
        }
        outputs_.push_back(activation_output(sum));
    }
    return Ok{};
}

mtl::Result<mtl::Ok, mtl::Error> Nn::run_input_layer(RunState& state) {

    for (std::size_t i = 0; i < n_hidden_; ++i) {
        FpType sum = *(state.weight_iter) * -1.0;
        state.weight_iter.next();
        for (const auto& input_elem : inputs_) {
            sum += *(state.weight_iter) * input_elem;
            state.weight_iter.next();
        }
        outputs_.push_back(activation_hidden(sum));
    }
    return Ok{};
}

mtl::Result<mtl::Ok, mtl::Error> Nn::run_hidden_layers(RunState& state) {
    // Wtf, why start at 1?
    for (std::size_t i = 1; i < n_hidden_layers_; ++i) {
        for (std::size_t j = 0; j < n_hidden_; ++j) {
            FpType sum = *(state.weight_iter) * -1.0;
            state.weight_iter.next();
            for (std::size_t k = 0; k < n_hidden_; ++k) {
                sum += *(state.weight_iter) * inputs_[k];
                state.weight_iter.next();
            }
            outputs_.push_back(activation_hidden(sum));
        }
    }

    return Ok{};
}

mtl::Result<mtl::Ok, mtl::Error> Nn::run_output_layer(RunState& state) {

    for (std::size_t i = 0; i < n_outputs_; ++i) {
        FpType sum = *(state.weight_iter) * -1.0;
        state.weight_iter.next();
        for (std::size_t k = 0; k < n_hidden_; ++k) {
            sum += *(state.weight_iter) * inputs_[k];
            state.weight_iter.next();
        }
        outputs_.push_back(activation_output(sum));
    }

    return Ok{};
}

mtl::Result<mtl::Ok, mtl::Error> Nn::run(const mtl::DynArray<FpType>& inputs) {
    // auto output_iter = outputs_.begin();

    // auto weight_iter = weights_.cbegin();
    RunState state;
    { state.weight_iter = weights_.c_iter(); }

    inputs_ = inputs;

    // Special case for no hidden layers
    if (hidden_layers_configured()) {
        return run_without_hidden_layers(state);
    }
    auto res = run_input_layer(state);
    if (res.is_err()) {
        return res;
    }
    res = run_hidden_layers(state);
    if (res.is_err()) {
        return res;
    }

    res = run_output_layer(state);
    if (res.is_err()) {
        return res;
    }

    if (weights_.size() != n_total_weights_) {
        return mtl::Result<Ok, mtl::Error>::err(
            "weights_.size() != n_total_weights_, expected to create all weights");
    }

    if (outputs_.size() != n_outputs_) {
        return mtl::Result<Ok, mtl::Error>::err(
            "outputs_.size() != n_outputs_, expected to create all outputs");
    }

    return Ok{};
}

mtl::Result<mtl::Ok, mtl::Error> set_output_layer_deltas() {

    // in genann:double const *o = ann->output + ann->inputs + ann->hidden * ann->hidden_layers
    // This is taking the (output_ptr + input_ptr + (0 or 1 for hidden) * n_hidden_layers to find
    // the delta

    return Ok{};
}
mtl::Result<mtl::Ok, mtl::Error> set_hidden_layer_deltas() { return Ok{}; }
mtl::Result<mtl::Ok, mtl::Error> train_output_layer() { return Ok{}; }
mtl::Result<mtl::Ok, mtl::Error> train_hidden_layer() { return Ok{}; }
mtl::Result<mtl::Ok, mtl::Error> Nn::train(const mtl::DynArray<FpType>& inputs,
                                           const mtl::DynArray<FpType>& desired_outputs,
                                           FpType learning_rate) {
    run(inputs);

    static_cast<void>(learning_rate);
    static_cast<void>(desired_outputs);

    return Ok{};
}

} // namespace nn