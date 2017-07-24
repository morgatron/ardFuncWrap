
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cstddef>

// ----------------------------------------------------------------------------

template <typename T>
typename std::decay<T>::type makeArg(std::stack<std::string>& args) {
    typename std::decay<T>::type rc;
    if (args.empty() || !(std::istringstream(args.top()) >> rc)) {
        throw std::runtime_error("can't create argument from '" + (args.empty()? "<empty>": args.top()) + "'");
    }
    args.pop();
    return rc;
}

// ----------------------------------------------------------------------------

namespace util
{
    template<typename T, T...>
    struct integer_sequence {
    };
    template<std::size_t... I>
    using index_sequence = integer_sequence<std::size_t, I...>;

    template <typename T, std::size_t N, T... I>
    struct integer_sequencer {
        using type = typename integer_sequencer<T, N - 1, N - 1, I...>::type;
    };
    template <typename T, T... I>
    struct integer_sequencer<T, 0, I...> {
        using type =  integer_sequence<T, I...>;
    };

    template<typename T, T N>
    using make_integer_sequence = typename integer_sequencer<T, N>::type;
    template<std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template <typename F, typename T, std::size_t... I>
    auto apply_aux(F&& fun, T&& tuple, index_sequence<I...>) -> decltype(fun(std::get<I>(tuple)...)) {
        return fun(std::get<I>(tuple)...);
    }

    template <typename F, typename T>
    auto apply(F&& f, T&& t)
        -> decltype(apply_aux(std::forward<F>(f), std::forward<T>(t), make_index_sequence<std::tuple_size<typename std::decay<T>::type>::value>())) {
        return apply_aux(std::forward<F>(f), std::forward<T>(t), make_index_sequence<std::tuple_size<typename std::decay<T>::type>::value>());
    }
}

// ----------------------------------------------------------------------------

template <typename S, typename F> class funcWrap;

template <typename R, typename... A, typename F>
class funcWrap<R(A...), F> {
private:
    F fun;

public:
    funcWrap(F fun): fun(fun) {}

    std::string operator()(std::stack<std::string>& args) {
        std::tuple<typename std::decay<A>::type...> t{ makeArg<A>(args)... };
        std::ostringstream out;
        out << util::apply(this->fun, t);
        return out.str();
    }
};

template <typename R, typename... A, typename S, typename... B>
class funcWrap<R(A...), R (S::*)(B...)> {
private:
    R (S::*mem)(B...);

public:
    funcWrap(R (S::*mem)(B...)): mem(mem) {}

    std::string operator()(std::stack<std::string>& args) {
        std::tuple<typename std::decay<A>::type...> t{ makeArg<A>(args)... };
        std::ostringstream out;
        out << util::apply([=](S& s, B... b){ return (s.*(this->mem))(b...); }, t);
        return out.str();
    }
};

// ----------------------------------------------------------------------------

template <typename R, typename... A>
funcWrap<R(A...), R(*)(A...)>
wrap(R(*fun)(A...)) { // deal with functions
    return funcWrap<R(A...), R(*)(A...)>(fun);
}
template <typename Signature, typename F>
funcWrap<Signature, F>
wrap(F&& f) { // deal with function objects
    return funcWrap<Signature, F>(std::forward<F>(f));
}
template <typename R, typename S, typename... A>
funcWrap<R(S&, A...), R (S::*)(A...)>
wrap(R(S::*mem)(A...)) {
    return funcWrap<R(S&, A...), R (S::*)(A...)>(mem);
}
