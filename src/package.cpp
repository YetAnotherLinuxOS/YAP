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

template <class T>
std::map<std::string, std::string> map_table(T config, std::string tablename) {
  std::map<std::string, std::string> buff;
  auto table = config->get_table(tablename);
  if (!table)
    return buff;
  auto it = table->begin();
  while (it != table->end()) {
    buff[it->first] = it->second->template as<std::string>()->get();
    ++it;
  }
  return buff;
}

yap::Package::Package(std::string pkg) {
  if (pkg.empty()) {
    std::cout << "yap: no package option" << std::endl;
    exit(-1);
  }

  // parse file
  auto config = cpptoml::parse_file("test/" + pkg + "/" + yap::get_ybh(pkg));

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
  dependencies = map_table(config, "dependencies");
  features = map_table(config, "features");
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
