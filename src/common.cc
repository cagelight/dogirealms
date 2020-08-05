#include "common.hh"

#include <iostream>

void DOGI::print(std::string_view msg, LogLevel) {
	std::cout << msg << std::endl;
}
