#include <../include/yap.h>

// get info from keys
std::string yap::toml_string::table(std::string filename, std::string keyname,
                                    std::string tablename) {
  // initialize file 'filename'
  auto config = cpptoml::parse_file(filename);

  // get table and get key from table
  auto table = config->get_table(tablename);
  auto info = table->get_as<std::string>(keyname);

  return (*info);
}

std::map<std::string, std::string>
yap::toml_string::map_table(std::string filename, std::string tablename) {
  std::map<std::string, std::string> buff;
  auto table = cpptoml::parse_file(filename)->get_table(tablename);
  if (!table)
    return buff;
  auto it = table->begin();
  while (it != table->end()) {
    buff[it->first] = it->second->as<std::string>()->get();
    ++it;
  }
  return buff;
}

// get info from arrays
std::vector<std::string> yap::toml_string::array(std::string filename,
                                                 std::string keyname) {
  // initialize file 'filename'
  auto config = cpptoml::parse_file(filename);

  // get array
  auto info = config->get_array_of<std::string>(keyname);

  return (*info);
}

// get array from table
std::vector<std::string> yap::toml_string::tarray(std::string filename,
                                                  std::string keyname,
                                                  std::string tablename) {
  // initialize file 'filename'
  auto config = cpptoml::parse_file(filename);

  // get array
  auto table = config->get_table(tablename);
  auto info = table->get_array_of<std::string>(keyname);

  return (*info);
}
