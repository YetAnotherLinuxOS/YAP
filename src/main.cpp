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

int main(int argc, char *argv[]) {
  // test if the first arg is NULL or not
  std::string option = argv[1] != NULL ? std::string(argv[1]) : "";

  // help menssage
  std::stringstream help;
  help << "Usage: yap [option] [arg]\n\n";
  help << "-h/--help        print this menssage and leave\n";
  help << "-i/--install     install a program\n";
  help << "-q/--info        get information about a program\n";
  help << "-d/--download    only downloads the files\n";
  help << "-u/--uninstall   uninstall a program\n";

  // get options and do events based on it
  if (option == "-h" || option == "--help" || option.empty())
    std::cout << help.str() << std::endl;

  if (!option.empty() && argv[2] != NULL) {
    if (option == "-q" || option == "--info") {

      yap::Package pkg(argv[2]);
      std::string info = pkg.get_info();
      std::cout << info << "\n";

    } else if (option == "-d" | option == "--download") {

      yap::Package pkg(argv[2]);
      pkg.Download();

    } else if (option == "-i" | option == "--install") {

      yap::Package pkg(argv[2]);
      pkg.Download();
      pkg.Extract();
      chdir(pkg.GetNameVer().c_str());
      pkg.Compile();

    } else if (option == "-u" | option == "--uninstall") {

      yap::Package pkg(argv[2]);
      pkg.Uninstall();

    } else {
      std::cout << "yap: no such option: '" << argv[1] << "'" << std::endl;
      return 2;
    }

  } else {
    std::cout << "yap: no package option" << std::endl;
    return 2;
  }

  return 0;
}
