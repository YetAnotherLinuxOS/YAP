#ifndef PACKAGE
#define PACKAGE

#include <string>
#include "cpptoml.h"
#include "yap.h"

/*
 *
 * TODO: Download package
 * probably using curl
*/

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
    public:
        std::string get_info();
        Package(std::string);
        void Download();
    };
}

#endif // PACKAGE
