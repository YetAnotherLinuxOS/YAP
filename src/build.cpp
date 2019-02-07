#include <../include/yap.h>

// download
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

void add_n(std::string name) {
  std::ofstream file(name, std::ios::app);
  file << "  \n\n";
}

// apply possible patches
void yap::apply_patches(std::vector<std::string> patches) {
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

void set_prefix(std::string PREFIX) {
  // get PREFIX
  std::ifstream fin;
  fin.open("config.mk");

  std::string data;
  std::stringstream prefix_file;
  while (getline(fin, data))
    prefix_file << data << std::endl;

  fin.close();

  // replace PREFIX
  std::ofstream fout;
  fout.open("config.mk");

  std::regex REG("\nPREFIX.*");
  std::string new_prefix_file =
      std::regex_replace(prefix_file.str(), REG, "\nPREFIX = " + PREFIX);

  fout << new_prefix_file;
  fout.close();
}

// compile process
void yap::compile(std::vector<std::string> make,
                  std::vector<std::string> patches) {
  if (patches.size() != 0)
    yap::apply_patches(patches);

  // make & install
  yap::run(make, "Compile");
}

// uninstall
void yap::uninstall(std::vector<std::string> c_uninstall, std::string pkg) {
  // variables
  std::string path = "USR/yap/src";
  std::string pkg_path = path + "/" + pkg;
  chdir(pkg_path.c_str()); // change to source path

  // uninstall
  yap::run(c_uninstall, "Uninstall");
}
