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

yap::Package::Package(std::string pkg) {
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
  dependecies = toml_string::array(ybh, "dependencies");

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
