#include "../include/package.h"

std::string yap::Package::get_info(){
    std::stringstream info;
    info << "\nName: " << name;
    info << "\nVersion: " << version;
    info << "\nDescription: " << description;
    info << "\nSource: " << source_link; 
    info << "\nDependencies:\n";
    for (auto dep : dependecies)
        info << "\t" << dep << "\n";

    return info.str(); 
}

yap::Package::Package(std::string path){
    // if more than 1 pkgs is found; display a list with it and exit
    std::vector<std::string> pkgs = yap::getfile(path);
    if (pkgs.size() > 1) {
        for (const auto& pkg : pkgs) std::cout << pkg << std::endl;
        return;
    } else if (pkgs.size() == 0) {
        std::cout << "yap: no package found" << std::endl;
        return;
    }

    // get strings from 'info' table in file.toml 
    name        = toml_string::table(pkgs[0], "name"       , "info");
    version     = toml_string::table(pkgs[0], "version"    , "info");
    description = toml_string::table(pkgs[0], "description", "info");
    manpage     = toml_string::table(pkgs[0], "manpage"    , "info");
    license     = toml_string::table(pkgs[0], "license"    , "info");
    source_link = toml_string::table(pkgs[0], "source"     , "build");

    // get array from 'dependences' key in file.toml
    dependecies = toml_string::array(pkgs[0], "dependences");
}

void yap::Package::Download(){

}

void yap::Package::Download(std::string){

}