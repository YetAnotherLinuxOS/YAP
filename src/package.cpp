#include "../include/package.h"

std::string yap::Package::get_info(){
    std::stringstream info;
    info << "Package: " << name << "-" << version << std::endl;
    
    info << "\nName          :\t" << name;
    info << "\nVersion       :\t" << version;
    info << "\nDescription   :\t" << description;
    info << "\nManpage       :\t" << manpage;
    info << "\nLicense       :\t" << license;
    
    info << "\n\nDependencies\n";
    for (auto dep : dependecies)
        info << "\t" << dep << "\n";

    return info.str(); 
}

yap::Package::Package(std::string path){
    // if more than 1 pkgs is found; display a list with it and exit
    std::vector<std::string> pkgs = yap::getfile(path);
    if (pkgs.size() > 1) {
        for (const auto& pkg : pkgs) std::cout << pkg << std::endl;
        exit(-1); // maybe exit (-1)?
    } else if (pkgs.size() == 0) {
        std::cout << "yap: no package found" << std::endl;
        exit(-1); // maybe exit (-1)?
    }

    // get strings from 'info' table in file.toml 
    name        = toml_string::table(pkgs[0], "name"       , "info");
    version     = toml_string::table(pkgs[0], "version"    , "info");
    description = toml_string::table(pkgs[0], "description", "info");
    manpage     = toml_string::table(pkgs[0], "manpage"    , "info");
    license     = toml_string::table(pkgs[0], "license"    , "info");
    source_link = toml_string::table(pkgs[0], "source"     , "build");

    // get array from 'dependencies' key in file.toml
    dependecies = toml_string::array(pkgs[0], "dependencies");
}

void yap::Package::Download(){
    yap::Download(source_link, name+version+".tar.gz");
}

void yap::Package::Compile() {
    yap::Compile(source_link, name+version+".tar.gz");
}

void yap::Package::Uninstall() {
    yap::Uninstall(name+version);
}
