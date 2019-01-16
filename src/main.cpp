#include "../include/cpptoml.h"
#include <experimental/filesystem>
#include <regex>
#include <iostream>

using namespace std;
namespace fs = std::experimental::filesystem;

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

// function that search for REGEX filename in directory
vector<string> getfile(string rex) {
    fs::path ybh_path = fs::current_path(); // directory to read

    // regex; variable rex plus regex /.*/
    // smatch; used in 'regex_match' to list /matches/
    regex reg(rex+".*");
    smatch matches;

    // create a string vector and list the files name in directory
    vector<string> ybhs;
    for (const auto& dirname : fs::directory_iterator(ybh_path)) {
        if (fs::is_regular_file(dirname)) ybhs.push_back(fs::path(dirname).filename());
    }

    // create string vector with the matches
    vector<string> returning;
    for (const auto& ybh : ybhs) {
        if (regex_match(ybh, matches, reg)) returning.push_back(matches[0]);
    }

    return returning;
}

// function that read a file.toml and return info
void info(string name) {
    // if more than 1 pkgs is found; return a list with it
    vector<string> pkgs = getfile(name);
    if (pkgs.size() > 1) {
        for (const auto& pkg : pkgs) cout << pkg << endl;
        return;
    } else if (pkgs.size() == 0) {
        cout << "yap: no package found" << endl;
        return;
    }

    // initialize file.toml
    auto config = cpptoml::parse_file(pkgs[0]);

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
                    "license      :\t" + (*p_lis)  + "\n";

    cout << infout << endl;
    
    // output dependence info from file.toml
    cout << "Dependences:\n" << endl;
    for (const auto& dep : *deps) cout << "\t" << dep << endl;
    cout << endl;
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
