#include <cstddef>
#include <type_traits>
#include <tuple>

template <typename T>
struct function_traits
    : public function_traits<decltype(&T::operator())>
{};
// For generic types, directly use the result of the signature of its 'operator()'

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
// we specialize for pointers to member function
{
    enum { arity = sizeof...(Args) };
    // arity is the number of arguments.

    typedef ReturnType result_type;

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        // the i-th argument is equivalent to the i-th tuple element of a tuple
        // composed of those arguments.
    };
};
int min(int a, int b){
	if (a>b)
		return b;
	else
		return a;
}
// test code below:
int main()
{
    //auto lambda = [](int i) { return long(i*10); };
	auto lambda = &min;

    typedef function_traits<decltype(lambda)> traits;

    static_assert(std::is_same<long, traits::result_type>::value, "err");
    static_assert(std::is_same<int, traits::arg<0>::type>::value, "err");

    return 0;
}
