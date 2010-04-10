#include "SerializationBasedImageSearchBackend.h"
#include "PreSelectScoreTable.h"
#include "BinaryScoreTableSerializer.h"
#include "TextScoreTableSerializer.h"
#include "../config.h"

#include <boost/thread/mutex.hpp>
#include <boost/timer.hpp>

#include <fstream>
#include <iostream>

using namespace ImageSearch;

boost::mutex scoreTableMutex;

static SERIALIZER_CLASS p_scoreTableSerialzer;

SerializationBasedImageSearchBackend::SerializationBasedImageSearchBackend (const std::string &imageDbPrefix, const std::string &archiveFileName)
  : ImageSearchBackend (imageDbPrefix), m_archiveFileName (archiveFileName)
{
  p_initScoreTable (LOAD);
}

SerializationBasedImageSearchBackend::SerializationBasedImageSearchBackend (const std::string &archiveFileName)
  : ImageSearchBackend (""), m_archiveFileName (archiveFileName)
{
  p_initScoreTable (EMPTY);
}

SerializationBasedImageSearchBackend::~SerializationBasedImageSearchBackend (void)
{
}

void
SerializationBasedImageSearchBackend::p_initScoreTable (const InitMode &initMode)
{
  if (m_scoreTable == NULL)
    {
      boost::mutex::scoped_lock lock (scoreTableMutex);
      if (m_scoreTable == NULL)
	{
	  if (initMode == LOAD)
	    {
	      std::cout << "Loading Score Table from file " << m_archiveFileName << std::endl;
	      try
		{
		  boost::timer timer;
		  m_scoreTable = p_scoreTableSerialzer.load (m_archiveFileName);
		  int elapsed = (int)(timer.elapsed () * 1000);
		  std::cout << "loading the images took "
			    << elapsed << " milliseconds." << std::endl;
		}
	      catch (const std::exception &e)
		{
		  std::cerr << "exception caught: " << e.what () << std::endl;
		  exit (1);
		}
	    }
	  else
	    {
	      std::cout << "Setting up empty Score Table" << std::endl;
	      m_scoreTable = new PreSelectScoreTable (getDbImageRows (),
						      getDbImageCols (),
						      m_nKeptCoeffs);
	    }
	  std::cout << "Loaded " << m_scoreTable->nImages () << " images, done" << std::endl;
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
SerializationBasedImageSearchBackend::addImage (const std::string imageName, 
						const int rows, const int cols)
{
  std::auto_ptr<ImageFeatures> image (createImageFeatures (imageName, rows, cols));
  m_scoreTable->appendImage (*image);
}

void
SerializationBasedImageSearchBackend::addImage (const ImageFeatures &image)
{
  m_scoreTable->appendImage (image);
}

void
SerializationBasedImageSearchBackend::save (void)
{
  try
    {
      p_scoreTableSerialzer.save (m_scoreTable, m_archiveFileName);
    }
  catch (const std::exception &e)
    {
      std::cerr << "exception caught: " << e.what () << std::endl;
      exit (1);
    }
}

std::string
SerializationBasedImageSearchBackend::getImageNameById (const unsigned long id)
{
  return m_scoreTable->getImageNameById (id);
}
