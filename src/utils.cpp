#include <../include/yap.h>

// get latest ybh from a given package
std::string yap::get_ybh(std::string name) {
  if (!fs::exists("test/" + name))
    exit(-1);
  fs::path path = "test/" + name;
  std::vector<std::string> versions;
  for (auto file : fs::directory_iterator(path)) {
    if (fs::is_regular_file(file))
      versions.push_back(file.path().filename());
  }
  return std::max_element(versions.begin(), versions.end())->c_str();
}

// function that search for REGEX filename in directory
std::vector<std::string> yap::getfile(std::string rex) {
  fs::path ybh_path = "test/"; // directory to read; '/' on the end IS necessary

  // regex; variable rex plus regex /.*/
  // smatch; used in 'regex_match' to list /matches/
  std::regex reg(rex + ".*");
  std::smatch matches;

  // create a string vector and list the files name in directory
  std::vector<std::string> ybhs;
  for (const auto &dirname : fs::directory_iterator(ybh_path)) {
    if (fs::is_regular_file(dirname))
      ybhs.push_back(fs::path(dirname).filename());
  }

  // create string vector with the matches
  std::vector<std::string> returning;
  for (const auto &ybh : ybhs) {
    if (std::regex_match(ybh, matches, reg))
      returning.push_back(matches[0]);
  }

  // if returning have only one element
  // insert ybh_path to the begin of the string
  if (returning.size() == 1) {
    returning.at(0).insert(0, std::string(ybh_path));
  }

  return returning;
}

// exec system commands - legacy
int yap::launcher(const char **command) {
  if (!fork()) {
    if (execvp(command[0], (char *const *)command) == -1)
      return -1;
  }
  wait(0);
  return 0;
}

// exec system commands
int yap::launcher(std::vector<std::string> commands) {
  char **args = vec_to_array(commands);
  if (!fork()) {
    if (execvp(args[0], args) == -1) {
      exit(-1);
    }
  }
  wait(0);
  free_array(args);
  return 0;
}

// run array commands
void yap::run(std::vector<std::string> commands, std::string name) {
  std::vector<std::string> args;
  for (auto command : commands) {
    args = split(command);
    if (yap::launcher(args) == -1) {
      std::cerr << name << " failed!\nerrno: " << errno << std::endl;
      exit(-1);
    }
  }
}

void free_array(char **arr) {
  int i = 0;
  while (arr[i] != NULL)
    delete[] arr[i++];
  delete[] arr;
}

char **vec_to_array(std::vector<std::string> vec) {
  char **a = new char *[vec.size() + 1];
  for (int i = 0; i < vec.size(); ++i) {
    a[i] = new char[vec[i].size()];
    strcpy(a[i], vec[i].c_str());
  }
  a[vec.size()] = NULL;
  return a;
}

std::vector<std::string> split(std::string string, std::string delimiter) {
  std::vector<std::string> buffer;
  size_t x;

  while ((x = string.find(delimiter)) != -1) {
    buffer.push_back(string.substr(0, x));
    string.erase(0, x + 1);
  }

  buffer.push_back(string);

  return buffer;
}
