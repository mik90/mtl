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

    // Fill array with random weights
    weights_.fill_with_generator([&dis, &gen] { return dis(gen); });
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
        // TODO Segfault here
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
    if (!hidden_layers_configured()) {
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

mtl::Result<mtl::Ok, mtl::Error>
Nn::set_output_layer_deltas(const mtl::DynArray<FpType>& desired_outputs) {

    auto delta_iter = deltas_.iter();
    auto desired_output_iter = desired_outputs.c_iter();
    auto output_iter = outputs_.c_iter();
    if (output_activation_func_ == ActivationFuncKind::SIGMOID_LINEAR) {
        for (std::size_t i = 0; i < n_outputs_; ++i) {
            *delta_iter++ = *desired_output_iter++ - *output_iter++;
        }
    } else {
        for (std::size_t i = 0; i < n_outputs_; ++i) {
            *delta_iter++ =
                (*desired_output_iter - *output_iter) * *output_iter * (FpType(1.0) - *output_iter);
            ++desired_output_iter;
            ++output_iter;
        }
    }

    return Ok{};
}
mtl::Result<mtl::Ok, mtl::Error> Nn::set_hidden_layer_deltas() {
#if 0
    for (h = ann->hidden_layers - 1; h >= 0; --h) {

        /* Find first output and delta in this layer. */
        double const *o = ann->output + ann->inputs + (h * ann->hidden);
        double *d = ann->delta + (h * ann->hidden);

        /* Find first delta in following layer (which may be hidden or output). */
        double const * const dd = ann->delta + ((h+1) * ann->hidden);

        /* Find first weight in following layer (which may be hidden or output). */
        double const * const ww = ann->weight + ((ann->inputs+1) * ann->hidden) + ((ann->hidden+1) * ann->hidden * (h));

        for (j = 0; j < ann->hidden; ++j) {

            double delta = 0;

            for (k = 0; k < (h == ann->hidden_layers-1 ? ann->outputs : ann->hidden); ++k) {
                const double forward_delta = dd[k];
                const int windex = k * (ann->hidden + 1) + (j + 1);
                const double forward_weight = ww[windex];
                delta += forward_delta * forward_weight;
            }

            *d = *o * (1.0-*o) * delta;
            ++d; ++o;
        }
    }
#endif
    auto hidden_layers_left = n_hidden_layers_;
    // Iterate through hidden layers backwards
    static_cast<void>(hidden_layers_left);

    return Ok{};
}
mtl::Result<mtl::Ok, mtl::Error> Nn::train_output_layer() { return Ok{}; }
mtl::Result<mtl::Ok, mtl::Error> Nn::train_hidden_layers() { return Ok{}; }

mtl::Result<mtl::Ok, mtl::Error> Nn::train(const mtl::DynArray<FpType>& inputs,
                                           const mtl::DynArray<FpType>& desired_outputs,
                                           FpType learning_rate) {
    run(inputs);

    static_cast<void>(learning_rate);
    static_cast<void>(desired_outputs);

    auto res = set_output_layer_deltas(desired_outputs);
    if (res.is_err()) {
        return res;
    }

    if (n_hidden_ > 0) {
        res = set_hidden_layer_deltas();
        if (res.is_err()) {
            return res;
        }
    }

    res = train_output_layer();
    if (res.is_err()) {
        return res;
    }

    res = train_hidden_layers();
    if (res.is_err()) {
        return res;
    }

    return Ok{};
}

} // namespace nn