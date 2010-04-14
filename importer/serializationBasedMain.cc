#include "SerializationBasedImageSearchBackend.h"
#include "UrlPrefixStrategy.h"
#include "util.h"
#include "../config.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <string>
#include <stdexcept>
#include <iostream>

//#include <unistd.h>

using namespace ImageSearch;

BOOST_CLASS_IMPLEMENTATION(Features, boost::serialization::primitive_type)

static URL_PREFIX_STRATEGY_CLASS urlPrefixStrategy;

static std::string
featuresFileName (const std::string &fileName)
{
  std::string baseName = Util::baseFileName (fileName);
  std::string dirName = Util::dirName (fileName);
  std::string dbName = Util::baseFileName (dirName);
  std::string newName = Util::changeExtension (baseName, "ser");
  std::string prefix = "cache/" + dbName
    + urlPrefixStrategy.makePrefix (baseName);
  return prefix + "/" + newName;
}

static void
save (const ReadOnlyImage &imageFeatures, const std::string &fileName)
{
  std::string newName = featuresFileName (fileName);
  if (Util::fileSize (newName) > 0)
    {
      return;
    }
  std::string dirName = Util::dirName (newName);
  if (!Util::dirExists (dirName))
    {
      Util::mkDirHier (dirName);
    }
  std::ofstream ofs (newName.c_str (), std::ios::out | std::ios::binary);
  boost::archive::binary_oarchive oa (ofs);
  oa << imageFeatures;
}


static ReadOnlyImage
load (const std::string &fileName)
{
  ReadOnlyImage result;
  std::ifstream ifs (fileName.c_str (), std::ios::in | std::ios::binary);
  if (!ifs)
    {
      throw std::ios::failure ("error opening file " + fileName);
    }
  boost::archive::binary_iarchive ia (ifs);
  ia >> result;
  return result;
}


static int
import (void)
{
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
	  std::cout << imageName;
	  try
	    {
	      std::string serName = featuresFileName (imageName);
	      if (Util::fileSize (serName) > 0)
		{
		  std::cout << " (cached)";
		  ReadOnlyImage roImage = load (serName);
		  backend.addImage (roImage);
		}
	      else
		{
                  std::auto_ptr<ReadOnlyImage>
		    imageFeatures (backend.createImageFeatures (imageName,
								rows, cols));
		  save (*imageFeatures, imageName);
		  backend.addImage (*imageFeatures);
		  imageFeatures.reset ();
		}
	      ++i;
	    }
	  catch (const std::exception &e)
	    {
	      std::cerr << imageName << ": exception caught: " << e.what () << std::endl;
	      rc++;
	    }
	}
    }
  std::cout << std::endl << "saving..." << std::endl;
  try
    {
      backend.save ();
    }
  catch (const std::exception &e)
    {
      std::cerr << "exception caught: " << e.what () << std::endl;
      rc++;
      return rc;
    }
  std::cout << "importer successfully finished, indexed "
	    << backend.nImages () << " images to " << DB_FILE << std::endl;
  //std::cout << "importer finished, sleeping..." << std::endl;
  //::sleep (10000);
  return rc;
}

int
main (void)
{
  std::cout << "importer start, dumping to " << DB_FILE << std::endl;
  int rc = import ();
  return rc;
}


