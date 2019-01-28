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

// get array from table
std::vector<std::string> yap::toml_string::tarray(std::string filename, std::string keyname, std::string tablename) {
    // initialize file 'filename'
    auto config = cpptoml::parse_file(filename);
    
    // get array
    auto table = config->get_table(tablename);
    auto info = table->get_array_of<std::string>(keyname);
    
    return (*info);
}

// exec system commands - legacy
int yap::launcher(const char** command) {
    if (!fork()) {
        if(execvp(command[0], (char*const*)command) == -1)
            return -1;
    }
    wait(0);
    return 0;
}

// exec system commands
int yap::launcher(std::vector<std::string> commands) {
    char** args = vec_to_array(commands);
    if (!fork()) {
        if(execvp(commands[0].c_str(), args) == -1){
            free_array(args);
            return -1;
        }
    }
    wait(0);
    free_array(args);
    return 0;
}

// download
void yap::download(std::string url, std::string name){
    std::vector<std::string> args = {
        "curl",
        url,
        "-o",
        name,
        "-L",
        "-s"
    };

    if (yap::launcher(args) == -1) {
        std::cerr << "Download failed!\nerrno: " << errno << "\n";
        exit(-1);    
    }
}

// extract
void yap::extract(std::string name, std::string file) {
    fs::create_directory(file);
    std::vector<std::string> args = {
        "tar",
        "-x",
        "-f",
        name,
        "-C",
        file,
        "--strip-components",
        "1"
    };
    
    if (yap::launcher(args) == -1) {
        std::cerr << "Extract falied!\nerrno: " << errno << std::endl;
        exit(-1);
    }
}

// make
void yap::make(std::vector<std::string> commands) {
    std::vector<std::string> args;
    for (auto command : commands) {
        args = split(command);
        if (yap::launcher(args) == -1) {
            std::cerr << "Compile failed!\nerrno: " << errno << std::endl;
            exit(-1);
        }
    }
}

// compile process
void yap::compile(std::string s_link, std::string name, std::vector<std::string> install, std::vector<std::string> make) {
    // variables
    std::string path = "USR/yap/src";
    chdir(path.c_str());
    
    std::regex REG(".tar.gz");
    std::string file = std::regex_replace(name, REG, ""); // file name without .tar.gz
    
    std::string PREFIX = "../../..";
    
    // download source
    std::cout << "Downloading..." << std::endl;
    yap::download(s_link, name);
    std::cout << "Finished Downloading" << std::endl;
 
    // extract source   
    std::cout << "Extracting..." << std::endl;
    yap::extract(name, file);
    fs::remove(name);
    std::cout << "Finished Extracting" << std::endl;
    
    chdir(file.c_str());
    // pre compile process
    if(fs::exists("config.mk")) {
        // get PREFIX
        std::ifstream fin;
        fin.open("config.mk");
        
        std::string data;
        std::stringstream prefix_file;
        while (getline(fin, data))
            prefix_file << data << std::endl;
        
        fin.close();
        
        // replace PREFIX
        std::ofstream fout;
        fout.open("config.mk");
        
        std::regex REG("\nPREFIX.*");
        std::string new_prefix_file = std::regex_replace(prefix_file.str(), REG, "\nPREFIX = "+PREFIX);
        
        fout << new_prefix_file;
        fout.close();
    }

    // make
    yap::make(make);
}


char** vec_to_array(std::vector<std::string> vec){
    char** a = new char*[vec.size()];
    for (int i = 0; i < vec.size(); ++i){
        a[i] = new char[vec[i].size()];
        strcpy(a[i], vec[i].c_str());
    }
    a[vec.size()] = NULL;
    return a;
}

void free_array(char** arr){
    int i = 0;
    while(arr[i] != NULL)
        delete [] arr[i++];
    delete [] arr;
}


std::vector<std::string> split(std::string string, std::string delimiter){
    std::vector<std::string> buffer;
    size_t x;

    while((x = string.find(delimiter)) != -1){
        buffer.push_back(string.substr(0,x));
        string.erase(0, x+1);
    }

    buffer.push_back(string);
    
    return buffer;
}

