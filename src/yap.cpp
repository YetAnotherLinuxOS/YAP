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

// exec system commands
int yap::launcher(const char *command[]) {
    if (!fork()) {
        if(execvp(command[0], (char*const*)command) == -1)
            return -1;
    }
    wait(0);
    return 0;
}

// download
void yap::Download(std::string url, std::string name){
    std::string file = "-o"+name;
    const char* args[] = {
        "curl",
        url.c_str(),
        file.c_str(),
        "-s",
        "-L",
        NULL
    };

    if (yap::launcher(args) == -1) {
        std::cerr << "Download failed!\nerrno: " << errno << "\n";
        exit(-1);    
    }
}

// extract
void yap::Extract(std::string name, std::string file) {
    fs::create_directory(file);
    const char *args[] = {
        "tar",
        "-x",
        "-f",
        name.c_str(),
        "-C",
        file.c_str(),
        "--strip-components",
        "1",
        NULL
    };
    
    if (yap::launcher(args) == -1) {
        std::cerr << "Extract falied!\nerrno: " << errno << std::endl;
        exit(-1);
    }
}

// make
void yap::Make(std::string path) {
    const char *args[] = {
        "make",
        "-C",
        path.c_str(),
        NULL
    };
    
    if (yap::launcher(args) == -1) {
        std::cerr << "Compilation falied!\nerrno: " << errno << std::endl;
        exit(-1);
    }
}

// install
void yap::Install(std::string path) {
    const char *args[] = {
        "make",
        "install",
        "clean",
        "-C",
        path.c_str(),
        NULL
    };
    
    if (yap::launcher(args) == -1) {
        std::cerr << "Install falied!\nerrno: " << errno << std::endl;
        exit(-1);
    }
}

// uninstall
void yap::Uninstall(std::string name) {
    // variables
    std::string path = "USR/yap/src";
    chdir(path.c_str());
    
    const char *args[] = {
        "make",
        "uninstall",
        "clean",
        "-C",
        name.c_str(),
        NULL
    };
    
    if (yap::launcher(args) == -1) {
        std::cerr << "Uninstall falied!\nerrno: " << errno << std::endl;
        exit(-1);
    }
}

// compile process
void yap::Compile(std::string sourceLink, std::string name) {
    // variables
    std::string path = "USR/yap/src";
    chdir(path.c_str());
    
    std::regex REG(".tar.gz");
    std::string file = std::regex_replace(name, REG, ""); // file name without .tar.gz
    
    std::string PREFIX = "../../..";
    std::string PREFIX_FILE = file+"/config.mk";
    
    // download source
    std::cout << "Downloading..." << std::endl;
    yap::Download(sourceLink, name);
    std::cout << "Finished Downloading" << std::endl;
 
    // extract source   
    std::cout << "Extracting..." << std::endl;
    yap::Extract(name, file);
    fs::remove(name);
    std::cout << "Finished Extracting" << std::endl;
    
    // pre compile process
    if(fs::exists(PREFIX_FILE)) {
        // get PREFIX
        std::ifstream fin;
        fin.open(PREFIX_FILE);
        
        std::string data;
        std::stringstream prefix_file;
        while (getline(fin, data))
            prefix_file << data << std::endl;
        
        fin.close();
        
        // replace PREFIX
        std::ofstream fout;
        fout.open(PREFIX_FILE);
        
        std::regex REGF("\nPREFIX.*");
        std::string new_prefix_file = std::regex_replace(prefix_file.str(), REGF, "\nPREFIX = "+PREFIX);
        
        fout << new_prefix_file;
        fout.close();
    }
    
    // compile source
    std::cout << "Compiling..." << std::endl;
    Make(file);
    std::cout << "Finished Compiling" << std::endl;
    
    // install source
    std::cout << "Installing..." << std::endl;
    Install(file);
    std::cout << "Finished Installing" << std::endl;
}
