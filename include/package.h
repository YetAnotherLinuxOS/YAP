#ifndef PACKAGE
#define PACKAGE

#include "cpptoml.h"
#include "yap.h"
#include <map>
#include <string>

std::vector<std::string> search(std::string);

template <class T> std::map<std::string, std::string> map_table(T, std::string);

namespace yap {
class Package {
private:
  std::string name;
  std::string version;
  std::string license;
  std::string manpage;
  std::string description;
  std::string source_link;
  std::string compression_format;
  // std::vector<std::string> dependecies;
  std::map<std::string, std::string> dependencies;
  std::map<std::string, std::string> features;
  std::vector<std::string> compile_make;
  std::vector<std::string> compile_install;
  std::vector<std::string> compile_uninstall;
  std::vector<std::string> patches;
  bool installed = false;

public:
  std::string get_info();
  Package(std::string);
  std::string GetNameVer();
  void Download();
  void Extract();
  bool IsInstalled();
  void PreCompile();
  void Compile();
  void Uninstall();
};
} // namespace yap

#endif // PACKAGE
