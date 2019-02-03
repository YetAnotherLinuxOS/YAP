#ifndef YAP_H
#define YAP_H

#include "cpptoml.h"
#include <cstdlib>
#include <errno.h>
#include <experimental/filesystem>
#include <iostream>
#include <regex>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::experimental::filesystem;

char **vec_to_array(std::vector<std::string>);
void free_array(char **);
std::vector<std::string> split(std::string, std::string = " ");

namespace yap {
// vector<string> getfile(string _regex_);
std::vector<std::string> getfile(std::string);

// string get_ybh(string package_name);
std::string get_ybh(std::string);

// int launcher(vector<string> _commands_);
int launcher(std::vector<std::string>);

// int launcher(const char _*command[]_);
int launcher(const char **command); /* only for legacy purposes */

// void download(string _sourceLink_, string _name_);
void download(std::string, std::string);

// void extract(string _filename.tar.gz_);
void extract(std::string, std::string, std::string);

// void compile(string _sourceLink_, string _name_);
void compile(std::vector<std::string>, std::vector<std::string>);

// void run(vector<string> _commands_, string _name_);
void run(std::vector<std::string>, std::string);

// void uninstall(vector<string> _command_, string _pkg_);
void uninstall(std::vector<std::string>, std::string);

// void apply_patches(vector<string> _patches_);
void apply_patches(std::vector<std::string>);

namespace toml_string {
// string table(string _filename_, string _keyname_, string _tablename_);
std::string table(std::string, std::string, std::string);

// vector<string> array(string _filename_, string _keyname_);
std::vector<std::string> array(std::string, std::string);

//  vector<string> tarray(string _filename_, string _keyname_, string
//  _tablename_);
std::vector<std::string> tarray(std::string, std::string, std::string);
} // namespace toml_string
} // namespace yap

#endif
