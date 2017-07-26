#include "initFromString.h"


unsigned int split(const std::string &txt, std::stack<std::string> &strs, char ch)
{
    unsigned int pos = txt.find( ch );
    unsigned long initialPos = 0;
    //strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push( txt.substr( initialPos, pos - initialPos + 1 ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
	unsigned int txtSize= txt.size();
    strs.push( txt.substr( initialPos, std::min( (unsigned int)pos, (unsigned int)txt.size() ) - initialPos + 1 ) );

    return strs.size();
}
