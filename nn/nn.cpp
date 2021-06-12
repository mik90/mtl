#include "nn.hpp"
#include "mtl/static_array.hpp"
#include <cmath>
namespace nn {

double nn::sigmoid_activation(double neuron) {
    /// @todo where do these numbers comefrom
    if (neuron < -45.0) {
        return 0;
    } else if (neuron > 45.0) {
        return 1;
    } else {
        return 1.0 / (1 + std::exp(-neuron));
    }
}
constexpr size_t lookup_size = 4096;
auto lookup_table = mtl::StaticArray<double, lookup_size>();
void nn::init_sigmoid_lookup() {}
double nn::sigmoid_activation_cached(double neuron) { return neuron; }
double nn::sigmoid_activation_threshold(double neuron) { return neuron; }
double nn::sigmoid_activation_linear(double neuron) { return neuron; }
} // namespace nn