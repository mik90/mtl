#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "nn.hpp"

nn::Nn get_default_nn() {

    const std::size_t n_inputs = 2;
    const std::size_t n_hidden_layers = 1;
    const std::size_t n_hidden = 1;
    const std::size_t n_outputs = 2;
    const auto hidden_act_func = nn::ActivationFuncKind::SIGMOID_CACHED;
    const auto output_act_func = nn::ActivationFuncKind::SIGMOID_CACHED;
    return nn::Nn(n_inputs, n_hidden_layers, n_hidden, n_outputs, hidden_act_func, output_act_func);
}

TEST(NnTest, ctor) {
    const std::size_t n_inputs = 2;
    const std::size_t n_hidden_layers = 1;
    const std::size_t n_hidden = 1;
    const std::size_t n_outputs = 2;
    const auto hidden_act_func = nn::ActivationFuncKind::SIGMOID_CACHED;
    const auto output_act_func = nn::ActivationFuncKind::SIGMOID_CACHED;
    const auto nn = nn::Nn::make_nn(n_inputs, n_hidden_layers, n_hidden, n_outputs, hidden_act_func,
                                    output_act_func);

    ASSERT_TRUE(nn.is_some());
}

TEST(NnTest, randomize) {
    auto nn = get_default_nn();
    // const mtl::DynArray<double> starting_weights(nn.get_weights());
    nn.randomize();
}