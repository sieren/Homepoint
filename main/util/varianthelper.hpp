#include <type_traits>
#include <variant>

namespace util
{

template <class... Fs>
struct OverLoaded;

template <class F1>
struct OverLoaded<F1> : F1 {
    using F1::operator();

    OverLoaded(F1&& head) : F1(std::forward<F1>(head)) {}
};

template <class F1, class... Fs>
struct OverLoaded<F1, Fs...> : F1, OverLoaded<Fs...> {
    using F1::operator();
    using OverLoaded<Fs...>::operator();

    OverLoaded(F1&& head, Fs&&... rest)
        : F1(std::forward<F1>(head)),
        OverLoaded<Fs...>(std::forward<Fs>(rest)...) {}
};

template <class... Fs>
OverLoaded<Fs...> overloaded(Fs&&... fs) {
    return OverLoaded<Fs...>(std::forward<Fs>(fs)...);
}

} // namespace util
