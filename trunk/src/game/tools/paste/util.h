#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>

using namespace std;

template<typename T>
string toString(const T& x)
{
    stringstream ss;
    ss << x;
    return ss.str();
}

#endif // UTIL_H
