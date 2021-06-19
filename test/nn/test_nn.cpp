#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "nn.hpp"
#include <cmath>
#include <limits>

nn::Nn get_default_nn() {

    const std::size_t n_inputs = 2;
    const std::size_t n_hidden_layers = 1;
    const std::size_t n_hidden = 1;
    const std::size_t n_outputs = 2;
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
    const auto starting_weights = nn.get_weights().copy();
    nn.randomize();
    const auto ending_weights = nn.get_weights().copy();

    ASSERT_GE(ending_weights.size(), 0);
    ASSERT_EQ(starting_weights.size(), ending_weights.size());

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