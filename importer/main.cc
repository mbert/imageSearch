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
  int id = backend.getLastDbImageId () + 1;
  const int rows = backend.getDbImageRows ();
  const int cols = backend.getDbImageCols ();
  int rc = 0;
  std::string imageName;
  while (!std::cin.eof ())
    {
      std::getline (std::cin, imageName);
      if (imageName.size () > 0)
	{
	  std::cout << "processing image: \"" << imageName
		    << "\"..." << std::endl;
	  try
	    {
	      backend.saveDbImage (*backend.createDbImage (imageName,
							   id, rows, cols));
	      ++id;
	      std::cout << "success." << std::endl;
	    }
	  catch (const std::exception &e)
	    {
	      std::cerr << "exception caught: " << e.what () << std::endl;
	      rc++;
	    }
	}
    }
  std::cout << "importer successfully finished" << std::endl;
  return rc;
}

