#ifndef YAP_FUNCTIONS
#define YAP_FUNCTIONS

#include <iostream>
#include <regex>
#include <experimental/filesystem>
#include "cpptoml.h"

using namespace std;
namespace fs = std::experimental::filesystem;

vector<string> getfile(string rex);
void info(string name);

#endif
