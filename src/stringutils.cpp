/*
    String Utility Implementation File
    John "Jack" Riales, CSC230. J00515647
*/

#include "stringutils.h"
using namespace std;

// Conversion functions needed by the binary class
int stringToInt(std::string value)
{
    return atoi(value.c_str());
}

std::string intToString (int value)
{
    std::ostringstream convert;
    convert << value;
    return convert.str();
}

std::string toLowerCase(std::string str)
{
    std::string destinationString;

    // Allocate the destination space
    destinationString.resize(str.size());

    // Convert the source string to lower case
    // storing the result in destination string
    std::transform(str.begin(),
                    str.end(),
                    destinationString.begin(),
                    ::tolower);

    return destinationString;
}
