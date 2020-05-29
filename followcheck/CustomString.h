#pragma once
#include <iostream>
#include <string>

using String = std::string;

class CustomString {

public:

    String content;

    CustomString();
    CustomString(String init);

    int hash();

    std::size_t findNext(String search); //consecutive searching
    void resetSearchPos(); //reset search position to beginning of string

    String section(unsigned int firstCharPos, char endChar); //write requested section of a string to a new string, section end at sepcific character
    String section(unsigned int firstCharPos, unsigned int endCharPos); //section ends at given integer position

private:

    String searchContent;
    unsigned int searchPosition;
};