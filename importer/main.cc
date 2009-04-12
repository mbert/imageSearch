#include "Backend.h"
#include "DBImage.h"

#include <string>
#include <stdexcept>
#include <iostream>

using namespace ImageSearch;

int
main(int argc, char **argv)
{
  std::cout << "importer start" << std::endl;
  ImageSearchBackend backend ("./");
  int id = backend.getLastImageId () + 1;
  std::auto_ptr<DBImage> dbImage = backend.createDbImage ("lenaxxx.ppm",
							  id, 256, 256);
  backend.saveDbImage (*dbImage);
  std::cout << "importer successfully finished" << std::endl;
  return 0;
}

