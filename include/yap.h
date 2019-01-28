#ifndef YAP_H
#define YAP_H

#include "cpptoml.h"
#include <regex>
#include <iostream>
#include <experimental/filesystem>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

namespace fs = std::experimental::filesystem;

char** vec_to_array(std::vector<std::string>);
void free_array(char**);
std::vector<std::string> split(std::string, std::string = " ");

namespace yap {
    // vector<string> getfile(string _regex_);
    std::vector<std::string> getfile(std::string);
    
    // int launcher(vector<string> _commands_);
    int launcher(std::vector<std::string>);

    // int launcher(const char _*command[]_);
    int launcher(const char** command); /* only for legacy purposes */
    
    // void Download(string _sourceLink_, string _name_);
    void download(std::string, std::string);
    
    // void Extract(string _filename.tar.gz_);
    void extract(std::string, std::string);
    
    // void Compile(string _sourceLink_, string _name_);
    void compile(std::string, std::string, std::vector<std::string>, std::vector<std::string>);

    // void Make(vector<string> _commands_);
    void make(std::vector<std::string>);
    
    namespace toml_string {
        // string table(string _filename_, string _keyname_, string _tablename_);
        std::string table(std::string, std::string, std::string);
        
        // vector<string> array(string _filename_, string _keyname_);
        std::vector<std::string> array(std::string, std::string);
        
        //  vector<string> tarray(string _filename_, string _keyname_, string _tablename_);
        std::vector<std::string> tarray(std::string, std::string, std::string);
    }
}

#endif
