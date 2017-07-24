#include <tuple>

template<typename T>
T argGetter(){
	T outVal;
	//Get the variable by parsing string data from external source, store the conversion in outVal
	return outVal;
}

template<typename Fn, Fn fn, typename... Args>
class commandWrap {

public:
	void operator()(){
		auto argTuple=std::make_tuple(argGetter<Args>()...);
		// Check for parsing errors etc...
		// Call the wrapped function using the obtained arguments:
		typename std::result_of<Fn(Args...)>::type result;
		//result= apply_tuple(fn, argTup);

		//Stringify the return value and send back to external source
		//retValueSend(result)
	}
};

int f1(int a, int b){
    return (a<b)?a:b;
}
double f2(double a){
    return 2*a;
}
int main(){
	// cmd1 and cmd2 should be callable objects
    commandWrap<decltype(&f1), &f1, int, int> cmd1;
    commandWrap<decltype(&f2), &f2, double> cmd2;
	//addCommand(cmd1);
	//addCommand(cmd2);
	//
	// Better would be:
    //commandWrap<decltype(&f1), &f2> cmd1; // <- doesn't compile
    //commandWrap<decltype(&f2), &f2> cmd2;
	// 
	//
	//
	// Best would be:
    //commandWrap<&f1> cmd1; // <- definitely doesn't compile
	return 0;
}
        
