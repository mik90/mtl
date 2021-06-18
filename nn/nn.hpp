#pragma once

#include "mtl/dyn_array.hpp"
#include "mtl/maybe.hpp"
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

class Nn {
  private:
    std::size_t n_inputs_;
    std::size_t n_hidden_layers_;
    /// @todo what is this??? (hidden). Is it the number of hidden neurons?
    std::size_t n_hidden_;
    std::size_t n_outputs_;
    std::size_t n_total_weights_;
    std::size_t n_total_neurons_;
    ActivationFuncKind hidden_activation_func_;
    ActivationFuncKind output_activation_func_;
    /// list of weights (size of n_weights)
    mtl::DynArray<double> weights_;
    /// list of outputs (input array and output for each of the n_total_neurons)
    mtl::DynArray<double> outputs_;
    /// deltas for each hidden and output neuron (n_total_neurons - n_inputs)
    mtl::DynArray<double> deltas_;

  public:
    /**
     * @param n_inputs must be at least 1
     * @param n_hidden_layers must be at least 0.
     * @param n_hidden must be at least 1 if n_hidden_layers is greater than 0. What is this vlaue??
     * @param n_outputs Must be at least 1
     */
    Nn(std::size_t n_inputs, std::size_t n_hidden_layers, std::size_t n_hidden,
       std::size_t n_outputs, std::size_t n_total_weights, std::size_t n_total_neurons,
       ActivationFuncKind hidden_activation_func, ActivationFuncKind output_activation_func);
    static mtl::Maybe<Nn> make_nn(std::size_t n_inputs, std::size_t n_hidden_layers,
                                  std::size_t n_hidden, std::size_t n_outputs,
                                  ActivationFuncKind hidden_activation_func,
                                  ActivationFuncKind output_activation_func);
    /// @todo serialize
    /// @todo deserialize
    /// @todo run that returns a list of outputs (genann run)
    /// @todo train that does a 'backprop update'
    void init_sigmoid_lookup();
    double sigmoid_activation(double neuron);
    /// @todo use mtl::memoized
    double sigmoid_activation_cached(double neuron);
    double sigmoid_activation_threshold(double neuron);
    double sigmoid_activation_linear(double neuron);
};

} // namespace nn