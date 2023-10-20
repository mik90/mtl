#pragma once

namespace mtl {
  template <typename T>
  class View {
  public:
    View() = delete;

    View(T& value) : value_(value) {}

    View(const View<T>& other) : value_(other.value_) {}
    // Copies of vievs should be cheap enough, moves aren't needed
    // Really it's just that i can't get this to compile if i do
    /*
    std::string my_string = "hello";
    mtl::View view_a{my_string};
    mtl::View view_b = std::move(view_a);
    EXPECT_EQ(view_b, my_string);
    */
    View(View<T>&& other) = delete;

    View<T>& operator=(const T& other) {
      value_ = other;
      return *this;
    }
    View<T>& operator=(const View<T>& other) {
      value_ = other.value_;
      return *this;
    }

    // This == is needed, evidently? https://stackoverflow.com/a/58780963
    auto operator==(const View<T>& other) const { return value_ == other.value_; }

    auto operator<=>(const View<T>& other) const { return value_ <=> other.value_; }

    auto operator==(const T& other) const { return value_ == other; }

    auto operator<=>(T& other) const { return value_ <=> other; }

    T* operator->() { return &value_; }
    const T* operator->() const { return value_; }

  private:
    T& value_;
  };
}  // namespace mtl