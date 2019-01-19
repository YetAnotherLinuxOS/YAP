#include "../include/yap.h"
#include <iostream>
#include "../include/package.h"

/*
 *
 * package manager must do:
 *      
 *      download tarball in due directory
 *      unpack tarball in due directory
 *      make it
 *      install it
 *
 */

int main(int argc, char* argv[]) {
    // test if the first arg is NULL or not
    std::string option = argv[1] != NULL ? std::string(argv[1]) : "";

    // help menssage
    std::stringstream help;
    help << "Usage: yap [option] [arg]\n\n";
    help << "-h/--help        print this menssage and leave\n";
    help << "-s/--search      search for a program\n";
    help << "-i/--install     install a program\n";
    help << "-q/--info        get information about a program\n";

    // get options and do events based on it
    if (option == "-h" || option == "--help" || option.empty()) {
        std::cout << help.str() << std::endl;
    } else if (option == "-q" || option == "--info") {
        if (argv[2] == NULL) {
            std::cout << "yap: no package option" << std::endl;
            return 2;
        }
        // yap::display_info(argv[2]);
        yap::Package pkg(argv[2]);
        std::string info = pkg.get_info();
        std::cout << info << "\n";
    } else {
        std::cout << "yap: no such option: '" << argv[1] << "'" << std::endl;
    }

    return 0;
}
