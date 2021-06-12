#pragma once

#include <cstdio>
#include <functional>

/**
 * @brief main nn defs. Based off of https://github.com/codeplea/genann/blob/master/genann.h
 */

namespace nn {

enum class ActivationFuncKind {
    SIGMOID,
    SIGMOID_CACHED,
    SIGMOID_THRESHOLD,
    SIGMOID_LINEAR,
};

class nn {
  private:
    size_t n_inputs;
    size_t n_hidden_layers;
    /// @todo what is this (hidden)
    size_t n_hidden;
    size_t n_outputs;
    ActivationFuncKind input_activation_func;
    ActivationFuncKind output_activation_func;
    size_t n_weights;
    size_t n_neurons;
    /// @todo list of weights (size of n_weights)
    /// @todo list of outputs (input array and output for each of the n_neurons)
    /// @todo deltas for each hidden and output neuron (n_neurons - n_inputs)
  public:
    nn(size_t n_inputs, size_t n_hidden_layers, size_t hiddens, size_t n_outputs);
    /// @todo serialize
    /// @todo deserialize
    /// @todo run that returns a list of outputs
    /// @todo train that does a 'backprop update'
    void init_sigmoid_lookup();
    double sigmoid_activation(double neuron);
    /// @todo use mtl::memoized
    double sigmoid_activation_cached(double neuron);
    double sigmoid_activation_threshold(double neuron);
    double sigmoid_activation_linear(double neuron);
};

} // namespace nn