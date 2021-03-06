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

namespace yap {
    // vector<string> getfile(string _regex_);
    std::vector<std::string> getfile(std::string);
    
    // void Download(string _sourceLink_, string _name_);
    void Download(std::string, std::string);
    
    // int launcher(const char _*command[]_);
    int launcher(const char **);
    
    namespace toml_string {
        // string table(string _filename_, string _keyname_, string _tablename_);
        std::string table(std::string, std::string, std::string);
        
        // vector<string> array(string _filename_, string _keyname_);
        std::vector<std::string> array(std::string, std::string);
    }
}

#endif
