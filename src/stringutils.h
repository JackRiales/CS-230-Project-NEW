/*
    String Utility Header
    John "Jack" Riales, CSC230, J00515647

    File contains global functions associated with string manipulation.

    Dependencies:
        STL String
        String Stream
        Standard Lib
        Algorithm
*/

#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

int stringToInt(std::string value);
std::string intToString (int value);
std::string toLowerCase(std::string str);
