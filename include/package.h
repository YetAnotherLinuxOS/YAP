#ifndef PACKAGE
#define PACKAGE

#include <string>
#include "cpptoml.h"
#include "yap.h"

namespace yap {
    class Package {
    private:
        std::string name;
        std::string version;
        std::string license;
        std::string manpage;
        std::string description;
        std::string source_link;
        std::vector<std::string> dependecies;
        std::vector<std::string> compile_make;
        std::vector<std::string> compile_install;
        std::vector<std::string> compile_uninstall;
        std::vector<std::string> patches;
    public:
        std::string get_info();
        Package(std::string);
        void Download();
        void Compile();
    };
}

#endif // PACKAGE
