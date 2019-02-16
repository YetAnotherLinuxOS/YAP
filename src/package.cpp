#include "../include/package.h"

std::string yap::Package::get_info() {
  std::stringstream info;
  info << "Package: " << name << "-" << version << std::endl;

  auto print_map = [&info](std::map<std::string, std::string> map) {
    for (auto it = map.begin(); it != map.end(); ++it)
      info << "\tkey: " << it->first << "\n\tvalue: " << it->second << "\n\n";
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

  // parse file
  auto config = cpptoml::parse_file("test/" + pkg + "/" + yap::get_ybh(pkg));

  // get map from tables
  auto map_table = [&config](std::string tablename) {
    std::map<std::string, std::string> buff;
    auto table = config->get_table(tablename);
    if (!table)
      return buff;
    for (auto it = table->begin(); it != table->end(); ++it)
      buff[it->first] = it->second->template as<std::string>()->get();
    return buff;
  };

  // info
  auto info = config->get_table("info");

  name = *(info->get_as<std::string>("name"));
  version = *(info->get_as<std::string>("version"));
  description = *(info->get_as<std::string>("description"));
  manpage = *(info->get_as<std::string>("manpage"));
  license = *(info->get_as<std::string>("license"));

  // build
  auto build = config->get_table("build");

  source_link = *(build->get_as<std::string>("source"));
  compression_format = *(build->get_as<std::string>("compression_format"));
  patches = *(build->get_array_of<std::string>("patches"));
  compile_make = *(build->get_array_of<std::string>("make"));
  compile_install = *(build->get_array_of<std::string>("install"));
  compile_uninstall = *(build->get_array_of<std::string>("uninstall"));

  // tables map
  dependencies = map_table("dependencies");
  features = map_table("features");

  // name for functions
  name_ver = name + version;
}

//std::string yap::Package::GetNameVer() { return name_ver; }

void yap::Package::Mkdir() {
  fs::create_directory(name);
  chdir(name.c_str());
}

void yap::Package::Chdir() {
  chdir(name_ver.c_str());
}

void yap::Package::IsInstalled() {
  char aws;
  if (yap::is_installed(name_ver)) {
      std::cout << "This package is already installed\nReinstall? [y], [n]: ";
      std::cin >> aws;
      if (aws == 'n')
        exit(0);
      else if (aws != 'y')
        exit(-1);
  }
}

void yap::Package::PreCompile() {
  /* TODO: Implement pre compile options based on ybh */
}

// download
void yap::Package::Download() {
  yap::download(source_link, name_ver + compression_format);
}

// extract
void yap::Package::Extract() {
  yap::extract(name_ver + compression_format, compression_format, name_ver);
}

std::vector<std::string> search(std::string reg) {
  std::regex REX(reg + ".*");
  std::smatch m;

  std::vector<std::string> all, latests, matches;

  fs::path path = "test/";
  for (auto &sub_path : fs::directory_iterator(path)) {
    for (auto &file : fs::directory_iterator(sub_path)) {
      if (fs::is_regular_file(file))
        all.push_back((std::string)sub_path.path().filename() + "/" +
                      (std::string)file.path().filename());
    }
    latests.push_back(std::max_element(all.begin(), all.end())->c_str());
    all.erase(all.begin(), all.end());
  }

  for (auto &elem : latests) {
    if (std::regex_match(elem, m, REX))
      matches.push_back(m[0]);
  }

  return matches;
}

void add_n(std::string name) {
  std::ofstream file(name, std::ios::app);
  file << "  \n\n";
}

// apply possible patches
void apply_patches(std::vector<std::string> patches) {
  int i = 0;
  for (auto patch : patches) {
    std::string name = "patch_" + std::to_string(i) + ".patch";
    yap::download(patch, name);
    add_n(name);
    std::vector<std::string> buffer = {"patch", "-i", name};
    if (yap::launcher(buffer) == -1) {
      std::cerr << "Failed to apply patches\nerrno" << errno << "\n";
      exit(-1);
    }
    ++i;
  }
}

// compile process
void yap::Package::Compile() {
  if (patches.size() != 0)
    apply_patches(patches);

  // make & install
  yap::run(compile_make, "Compile");
}

void write_pkgs(std::string name, std::string pkg) {
  if (!yap::is_installed(pkg)) {
    std::ofstream file ("/tmp/world", std::ios::app);
    if (file.is_open())
      file << name + "/" + pkg << std::endl;
    file.close();
  }
}

// install
void yap::Package::Install() { /* working */
  write_pkgs(name, name_ver);
}

// uninstall
void yap::Package::Uninstall() { /* working */
}
