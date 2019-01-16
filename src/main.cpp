#include "../include/cpptoml.h"
#include <iostream>

using namespace std;

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

// function that read a file.toml and return info
// actually set to 'dzen-0.9.5_svn.toml'; temporarily
void info(){
    // initialize file.toml
    auto config = cpptoml::parse_file("dzen-0.9.5_svn.toml"); // temporarily

    // get info from [info] table in file.toml
    auto info = config->get_table("info");
    auto p_name = info->get_as<string>("name");
    auto p_ver = info->get_as<string>("version");
    auto p_desc = info->get_as<string>("description");
    auto p_man = info->get_as<string>("manpage");
    auto p_lis = info->get_as<string>("license");

    // get array from 'dependences' key in file.toml
    auto deps = config->get_array_of<string>("dependences");

    // output info from file.toml
    string infout = "Info About: " + (*p_name) +"-"+ (*p_ver) + "\n\n"
                    "name         :\t" + (*p_name) + "\n"
                    "version      :\t" + (*p_ver)  + "\n"
                    "description  :\t" + (*p_desc) + "\n"
                    "manpage      :\t" + (*p_man)  + "\n"
                    "license      :\t" + (*p_lis)  + "\n\n";

    cout << infout << endl;
    
    // output dependence info from file.toml
    cout << "Dependences:\n" << endl;
    for (const auto& dep : *deps) cout << "\t" << dep << endl;
}

// main function '-
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
        info();
    } else {
        cout << "yap: no such option: '" << argv[1] << "'" << endl;
    }

    return 0;
}
