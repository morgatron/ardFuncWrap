#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <tuple>
#include "apply_tuple.h"
using std::cout;
using std::endl;
using std::string;
std::stack<string> argStack;
std::stack<string> retStack;

template<typename T>
T getNextArg(){
	std::stringstream ss;
	ss.str(argStack.top());
	argStack.pop();
	T convertedVar;
	ss >> convertedVar;
	return convertedVar;
}

template <typename Fn, typename funcSig> struct funcWrap;  

template <typename Fn, typename R, typename ...Args>
struct funcWrap<Fn, R(Args...)>       // specialized for typename = R(Args...)
{
	using funcType= Fn;//R(Args...);
	funcType *wrappedFunc;

	funcWrap(funcType &inputFunc){
		this->wrappedFunc=&inputFunc;
	}
	void operator()(){
		auto tup =std::make_tuple(getNextArg<Args>()...);
		std::stringstream ss;
		ss << apply_tuple(*wrappedFunc, tup);
		retStack.push(ss.str());
	}
};

int add(float a, float b){
	return a+b;
}

int main(){
	argStack.push(string("2"));
	argStack.push(string("5"));
	auto wrapped=funcWrap<decltype(add), decltype(add)>(add);
	wrapped();
	cout << retStack.top();
	return 0;
}
