#include "CustomString.h"

using String = std::string;

CustomString::CustomString() {
	content = "";
	searchPosition = 0;
}

CustomString::CustomString(String init) {
	content = init;
	searchPosition = 0;
}

int CustomString::hash() {

	int h = 0;
	for (unsigned int i = 0; i < content.length(); i++) {
		h = 31 * h + content.at(i);
	}
	return h;
}

std::size_t CustomString::findNext(String search) {

	std::size_t found;
	searchContent = this->section(searchPosition, (unsigned int) content.length()); //search only in section of content coming AFTER searchPosition
	found = searchContent.find(search);

	if (found != std::string::npos) {

		searchPosition = found +  (content.length() - searchContent.length()) + 1; //next search will start 1 character after last result position
		return found + (content.length() - searchContent.length());		//return integer position of search term in content (NOT position in searchContent)

	}
	return std::string::npos;
}

String CustomString::section(unsigned int firstCharPos, char endChar) { 

	String newString;
	unsigned int i = firstCharPos;

	do {
		newString.push_back(content.at(i));
		i++;
	} while (content.at(i) != endChar);

	return newString;
}

String CustomString::section(unsigned int firstCharPos, unsigned int endCharPos) {

	String newString;
	unsigned int i = firstCharPos;

	for (unsigned int i = firstCharPos; i < endCharPos; i++) {

		newString.push_back(content.at(i));
	}

	return newString;
}

void CustomString::resetSearchPos() {
	searchPosition = 0;
}