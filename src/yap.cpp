#include "../include/yap.h"

// function that search for REGEX filename in directory
std::vector<std::string> yap::getfile(std::string rex) {
    fs::path ybh_path = "test/"; // directory to read; '/' on the end IS necessary

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
    
    // if returning have only one element
    // insert ybh_path to the begin of the string
    if (returning.size() == 1) {
        returning.at(0).insert(0, std::string(ybh_path));
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


void yap::Download(std::string url, std::string name){
    std::string file = "-o"+name;
    if (!fork()) {
        std::cout << "Downloading " + name << "\n";
        execlp("curl", "curl", url.c_str(), file.c_str(), "-s");
        exit(0);
    }
    wait(0);
    /* TODO: Download may fail, check for errors */
    std::cout << "Finished downloading " + name << "\n";
}
