#ifndef YAP_H
#define YAP_H

#include "cpptoml.h"
#include <cstdlib>
#include <errno.h>
#include <experimental/filesystem>
#include <iostream>
#include <map>
#include <regex>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::experimental::filesystem;

char **vec_to_array(std::vector<std::string>);
void free_array(char **);
std::vector<std::string> split(std::string, std::string = " ");

namespace yap {
// utils
std::vector<std::string> getfile(std::string);
std::string get_ybh(std::string);

int launcher(std::vector<std::string>);
int launcher(const char **command); /* only for legacy purposes */

void run(std::vector<std::string>, std::string);

// build
void download(std::string, std::string);
void extract(std::string, std::string, std::string);
void compile(std::vector<std::string>, std::vector<std::string>);
void uninstall(std::vector<std::string>, std::string);
void apply_patches(std::vector<std::string>);
} // namespace yap

#endif
