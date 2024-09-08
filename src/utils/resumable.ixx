module;
#include <coroutine>
#include <exception>

export module zjucad.gauwn.utils.resumable;

namespace zjucad::gauwn::utils {
export struct Resumable {
    struct PromiseType {
        Resumable get_return_object() {
            return Resumable(std::coroutine_handle<PromiseType>::from_promise(*this));
        }
        auto initial_suspend() noexcept { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
    };
    using promise_type = PromiseType;

    std::coroutine_handle<PromiseType> _coroutine = nullptr;
    Resumable() = default;
    Resumable(Resumable const &) = delete;
    Resumable &operator=(Resumable const &) = delete;
    Resumable(Resumable &&other) noexcept : _coroutine(other._coroutine) {
        other._coroutine = nullptr;
    }
    Resumable &operator=(Resumable &&other) noexcept {
        if (&other != this) {
            _coroutine = other._coroutine;
            other._coroutine = nullptr;
        }
    }
    explicit Resumable(std::coroutine_handle<PromiseType> coroutine) : _coroutine(coroutine) {
    }
    ~Resumable() {
        if (_coroutine) {
            _coroutine.destroy();
        }
    }
    void resume() { _coroutine.resume(); }
};

} // namespace zjucad::gauwn::utils