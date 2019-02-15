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

void yap::download(std::string url, std::string name) {
  std::vector<std::string> args = {"curl", url, "-o", name, "-L", "-s"};

  if (yap::launcher(args) == -1) {
    std::cerr << "Download failed!\nerrno: " << errno << "\n";
    exit(-1);
  }
}

// extract
void yap::extract(std::string name, std::string compression_format,
                  std::string directory) {
  fs::create_directory(directory);
  std::vector<std::string> args;
  if (compression_format == ".tar.gz")
    args = {"tar", "-x", "-f", name, "-C", directory, "--strip-components",
            "1"};
  else if (compression_format == ".zip")
    args = {"unzip", name, "-d", directory};
  else {
    std::cerr << "Compression not supported\n";
    exit(-1);
  }

  if (yap::launcher(args) == -1) {
    std::cerr << "Extract falied!\nerrno: " << errno << std::endl;
    exit(-1);
  }
}