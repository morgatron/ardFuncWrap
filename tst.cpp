#include <string>
#include <iostream>
#include <stack>

unsigned int split(const std::string &txt, std::stack<std::string> &strs, char ch)
{
    unsigned int pos = txt.find( ch );
    unsigned int initialPos = 0;
    //strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push( txt.substr( initialPos, pos - initialPos + 1 ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

using namespace std;
int main(){
	string st="17.5 14.2";
	stack<string> stck;
	split(st, stck, ' ');
	cout <<stck.top() << endl;
	stck.pop();
	cout << stck.top() << endl;
	return 0;
}
