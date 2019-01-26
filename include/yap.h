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
    
    // int launcher(const char _*command[]_);
    int launcher(const char **);
    
    // void Download(string _sourceLink_, string _name_);
    void Download(std::string, std::string);
    
    // void Extract(string _filename.tar.gz_);
    void Extract(std::string, std::string);
    
    // void Make(string _path_);
    void Make(std::string);
    
    // void Install(string _path_);
    void Install(std::string);
    
    // void Uninstall(string _path_);
    void Uninstall(std::string);
    
    // void Compile(string _sourceLink_, string _name_);
    void Compile(std::string, std::string);
    
    namespace toml_string {
        // string table(string _filename_, string _keyname_, string _tablename_);
        std::string table(std::string, std::string, std::string);
        
        // vector<string> array(string _filename_, string _keyname_);
        std::vector<std::string> array(std::string, std::string);
    }
}

#endif
