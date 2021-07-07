#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "nn.hpp"
#include <cmath>
#include <limits>

using nn::FpType;

nn::Nn get_default_nn() {
    const std::size_t n_inputs = 2;
    const std::size_t n_hidden_layers = 1;
    const std::size_t n_hidden = 1;
    const std::size_t n_outputs = 2;
    const auto hidden_act_func = nn::ActivationFuncKind::SIGMOID_CACHED;
    const auto output_act_func = nn::ActivationFuncKind::SIGMOID_CACHED;
    return {n_inputs, n_hidden_layers, n_hidden, n_outputs, hidden_act_func, output_act_func};
}
nn::Nn get_nn_no_hidden() {
    const std::size_t n_inputs = 1;
    const std::size_t n_hidden_layers = 0;
    const std::size_t n_hidden = 0;
    const std::size_t n_outputs = 1;
    const auto hidden_act_func = nn::ActivationFuncKind::SIGMOID_CACHED;
    const auto output_act_func = nn::ActivationFuncKind::SIGMOID_CACHED;
    return {n_inputs, n_hidden_layers, n_hidden, n_outputs, hidden_act_func, output_act_func};
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

bool are_equal(double a, double b) {
    const auto diff = std::fabs(a - b);
    return diff < std::numeric_limits<double>::epsilon();
}

TEST(NnTest, randomize) {
    auto nn = get_default_nn();
    const auto starting_weights = nn.borrow_weights().copy();
    nn.randomize();
    const auto ending_weights = nn.borrow_weights().copy();

    ASSERT_GE(ending_weights.size(), 0);
    ASSERT_GT(ending_weights.size(), starting_weights.size());

    for (std::size_t i = 0; i < ending_weights.size(); ++i) {
        ASSERT_FALSE(are_equal(starting_weights[i], ending_weights[i]));
    }
}

TEST(NnTest, init_sigmoid_lookup) {
    auto nn = get_default_nn();
    nn.randomize();
    nn.init_sigmoid_lookup();
    SUCCEED();
}

TEST(NnTest, run_without_hidden_layers) {
    auto nn = get_nn_no_hidden();
    const auto configured_n_outputs = nn.n_outputs();
    const auto inputs = mtl::DynArray<double>{0.0, 1.0, 0.3, 2.0};

    nn.randomize();
    nn.init_sigmoid_lookup();

    const auto result = nn.run(inputs);

    ASSERT_TRUE(result.is_ok());
    const auto& outputs = nn.borrow_outputs();
    ASSERT_EQ(outputs.size(), configured_n_outputs);
}