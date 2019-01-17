#include "../include/functions.h"

/************
 *
 * package manager must do:
 *      
 *      download tarball in due directory
 *      unpack tarball in due directory
 *      make it
 *      install it
 *
 ************/

int main(int argc, char* argv[]) {
    // test if the first arg is NULL or not
    string option = argv[1] != NULL ? string(argv[1]) : "";

    // help menssage
    string help = "Usage: yap [option] [arg]\n\n"
                  "-h/--help        print this menssage and leave\n"
                  "-s/--search      search for a program\n"
                  "-i/--install     install a program\n"
                  "-q/--info        get information about a program\n";

    // get options and do events based on it
    if (option == "-h" || option == "--help" || option.empty()) {
        cout << help << endl;
    } else if (option == "-q" || option == "--info") {
        if (argv[2] == NULL) {
            cout << "yap: no package option" << endl;
            return 2;
        }
        info(argv[2]);
    } else {
        cout << "yap: no such option: '" << argv[1] << "'" << endl;
    }

    return 0;
}
