#ifndef YAP_FUNCTIONS
#define YAP_FUNCTIONS

#include <iostream>
#include <regex>
#include <experimental/filesystem>
#include "cpptoml.h"

namespace fs = std::experimental::filesystem;

std::vector<std::string> getfile(std::string rex);
void info(std::string name);

#endif
