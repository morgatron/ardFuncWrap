 #include <iostream>
 template<typename T>
 struct identity
 {
     using type = T;
 };
using std::cout;
using std::endl;
 int main(){
	using t = typename identity<typename identity<int>::type>::type;
	t num=5.5;
	cout << num << endl;	
	return 0;
 }
