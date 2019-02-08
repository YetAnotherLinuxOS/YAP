#include "../include/package.h"

std::string yap::Package::get_info() {
  std::stringstream info;
  info << "Package: " << name << "-" << version << std::endl;

  auto print_map = [&info](std::map<std::string, std::string> map) {
    auto it = map.begin();
    while (it != map.end()) {
      info << "\tkey: " << it->first << "\n\tvalue: " << it->second << "\n\n";
      ++it;
    }
  };

  info << "\nName          :\t" << name;
  info << "\nVersion       :\t" << version;
  info << "\nDescription   :\t" << description;
  info << "\nManpage       :\t" << manpage;
  info << "\nLicense       :\t" << license;

  if (!dependencies.empty()) {
    info << "\n\nDependencies\n";
    print_map(dependencies);
  }

  if (!features.empty()) {
    info << "\n\nFeatures\n";
    print_map(features);
  }

  return info.str();
}

yap::Package::Package(std::string pkg) {
  if (pkg.empty()) {
    std::cout << "yap: no package option" << std::endl;
    exit(-1);
  }

  // get the newest package version
  std::string ybh = "test/" + pkg + "/" + yap::get_ybh(pkg);

  // get strings from 'info' table in file.toml
  name = toml_string::table(ybh, "name", "info");
  version = toml_string::table(ybh, "version", "info");
  description = toml_string::table(ybh, "description", "info");
  manpage = toml_string::table(ybh, "manpage", "info");
  license = toml_string::table(ybh, "license", "info");
  source_link = toml_string::table(ybh, "source", "build");
  compression_format = toml_string::table(ybh, "compression_format", "build");

  // get array from 'dependencies' key in file.toml
  // dependecies = toml_string::array(ybh, "dependencies");
  dependencies = toml_string::map_table(ybh, "dependencies");
  features = toml_string::map_table(ybh, "features");

  // get array from table 'build'
  patches = toml_string::tarray(ybh, "patches", "build");
  compile_make = toml_string::tarray(ybh, "make", "build");
  compile_install = toml_string::tarray(ybh, "install", "build");
  compile_uninstall = toml_string::tarray(ybh, "uninstall", "build");
}

std::string yap::Package::GetNameVer() { return name + version; }

void yap::Package::Download() {
  yap::download(source_link, name + version + compression_format);
}

void yap::Package::Compile() { yap::compile(compile_make, patches); }

void yap::Package::Extract() {
  yap::extract(name + version + compression_format, compression_format,
               name + version);
}

bool yap::Package::IsInstalled() { return installed; }

void yap::Package::PreCompile() { /* TODO: Implement pre compile options based
                                     on ybh */
}

void yap::Package::Uninstall() {
  yap::uninstall(compile_uninstall, name + version);
}

std::vector<std::string> search(std:: string reg) {
    std::regex REX(reg + ".*");
    std::smatch m;
    
    std::vector<std::string> all, latests, matches;
    
    fs::path path = "test/";
    for (auto& sub_path : fs::directory_iterator(path)) {
        for (auto& file : fs::directory_iterator(sub_path)) {
            if (fs::is_regular_file(file))
               all.push_back((std::string) sub_path.path().filename() + "/" + (std::string) file.path().filename());
        }
        latests.push_back(std::max_element(all.begin(), all.end())->c_str());
        all.erase(all.begin(), all.end());
    }
    
    for (auto& elem : latests) {
        if(std::regex_match(elem, m, REX))
            matches.push_back(m[0]);
    }

    return matches;
}
