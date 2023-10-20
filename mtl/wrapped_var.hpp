#pragma once

#include <mutex>
#include <type_traits>
#include <utility>

namespace mtl {

  /**
   * @brief RAII style accessor for a WrappedVar var
   * @tparam UnderlyingVarType the variable type protected by the mutex
   * @tparam MutexType the mutex that protects UnderlyingType
   * @tparam LockType the type used for locking MutexType
   */
  template <class UnderlyingVarType, class LockType>
  class VarAccessor {
  public:
    // General case
    VarAccessor(UnderlyingVarType& var, LockType&& lock) : var_(var), lock_(std::move(lock)) {}

    UnderlyingVarType& get_ref() noexcept { return var_; }

    const UnderlyingVarType& get_cref() const noexcept { return var_; }

    UnderlyingVarType clone() const { return var_; }

  private:
    UnderlyingVarType& var_;
    LockType lock_;
  };

  /**
   * @brief Mutex wrapper around variable of type UnderlyingVarType
   * @tparam UnderlyingVarType the variable type protected by the mutex
   * @tparam MutexType the mutex that protects UnderlyingType
   */
  template <class UnderlyingVarType>
  class WrappedVar {
  public:
    using MutexType = std::mutex;
    /**
     * @brief Constructs mutex-protected variable in-place. Pass in args like you would for
     *        std::vector::emplace_back.
     */
    template <class... Args>
    explicit WrappedVar(Args&&... args) : var_(std::forward<Args>(args)...) {}

    /**
     * @brief Getter that returns a variable accessor helper type
     * @return VarAccessor that can be locked for modifying the underlying variable
     */
    VarAccessor<UnderlyingVarType, std::unique_lock<MutexType>> get() {
      return {var_, std::unique_lock<MutexType>{mutex_}};
    }

    /**
     * @brief Pass-thru to check if anyone else is accessing the variable.
     */
    bool try_lock() { return mutex_.try_lock(); }

  private:
    UnderlyingVarType var_;
    MutexType mutex_;
  };

}  // namespace mtl