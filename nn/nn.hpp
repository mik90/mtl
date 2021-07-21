#pragma once

#include "mtl/dyn_array.hpp"
#include "mtl/maybe.hpp"
#include "mtl/result.hpp"
#include "mtl/static_array.hpp"
#include <cstddef>
#include <cstdio>
#include <functional>

/**
 * @brief main nn defs. Based off of https://github.com/codeplea/genann/blob/master/genann.h
 */

namespace nn {
struct RunState;

/// @brief type for weights, outputs, deltas, and pretty much everything
using FpType = double;

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
    /// @todo just use a func pointer
    ActivationFuncKind hidden_activation_func_;
    ActivationFuncKind output_activation_func_;
    /// list of weights (size of n_weights)
    mtl::DynArray<FpType> weights_;
    /// list of outputs for each of the n_total_neurons
    mtl::DynArray<FpType> outputs_;
    /// list of inputs for each of the n_total_neurons
    mtl::DynArray<FpType> inputs_;
    /// deltas for each hidden and output neuron (n_total_neurons - n_inputs)
    mtl::DynArray<FpType> deltas_;
    mtl::Result<mtl::Ok, mtl::Error> run_input_layer(RunState& state);
    mtl::Result<mtl::Ok, mtl::Error> run_hidden_layers(RunState& state);
    mtl::Result<mtl::Ok, mtl::Error> run_output_layer(RunState& state);
    mtl::Result<mtl::Ok, mtl::Error> run_without_hidden_layers(RunState& state);

    mtl::Result<mtl::Ok, mtl::Error>
    set_output_layer_deltas(const mtl::DynArray<FpType>& desired_outputs);
    mtl::Result<mtl::Ok, mtl::Error> set_hidden_layer_deltas();
    mtl::Result<mtl::Ok, mtl::Error> train_output_layer();
    mtl::Result<mtl::Ok, mtl::Error> train_hidden_layers();

    mtl::StaticArray<FpType, 4096> lookup_table_;
    FpType table_interval_ = 0;
    /// @todo wtf r these
    static constexpr FpType sigmoid_dom_min_ = -15.0;
    static constexpr FpType sigmoid_dom_max_ = 15.0;

  public:
    /**
     * @param n_inputs must be at least 1
     * @param n_hidden_layers must be at least 0.
     * @param n_hidden must be at least 1 if n_hidden_layers is greater than 0. What is this vlaue??
     * @param n_outputs Must be at least 1
     */
    Nn(std::size_t n_inputs, std::size_t n_hidden_layers, std::size_t n_hidden,
       std::size_t n_outputs, ActivationFuncKind hidden_activation_func,
       ActivationFuncKind output_activation_func);
    static mtl::Maybe<Nn> make_nn(std::size_t n_inputs, std::size_t n_hidden_layers,
                                  std::size_t n_hidden, std::size_t n_outputs,
                                  ActivationFuncKind hidden_activation_func,
                                  ActivationFuncKind output_activation_func);
    /// @todo serialize
    /// @todo deserialize
    mtl::Result<mtl::Ok, mtl::Error> run(const mtl::DynArray<FpType>& inputs);

    /// @brief train that does a 'backprop update'
    mtl::Result<mtl::Ok, mtl::Error> train(const mtl::DynArray<FpType>& inputs,
                                           const mtl::DynArray<FpType>& desired_outputs,
                                           FpType learning_rate);
    void randomize();
    bool hidden_layers_configured() const noexcept { return n_hidden_layers_ > 0; }
    void init_sigmoid_lookup();
    FpType sigmoid_activation(FpType neuron);
    /// @todo use mtl::memoized
    FpType sigmoid_activation_cached(FpType neuron);
    FpType sigmoid_activation_threshold(FpType neuron);
    FpType sigmoid_activation_linear(FpType neuron);
    FpType activation_hidden(FpType neuron);
    FpType activation_output(FpType neuron);

    const mtl::DynArray<FpType>& borrow_outputs() const { return outputs_; }
    const mtl::DynArray<FpType>& borrow_weights() const { return weights_; }
    mtl::DynArray<FpType> copy_weights() const { return weights_.copy(); }
    std::size_t n_outputs() const noexcept { return n_outputs_; }
};

// gennann uses a bunch of variables in a single function, i'll just pass around a state
struct RunState {
    mtl::ConstIterator<FpType> weight_iter; // Similar to genann's `w`
};
} // namespace nn