#include "SerializationBasedImageSearchBackend.h"
#include "PreSelectScoreTable.h"

#include <boost/thread/mutex.hpp>

#include <iostream>

using namespace ImageSearch;

boost::mutex scoreTableMutex;

SerializationBasedImageSearchBackend::SerializationBasedImageSearchBackend (const std::string &imageDbPrefix)
  : ImageSearchBackend (imageDbPrefix)
{
  initScoreTable ();
}

SerializationBasedImageSearchBackend::~SerializationBasedImageSearchBackend (void)
{
}

void
SerializationBasedImageSearchBackend::initScoreTable (void)
{
  if (m_scoreTable == NULL)
    {
      boost::mutex::scoped_lock lock (scoreTableMutex);
      if (m_scoreTable == NULL)
	{
	  std::cout << "Setting up Score Table" << std::endl;
	  m_scoreTable = new PreSelectScoreTable (getDbImageRows (),
						  getDbImageCols (),
						  m_nKeptCoeffs);
	  std::cout << "Done" << std::endl;
	}
      else
	{
	  std::cout << "seems like someone else's just created the score table."
		    << std::endl;
	}
    }
  else
    {
      std::cout << "using existing score table." << std::endl;
    }
}

void
SerializationBasedImageSearchBackend::addImage (const unsigned long id,
						const std::string imageName, 
						const int rows, const int cols)
{
  std::auto_ptr<ImageFeatures> image (createImageFeatures (imageName, rows, cols));
  m_scoreTable->appendImage (id, *image);
  ++m_nDbImages;
}


std::string
SerializationBasedImageSearchBackend::getImageNameById (const unsigned long id)
{
  //std::auto_ptr<DBImage> dbImage (getDbImageById (id));
  //return dbImage->getFileName ();
  return "foo";
}
