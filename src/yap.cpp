#include "../include/yap.h"

void yap::display_info(std::string name){
    // if more than 1 pkgs is found; display a list with it and exit
    std::vector<std::string> pkgs = yap::getfile(name);
    if (pkgs.size() > 1) {
        for (const auto& pkg : pkgs) std::cout << pkg << std::endl;
        return;
    } else if (pkgs.size() == 0) {
        std::cout << "yap: no package found" << std::endl;
        return;
    }

    // get strings from 'info' table in file.toml 
    std::string p_name = toml_string::table(pkgs[0], "name"       , "info");
    std::string p_ver  = toml_string::table(pkgs[0], "version"    , "info");
    std::string p_desc = toml_string::table(pkgs[0], "description", "info");
    std::string p_man  = toml_string::table(pkgs[0], "manpage"    , "info");
    std::string p_lis  = toml_string::table(pkgs[0], "license"    , "info");

    // get array from 'dependences' key in file.toml
    std::vector<std::string> deps = toml_string::array(pkgs[0], "dependences");
    
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

// function that search for REGEX filename in directory
std::vector<std::string> yap::getfile(std::string rex) {
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

// get info from keys
std::string yap::toml_string::table(std::string filename, std::string keyname, std::string tablename) {
    // initialize file 'filename'
    auto config = cpptoml::parse_file(filename);

    // get table and get key from table
    auto table = config->get_table(tablename);
    auto info = table->get_as<std::string>(keyname);

    return (*info);
}

// get info from arrays
std::vector<std::string> yap::toml_string::array(std::string filename, std::string keyname) {
    // initialize file 'filename'
    auto config = cpptoml::parse_file(filename);

    // get array
    auto info = config->get_array_of<std::string>(keyname);

    return (*info);
}
