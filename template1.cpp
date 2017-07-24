//#include "libs/mpl/book/chapter1/binary.hpp"
#include "boost/utility/binary.hpp"
#include <iostream>
//using namespace boost;
int main()
{
 std::cout << utility::binary<101010>::value << std::endl;
 return 0;
}
