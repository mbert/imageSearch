#include "Backend.h"
#include "DBImage.h"

#include <string>
#include <stdexcept>
#include <iostream>

using namespace ImageSearch;

int
main(int argc, char **argv)
{
  ImageSearchBackend backend ("./");
  std::cout << "importer start" << std::endl;
  Features fY ((const unsigned char*)"asdf");
  Features fU ((const unsigned char*)"fdsa");
  Features fV ((const unsigned char*)"dsaf");
  fY[2] = '\0';
  fU[3] = '\0';
  fV[1] = '\0';
  float dY = 3.14;
  float dU = 9.16;
  float dV = 42.0;
  DBImage dbImage (0, "foo.png", fY, fU, fV, dY, dU, dV);
  backend.saveDbImage (dbImage);
  std::cout << "importer successfully finished" << std::endl;
  return 0;
}

