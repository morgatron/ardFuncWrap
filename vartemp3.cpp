#include <iostream>
#include <stack>
#include <functional>
#include<cstdlib>
#include <string>
#include <sstream>
#include <tuple>
#include "tuple_print.h"
#include "apply_tuple.h"
#include "type_print.h"
using namespace std::placeholders;
using std::cout;
using std::endl;
std::stack<char*> mystack;

template<typename T>
T argGetter(){
	std::stringstream ss;
	ss.str(mystack.top());
	mystack.pop();
	cout << "st: " << ss.str() << endl;
	T var1;//=atof
	ss >> var1;
	if(ss.fail()){
		//Some kind of null value
		cout << "failed parsing" << endl;
	}
	return var1;
}


//template <typename T> using Fn = T;
template <typename Fn, typename funcSig> struct funcWrap;         // leave undefined


//};

template <typename Fn, typename R, typename ...Args>
struct funcWrap<Fn, R(Args...)>                  // specialized for typename = R(Args...)

{
	using funcType= Fn;//R(Args...);
	//std::function<funcType> localFunc;
	funcType *localFunc;

	funcWrap(funcType &inputFunc){
		this->localFunc=&inputFunc;
	}
	//template<class T>
	//funcWrap(T &obj, funcType &inputFunc){
	//	this->localFunc=inputFunc;
	//}
	void operator()(){
		auto tup =std::make_tuple(argGetter<Args>()...);
		print(tup);
		cout << apply_tuple(*localFunc, tup);
		// send stuff back!
	}
};

int min(float a, float b, float c){
	if(a>b){
		return b;
	}

	if(c>a)
		return a;
	return c;
}

class TestClass{
	public:
	int min(float a, float b){
		if(a>b){
			return b;
		}

		return a;
	}


};
template <typename T>
struct tell_me_the_type;

int main(){
	mystack.push((char*)"227.0");
	mystack.push((char*)"84.3");
	mystack.push((char*)"82.2");
	mystack.push((char*)"81.5");
	TestClass testClass;
	//cout << argGetter<float>() << endl;
	//auto fw=funcWrap<decltype(min)>(min);
	//decltype(&testClass::min) typ;
	std::function<decltype(min)> stdFunc=min; 
	auto fw=funcWrap<decltype(stdFunc), decltype(min)>(stdFunc);
	//auto fw=funcWrap<decltype(min)>(min);
	fw();
	//std::function<decltype(&TestClass::min)> bound; 
	//bound=std::bind(&TestClass::min, &testClass, _1, _2);
	//cout << bound(5,3) << endl;;
	//cout << type_name<decltype(bound)>() << endl;
	//tell_me_the_type<decltype(bound)>;
	//

	//fw();
	return 0;
}
