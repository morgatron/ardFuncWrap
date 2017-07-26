#ifndef __INC_INITFROMSTRING_H
#define __INC_INITFROMSTRING_H
#include <string>
#include <stack>
//#include "from_string.h"
unsigned int split(const std::string &txt, std::stack<std::string> &strs, char ch);
//

template<typename T>
int fromString(const std::string& s, T& val){
	val=T(s);
	return 0;
	//return boost::lexical_cast<T>(s);
}

template<>
int fromString(const std::string& s, unsigned long & val){
	char *finish;
	const char *start=s.c_str();
	val=strtoul(start, &finish, 10);
	if(finish!=start + s.size()){
		if(finish == start)
			return -1;
		else
			return -2;
	}
	else
		return 0;
}
template<>
int fromString(const std::string& s, int& val){
	char *finish;
	const char *start=s.c_str();
	val=strtol(start, &finish, 10);
	if(finish!=start + s.size()){
		if(finish == start)
			return -1;
		else
			return -2;
	}
	else
		return 0;
}

template<>
int fromString(const std::string& s, float& val){
	char *finish;
	const char *start=s.c_str();
	val=strtof(start, &finish);
	if(finish!=start + s.size()){
		if(finish == start)
			return -1;
		else
			return -2;
	}
	else
		return 0;
}

template<>
int fromString(const std::string& s, double& val){
	char *finish;
	const char *start=s.c_str();
	val=strtod(start, &finish);
	if(finish!=start + s.size()){
		if(finish == start)
			return -1;
		else
			return -2;
	}
	else
		return 0;
}
using namespace std;
template <typename T>
int initFromString(std::string st, T& val){
	std::istringstream ss(st);
	ss >> val;
	auto errVal=ss.fail();
	cout << "parsing " << st << ", err val is " << errVal <<endl;
	return errVal;
	//return fromString<T>(st, val);
}
//
// Initialize an array from a string- tricky! WIP. Need more template parameters here.
/*
template <std::size_t SIZE>
int initFromString<std::array<float, SIZE>>(std::string txt, std::array<float, SIZE> &val){
	//assume st is of the form 0,1,2,3
	char ch=',';
    unsigned int pos = txt.find( ch );
    unsigned int initialPos = 0;
    //strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push( txt.substr( initialPos, pos - initialPos + 1 ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }
		
}
*/
#endif //INC
