#include "../include/package.h"

std::string yap::Package::get_info() {
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

yap::Package::Package(std::string path) {
  // if more than 1 pkgs is found; display a list with it and exit
  std::vector<std::string> pkgs = yap::getfile(path);
  if (pkgs.size() > 1) {
    for (const auto &pkg : pkgs)
      std::cout << pkg << std::endl;
    exit(-1); // maybe exit (-1)?
  } else if (pkgs.size() == 0) {
    std::cout << "yap: no package found" << std::endl;
    exit(-1); // maybe exit (-1)?
  }

  // get strings from 'info' table in file.toml
  name = toml_string::table(pkgs[0], "name", "info");
  version = toml_string::table(pkgs[0], "version", "info");
  description = toml_string::table(pkgs[0], "description", "info");
  manpage = toml_string::table(pkgs[0], "manpage", "info");
  license = toml_string::table(pkgs[0], "license", "info");
  source_link = toml_string::table(pkgs[0], "source", "build");
  compression_format =
      toml_string::table(pkgs[0], "compression_format", "build");

  // get array from 'dependencies' key in file.toml
  dependecies = toml_string::array(pkgs[0], "dependencies");

  // get array from table 'build'
  patches = toml_string::tarray(pkgs[0], "patches", "build");
  compile_make = toml_string::tarray(pkgs[0], "make", "build");
  compile_install = toml_string::tarray(pkgs[0], "install", "build");
  compile_uninstall = toml_string::tarray(pkgs[0], "uninstall", "build");
}

std::string yap::Package::GetNameVer() { return name + version; }

void yap::Package::Download() {
  yap::download(source_link, name + version + compression_format);
}

void yap::Package::Compile() { yap::compile(compile_make, patches); }

void yap::Package::Extract() {
  yap::extract(name + version + compression_format, compression_format, name + version);
}

bool yap::Package::IsInstalled() { return installed; }

void yap::Package::PreCompile() { /* TODO: Implement pre compile options based
                                     on ybh */
}

void yap::Package::Uninstall() {
  yap::uninstall(compile_uninstall, name + version);
}
