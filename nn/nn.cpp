#include "nn.hpp"
#include "mtl/algorithm.hpp"
#include "mtl/static_array.hpp"
#include <cassert>
#include <cmath>
#include <random>

namespace nn {

double Nn::sigmoid_activation(double neuron) {
    /// @todo where do these numbers comefrom
    if (neuron < -45.0) {
        return 0;
    } else if (neuron > 45.0) {
        return 1;
    } else {
        return 1.0 / (1 + std::exp(-neuron));
    }
}

/// @todo Create Lookup table struct
constexpr std::size_t lookup_size = 4096;
auto lookup_table = mtl::StaticArray<double, lookup_size>();
double interval;
/// @todo wtf r these
constexpr double sigmoid_dom_min = -15.0;
constexpr double sigmoid_dom_max = 15.0;

double Nn::sigmoid_activation_cached(double neuron) {
    assert(!std::isnan(neuron));

    if (neuron < sigmoid_dom_min) {
        return *(lookup_table.cbegin());
    }
    if (neuron >= sigmoid_dom_max) {
        return *(lookup_table.cend());
    }

    const auto est = (neuron - sigmoid_dom_min) * interval + 0.5;
    if (est > lookup_size) {
        return *(lookup_table.cend());
    }
    return lookup_table[static_cast<std::size_t>(est)];
}
double Nn::sigmoid_activation_linear(double neuron) { return neuron; }
double Nn::sigmoid_activation_threshold(double neuron) { return neuron > 0; }

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
    std::uniform_real_distribution<double> dis(-0.5, 0.5);

    // Iterate thru weights_, randomizing
    for (auto& elem : weights_) {
        elem = dis(gen);
    }
}

void Nn::init_sigmoid_lookup() {
    const double coef = (sigmoid_dom_max - sigmoid_dom_min) / lookup_size;

    /// @todo Interval needs to be a member, and what is the interval for? Lookup talb interval?
    interval = lookup_size / (sigmoid_dom_max - sigmoid_dom_min);

    for (std::size_t i = 0; i < lookup_table.size(); ++i) {
        lookup_table[i] = sigmoid_activation(sigmoid_dom_min + coef * static_cast<double>(i));
    }
}
double Nn::activation_hidden(double neuron) {
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

double Nn::activation_output(double neuron) {
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

mtl::DynArray<double> Nn::run(const mtl::DynArray<double>& inputs) {
    outputs_ = inputs;
    auto output_iter = outputs_.begin();

    auto weight_iter = weights_.cbegin();

    // Special case for no hidden layers
    if (!n_hidden_layers_) {
        for (std::size_t i = 0; i < outputs_.size(); ++i) {
            double sum = *weight_iter++ * -1.0;
            for (const auto& input_elem : inputs) {
                sum = *weight_iter++ * input_elem;
            }
            *output_iter++ = activation_output(sum);
        }
        return outputs_.copy();
    }
    return outputs_.copy();
}
} // namespace nn