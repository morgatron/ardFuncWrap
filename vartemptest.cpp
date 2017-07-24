#include <iostream>
#include <stack>

using namespace std;

stack<char*> mystack;

template<typename (*T)(int)>
void wrapper(T func){
	int var1=atof(mystack.top());
	//mystack.pop();
    cout << func(var1);
}

//template<typename ftype, typename... ArgTypes>
//void wrapper(void(ArgTypes... args){
//
//    wrapper(args...);
//}

template<typename T>
bool pair_comparer(T a, T b) {
  return a == b;
}

template<typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args) {
  return a == b && pair_comparer(args...);
}

void func(int num){
    cout << __PRETTY_FUNCTION__ << endl;
	cout << num << endl;
    return 1;
}

int main(){
	mystack.push((char*)"1.50");
	mystack.push("2.5");
	//cout << mystack.pop() << endl;
	//cout << mystack.pop() << endl;
    cout << wrapper<func>(func) << endl;
    //cout << __PRETTY_FUNCTION__ << endl;
    //int a=3,b=5;
    //float f1=1.5,f2=1.7;
    //cout << pair_comparer(f1, f2, a, b) << endl;

    return 0;
}
