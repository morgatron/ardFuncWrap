#include <iostream>
#include <stack>
#include <functional>
#include<cstdlib>
#include <string>
#include <sstream>
#include <tuple>
#include "tuple_print.h"
#include "apply_tuple.h"
//#include "locApply.h"
//#include <redi/index_tuple.h>



std::stack<char*> mystack;

void add1(int &v)
{
  v+=1;
}

void add2(int &v)
{
  v+=2;
}


template <class ArgT, class RetT, class F>
struct A {
    F f;
public:
    A(F f) : f(f) {}

    RetT operator()(ArgT arg) { return f(arg); }
};

typedef void (*vPoint)();
template <class ArgT, class RetT>
struct B {
	typedef RetT (*actPoint)(ArgT);
	actPoint f;
public:
	//
	//(*RetT)(ArgT) locFp;	
	B(actPoint f) : f(f){}

    RetT operator()(ArgT arg) { return (*f)(arg); }
};

/*Class representing one command that 
 * could be given to an arduino. The main properties are the types of
 * arguments/return value.
 *
 * Current: Handles numbers only
 *
 * Issue 1: Map to multiple inputs properly
 *
 * Issue 2: How to deal with e.g. enums? For string type commands, e.g. "direct" or "reverse",
 * it would be nice to map those to an enum or macro. Could even use a c++ has type. Would probably 
 * have to pass in another function pointer to a string->int conversion function, or a hash map.
*/
using std::cout;
using std::endl;

//class<retT, ...args>
//Methods:
//  T argsTuple<args...>
//  operator();
//
//private var:
//  bool bMessedUpParsing;
//  char* functionName;
//operator(){
// args... std::tie<argTypes>
//}
//



//from SO
template<typename Fn, Fn fn, typename... Args>
typename std::result_of<Fn(Args...)>::type
wrapper(Args&&... args) {

    return fn(std::forward<Args>(args)...);
}
#define WRAPPER(FUNC) wrapper<decltype(&FUNC), &FUNC>


template<typename T>
T argGetter(){
	std::stringstream ss;
	ss.str(mystack.top());
	mystack.pop();
	T var1;//=atof
	ss >> var1;
	if(ss.fail()){
		//Some kind of null value
	}
	return var1;
}
//template <size_t N>
//using Vector = Matrix<N, 1>;
//
//template<typename Fn, typename... Args>
//using Fn(Args...) Fn;


template<typename Fn, Fn fn, typename... Args>
typename std::result_of<Fn(Args...)>::type
wrapper2() {

    //return fn(std::forward<Args>(args)...);
    return fn(argGetter<Args>()...);
}
#define WRAPPER2(FUNC) wrapper2<decltype(&FUNC), &FUNC>

//template <typename R, R fn> struct func;         // leave undefined
//
//template <typename R, R fn, typename ...Args>
//struct func<R(Args...), fn>                  // specialized for typename = R(Args...)
//{
//	void operator()(){
//		auto tup =std::make_tuple(argGetter<Args>...);
//		fn(5,1);
//		print(tup);
//	}
//};
//
template<typename Fn, Fn fn, typename... Args>
class wrapper3 {
	template<typename T>
	T prvArgGetter(){
		ss.clear();
		std::string st=mystack.top();
		ss.str(st);
		mystack.pop();
		T var1;//=atof
		ss >> var1;
		if(ss.fail()){
			bParsingError=true;
			failedSt=st;
		}
		else{
			successfullParses++;
		}
		return var1;
	}
    //return fn(std::forward<Args>(args)...);
public:
	//std::result_of<Fn(Args...)>::type// <- instead of void
	//typename std::result_of<Fn(Args...)>::type operator()(){
	typename std::result_of<Fn(Args...)>::type operator()(){
	//void operator()(){
		bParsingError=false;
		successfullParses=0;

		auto argTup=std::make_tuple(prvArgGetter<Args>()...);
		if(bParsingError){
			std::cout << "Failed while parsing \"" << failedSt << "\", at position " << successfullParses+1 << endl;
		}
		print(argTup);
		//call(fn, argTup);
		return apply_tuple(fn, argTup);
		//return ;
		//return fn(argGetter<Args>()...);
	}
	
	//parsing stuff;
private:
	static std::stringstream ss;
	static bool bParsingError;
	static unsigned int successfullParses;
	static std::string failedSt;
};
#define WRAPPER3(FUNC) wrapper3<decltype(&FUNC), &FUNC>

int min(int a, int b){
	cout << "Min got a:" << a << ", b: " << b << endl;
    return (a<b)?a:b;
}


template <class ArgT1, class RetT>
class ArduinoCommand {
	typedef RetT (*locFuncP)(ArgT1);
	locFuncP f; //Local pointer to the function to be called
public:
	ArduinoCommand(locFuncP f) : f(f){}

    void operator()() { 
	
		ArgT1 var1=atof(mystack.top());
		RetT retVal=(*f)(var1);
		cout << "retVal was: " << retVal <<endl;
		return;
	}
};

//Dummy function to test
int unchanged(int i) { return i; }


int main()
{
	mystack.push((char*)"227.0");
	mystack.push((char*)"84");

    //A < int, int, int(*)(int)> t( unchanged );
	//vPoint vp=(vPoint)&unchanged;
	//B <int, int, reinterpret_cast<vPoint>(&unchanged)> s;
	//B <int, int> s(&unchanged);
	//ArduinoCommand <int, int> ac(&unchanged);
	//std::cout << ac() << std::endl;
    //doOperation< int, int, unchanged> doOp;
	//doOp();

    //for (int i = 0; i < 10; i++)
     //   std::cout << t(i) << "\n";
  //doOperation<add1>();
  //doOperation<add1(int)>();
  //doOperation<add2(int)>();
	//decltype(&min) wrap1 =&wrapper<decltype(&min), &min>(min)

    //auto wrap1=WRAPPER(min);
    //wrap2=WRAPPER(rand)
    //std::cout<<WRAPPER(min)(10, 20)<<'\n';
    //std::cout<<wrapper2<decltype(&min), &min, int, int>()<<'\n';
	//auto tup=std::make_tuple(3,5);
	//std::cout << apply_tuple(min, tup) << std::endl;
	//func<decltype(min), min>();
    wrapper3<decltype(&min), &min, int, int> obj;
	std::cout << obj() << std::endl;
    //std::cout<<wrap2()<<'\n';
}
