#include <iostream>
#include "apply_tuple.h"

double add(int a, double b) { return a+b; }
std::tuple<int,double> some_tuple{2, 34.2};

int main()
{
  using namespace std;
  cout 
    << apply(add, 2, 34.2) << endl
    << apply_tuple(add, some_tuple) << endl
    << apply_tuple(add, std::make_tuple(2,34.7)) << endl
  ;
}
