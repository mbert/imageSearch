#include "SerializationBasedImageSearchBackend.h"
#include "../config.h"

#include <string>
#include <stdexcept>
#include <iostream>

using namespace ImageSearch;

int
main(int argc, char **argv)
{
  std::cout << "importer start, dumping to " << DB_FILE << std::endl;
  SerializationBasedImageSearchBackend backend (DB_FILE);
  const int rows = backend.getDbImageRows ();
  const int cols = backend.getDbImageCols ();
  int rc = 0;
  std::string imageName;
  int i = 0;
  while (!std::cin.eof ())
    {
      std::getline (std::cin, imageName);
      if (imageName.size () > 0)
	{
	  if (i > 0)
	    {
	      std::cout << ", ";
	    }
	  std::cout << "processing image: \"" << imageName << "\"";
	  try
	    {
	      backend.addImage (imageName, rows, cols);
	      ++i;
	    }
	  catch (const std::exception &e)
	    {
	      std::cerr << imageName << ": exception caught: " << e.what () << std::endl;
	      rc++;
	    }
	}
    }
  std::cout << "saving..." << std::endl;
  try
    {
      backend.save ();
    }
  catch (const std::exception &e)
    {
      std::cerr << "exception caught: " << e.what () << std::endl;
    }
  std::cout << "importer successfully finished, indexed " << backend.nImages () << " images." << std::endl;
  return rc;
}

