#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <functional>
#include <cstddef>
using namespace std;

class Obj;

class Obj{

public:
	int a=0;

    void operator()(int &val) {
		cout << "Got: " << val << endl;
		this->a+=val;
        return;
    }
};


int main(){
	int i=6;
	Obj obj;
	obj(i);
	cout << obj.a << endl;
	function<void(int&)> f=obj;
	f(i);
	cout << obj.a << endl;
	return 0;
}
