#include "../include/functions.h"

// function that search for REGEX filename in directory
std::vector<std::string> getfile(std::string rex) {
    fs::path ybh_path = fs::current_path(); // directory to read

    // regex; variable rex plus regex /.*/
    // smatch; used in 'regex_match' to list /matches/
    std::regex reg(rex+".*");
    std::smatch matches;

    // create a string vector and list the files name in directory
    std::vector<std::string> ybhs;
    for (const auto& dirname : fs::directory_iterator(ybh_path)) {
        if (fs::is_regular_file(dirname)) ybhs.push_back(fs::path(dirname).filename());
    }

    // create string vector with the matches
    std::vector<std::string> returning;
    for (const auto& ybh : ybhs) {
        if (std::regex_match(ybh, matches, reg)) returning.push_back(matches[0]);
    }

    return returning;
}

// function that read a file.toml and return info
void info(std::string name) {
    // if more than 1 pkgs is found; return a list with it
    std::vector<std::string> pkgs = getfile(name);
    if (pkgs.size() > 1) {
        for (const auto& pkg : pkgs) std::cout << pkg << std::endl;
        return;
    } else if (pkgs.size() == 0) {
        std::cout << "yap: no package found" << std::endl;
        return;
    }

    // initialize file.toml
    auto config = cpptoml::parse_file(pkgs[0]);

    // get info from [info] table in file.toml
    auto info = config->get_table("info");
    auto p_name = info->get_as<std::string>("name");
    auto p_ver = info->get_as<std::string>("version");
    auto p_desc = info->get_as<std::string>("description");
    auto p_man = info->get_as<std::string>("manpage");
    auto p_lis = info->get_as<std::string>("license");

    // get array from 'dependences' key in file.toml
    auto deps = config->get_array_of<std::string>("dependences");

    // output info from file.toml
    std::string infout = "Info About: " + (*p_name) +"-"+ (*p_ver) + "\n\n"
                        "name         :\t" + (*p_name) + "\n"
                        "version      :\t" + (*p_ver)  + "\n"
                        "description  :\t" + (*p_desc) + "\n"
                        "manpage      :\t" + (*p_man)  + "\n"
                        "license      :\t" + (*p_lis)  + "\n";

    std::cout << infout << std::endl;
    
    // output dependence info from file.toml
    std::cout << "Dependences:\n" << std::endl;
    for (const auto& dep : *deps) std::cout << "\t" << dep << std::endl;
    std::cout << std::endl;
}
