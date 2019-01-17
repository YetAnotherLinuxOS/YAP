#include "../include/yap.h"

namespace yts = yap::toml_string;

// function that read a file.toml and return info
void info(std::string name) {
    // if more than 1 pkgs is found; return a list with it
    std::vector<std::string> pkgs = yap::getfile(name);
    if (pkgs.size() > 1) {
        for (const auto& pkg : pkgs) std::cout << pkg << std::endl;
        return;
    } else if (pkgs.size() == 0) {
        std::cout << "yap: no package found" << std::endl;
        return;
    }

    // get strings from 'info' table in file.toml 
    std::string p_name = yts::table(pkgs[0], "name"       , "info");
    std::string p_ver  = yts::table(pkgs[0], "version"    , "info");
    std::string p_desc = yts::table(pkgs[0], "description", "info");
    std::string p_man  = yts::table(pkgs[0], "manpage"    , "info");
    std::string p_lis  = yts::table(pkgs[0], "license"    , "info");

    // get array from 'dependences' key in file.toml
    std::vector<std::string> deps = yts::array(pkgs[0], "dependences");
    
    // output info from file.toml
    std::string infout = "Info About: "     + p_name + "-" + p_ver + "\n\n"
                         "name         :\t" + p_name + "\n"
                         "version      :\t" + p_ver  + "\n"
                         "description  :\t" + p_desc + "\n"
                         "manpage      :\t" + p_man  + "\n"
                         "license      :\t" + p_lis  + "\n";

    std::cout << infout << std::endl;
    
    // output dependence info from file.toml
    std::cout << "Dependences:\n" << std::endl;
    for (const auto& dep : deps) std::cout << "\t" << dep << std::endl;
    std::cout << std::endl;
}
