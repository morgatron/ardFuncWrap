#ifndef __INC_INITFROMSTRING_H
#define __INC_INITFROMSTRING_H
unsigned int split(const std::string &txt, std::stack<std::string> &strs, char ch);
//
template <typename T>
int initFromString(std::string st, T& val){
	return std::istringstream(st) >> val;
}
// Initialize an array from a string- tricky! Need more template parameters here.

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
#endif //INC
