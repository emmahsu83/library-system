#pragma once
#include <string>

class Book
{
public:
    std::string title;
    std::string author;
    std::string isbn;
    int nocAvailable;
    int nocCheckOut;
};