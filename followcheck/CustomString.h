#pragma once
#include <iostream>
#include <string>

class CustomString {

public:

    std::string content;

    CustomString();
    CustomString(std::string init);

    int hash();

    std::size_t findNext(std::string search); //consecutive searching
    void resetSearchPos(); //reset search position to beginning of string

    std::string section(unsigned int firstCharPos, char endChar); //write requested section of a string to a new string, section ends at sepcific character
    std::string section(unsigned int firstCharPos, unsigned int endCharPos); //section ends at given integer position

private:

    std::string searchContent;
    unsigned int searchPosition;
};