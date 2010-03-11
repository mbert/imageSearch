#include "SerializationBasedImageSearchBackend.h"

#include <string>
#include <stdexcept>
#include <iostream>

using namespace ImageSearch;

int
main(int argc, char **argv)
{
  std::cout << "importer start" << std::endl;
  SerializationBasedImageSearchBackend backend ("");
  const int rows = backend.getDbImageRows ();
  const int cols = backend.getDbImageCols ();
  int rc = 0;
  std::string imageName;
  int id = 0;
  while (!std::cin.eof ())
    {
      std::getline (std::cin, imageName);
      if (imageName.size () > 0)
	{
	  std::cout << "processing image: \"" << imageName
		    << "\"..." << std::endl;
	  try
	    {
	      backend.addImage (id, imageName, rows, cols);
	      ++id;
	    }
	  catch (const std::exception &e)
	    {
	      std::cerr << "exception caught: " << e.what () << std::endl;
	      rc++;
	    }
	}
    }
  std::cout << "importer successfully finished, indexed " << backend.getNImages () << " images." << std::endl;
  return rc;
}

