#ifndef __INC_ARGWRAP_H
#define __INC_ARGWRAP_H
/* USAGE
 * A small library to make it easier to wrap functions on a microntroller sot that they can be called via a text interface, i.e. from the serial port.
 * As much as possible, the usage should be to simply write "exposeSerial("name", func)" for some function, and the library takes care of the rest based on argument types.
 * This should work for most functions that have arguments consisting only of basic types (strings, numbers). Anything else will require either another wrapping function that converts from basic types, or that a template specialisation 
 * "int initFromString<T>(std::string, T &obj )"
 * Could also be (maybe) T initFromString<T>(std::string);
 * 
 * 
 * A common pattern when using microcontrollers such as Arduinos is to 
 * debug via print statements. Of course, any serious debugging should probably be done with JTAG or something similar, but that's not always convenient.
 *
 * This should also be useful when writing the interface for an MCU.
 * In this case, you won't always want the default parsing. In this case you again have two options: make a wrapper function that calls the things you are interested in, and takes standard types as inputs. Or use exposeSerialRaw("name", func) where func takes and returns a string (i.e. std::string func(std::string) ), and is responsible for it's own parsing.
 */



//#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cstddef>
#include "initFromString.h"








// ----------------------------------------------------------------------------

template <typename T>
typename std::decay<T>::type makeArg(std::stack<std::string>& args, bool& bSucceeded) {
    typename std::decay<T>::type rc;
	if(!bSucceeded)
		return rc;
    //if (args.empty() || !(std::istringstream(args.top()) >> rc)) {
    if (args.empty() || ! initFromString(args.top(), rc)) {
        //throw std::runtime_error("can't create argument from '" + (args.empty()? "<empty>": args.top()) + "'");
		bSucceeded=false;
    }
	else{
		args.pop();
	}
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
    //std::string operator()(std::string argsStr) {
		//std::stack<std::string> args;
		//split(argsStr, args, ' ');
		std::ostringstream out;
		//Could also try passing in a string for use giving the error message, and check it's length to test success.
		bool bSucceeded=true;
        std::tuple<typename std::decay<A>::type...> t{ makeArg<A>(args, bSucceeded)... };
		if(!bSucceeded){
            out << "failed parsing args: ";
            
            if(args.empty()){
                out << "not enough arguments" << std::endl; 
            }
            else{
                out << "couldn't parse arg '" << args.top() << "'." << std::endl;
            }
        }
		else{ //Parsing worked, call the function and parse the output;
			out << util::apply(this->fun, t);
		}
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
    //std::string operator()(std::string argsStr) {
	//	std::stack<std::string> args;
	//	split(argsStr, args, ' ');
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

#endif// __INC_ARGWRAP_H

