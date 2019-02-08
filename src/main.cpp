#include "../include/package.h"
#include "../include/yap.h"
#include <iostream>

/*
 *
 * TODO:
 *
 *      download tarball in due directory
 *      unpack tarball in due directory
 *      make it
 *      install it
 *
 */

int main(int argc, char **argv) {
  // convert C array to C++ vector
  std::vector<std::string> options;

  for (unsigned i = 0; i <= argc; ++i) {
    if (argv[i] == NULL)
      options.push_back("");
    else
      options.push_back((std::string)argv[i]);
  }

  // help menssage
  std::stringstream help;
  help << "Usage: yap [option] [arg]\n\n";
  help << "-h, --help              print this menssage and leave\n";
  help << "-i, --install    <pkg>  install a program\n";
  help << "-q, --info       <pkg>  get information about a program\n";
  help << "-d, --download   <pkg>  only downloads the files\n";
  help << "-u, --uninstall  <pkg>  uninstall a program\n";
  help << "-s, --search     <rex>  search for packages\n";

  // get options and do events based on it
  if (options[1] == "-h" | options[1] == "--help" | options[1].empty()) {

    std::cout << help.str() << std::endl;

  } else if (options[1] == "-q" | options[1] == "--info") {

    yap::Package pkg(options[2]);
    std::string info = pkg.get_info();
    std::cout << info << "\n";

  } else if (options[1] == "-d" | options[1] == "--download") {

    yap::Package pkg(options[2]);
    pkg.Download();

  } else if (options[1] == "-i" | options[1] == "--install") {

    yap::Package pkg(options[2]);
    pkg.Download();
    pkg.Extract();
    chdir(pkg.GetNameVer().c_str());
    pkg.Compile();

  } else if (options[1] == "-u" | options[1] == "--uninstall") {

    yap::Package pkg(options[2]);
    pkg.Uninstall();

  } else if (options[1] == "-s" | options[1] == "--search") {

    for (auto &pkg : search(options[2]))
      std::cout << pkg << std::endl;

  } else {

    std::cout << "yap: no such option: '" << options[1] << "'" << std::endl;
    return 2;
  }

  return 0;
}
