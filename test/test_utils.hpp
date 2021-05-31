#pragma once

/**
 * @brief Unit test helpers
 */

namespace test_utils {

struct MoveOnlyInt {
  private:
    int value_ = 0;

  public:
    MoveOnlyInt(MoveOnlyInt& other) = delete;
    MoveOnlyInt(const MoveOnlyInt& other) = delete;
    MoveOnlyInt& operator=(const MoveOnlyInt& other) = delete;
    MoveOnlyInt& operator=(MoveOnlyInt& other) = delete;

    MoveOnlyInt(MoveOnlyInt&& other) = default;
    MoveOnlyInt& operator=(MoveOnlyInt&& other) = default;

    MoveOnlyInt() = default;
    ~MoveOnlyInt() = default;

    MoveOnlyInt(int value) : value_(value){};
    int get_value() const noexcept { return value_; }
};
inline bool operator==(const MoveOnlyInt& lhs, const MoveOnlyInt& rhs) {
    return lhs.get_value() == rhs.get_value();
}
inline bool operator!=(const MoveOnlyInt& lhs, const MoveOnlyInt& rhs) { return !(lhs == rhs); }
inline bool operator==(const MoveOnlyInt& lhs, int rhs) { return lhs.get_value() == rhs; }
inline bool operator!=(const MoveOnlyInt& lhs, int rhs) { return !(lhs == rhs); }

struct CopyableType {
    CopyableType(CopyableType& other) = default;
    CopyableType(const CopyableType& other) = default;
    CopyableType& operator=(const CopyableType& other) = default;
    CopyableType& operator=(CopyableType& other) = default;

    CopyableType(CopyableType&& other) = default;
    CopyableType& operator=(CopyableType&& other) = default;

    CopyableType() = default;
    ~CopyableType() = default;
};

} // namespace test_utils