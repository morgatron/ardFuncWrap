
#include <iostream>
#include <stack>
#include "argwrap.h"
#include <functional>

float f=0;
void noadd(float f0, float f1){
	f=f0+f1;
}

float add(float f0, float f1) { return f0 + f1; }

struct adder {
    float value;
    adder(float value): value(value) {}
    float operator()(float f0, float f1) {
        return value + f0 + f1;
    }
};
struct foo {
    float value;
    foo(): value() {}
    explicit foo(float value): value(value) {}
    foo mem(foo f) { return foo(value + f.value); }
};

/*
std::istream& operator>> (std::istream& in, foo& f) {
    return in >> f.value;
}
std::ostream& operator<< (std::ostream& out, foo const& f) {
    return out << f.value;
}
*/


/*
//Template function with signature void(*)(std::string) that takes a wrapped functioon as template paremeter and simply calles it.
template <R(*T)(int)>
void wrapwrap(int stck){
	(*T)(stck);	
}

*/

using namespace std;
int main() {
    std::stack<std::string> args;
	adder myAdder(0.5);
	std::cout << myAdder(10., 5.) << std::endl;
	int num;
	initFromString<int>("5", num);
	std::cout << num << std::endl;
	std::cout << "myAdder: " << endl; 

	//auto wMyAdder= wrap<float(float,float)>(myAdder);
	auto wMyAdder= wrap<float(float,float)>(myAdder);
    args.push("17.5");
    args.push("42.25");
	std::cout << wMyAdder(args) << std::endl;

    auto wfun = wrap(&add);
	//auto wAdd= wrap(&myAdder);
    args.push("17.5");
    args.push("42.25");
	//std::string st="57.5 42.25";
    std::cout << "wfun result=" << wfun(args) << "\n";
	//void (*f)(std::stack<std::string>);
	std::function<std::string(std::stack<std::string>&)> f=wfun;
    args.push("17.5");
    args.push("42.25");
	std:: cout << f(args) << "\n";
	//auto f=std::bind(wfun);

	
	/*
    auto wobj = wrap<float(float, float)>(adder(3.125));
    args.push("17.5");
    args.push("42.25");
    std::cout << "wobj result=" << wobj(args) << "\n";

    auto wmem = wrap(&foo::mem);
    args.push("17.5");
    args.push("42.25");
    std::cout << "wmem result=" << wmem(args) << "\n";
	*/
}
