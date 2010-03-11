#include "DbBasedImageSearchBackend.h"
#include "PreSelectScoreTable.h"
#include "postgresql/PostgresQl.h"
#include "macros.h"
#include "config.h"

#include <boost/thread/mutex.hpp>

#include <iostream>

using namespace ImageSearch;

boost::mutex scoreTableMutex;

DbBasedImageSearchBackend::DbBasedImageSearchBackend (const std::string &imageDbPrefix)
  : ImageSearchBackend (imageDbPrefix)
{
  m_database = new PostgresQl (HOSTADDR, DB_NAME, TABLE_NAME,
			       USERNAME, PASSWORD,
			       DB_IMAGE_ROWS, DB_IMAGE_COLS, KEPT_COEFFS);

  if (m_imageDbPrefix.size () > 0)
    {
      initScoreTable ();
    }
  else
    {
      std::cout << "no image db prefix given, assuming minimal mode."
		<< std::endl;
    }
}

DbBasedImageSearchBackend::~DbBasedImageSearchBackend (void)
{
  delete m_database;
}

void
DbBasedImageSearchBackend::initScoreTable (void)
{
  if (m_scoreTable == NULL)
    {
      boost::mutex::scoped_lock lock (scoreTableMutex);
      if (m_scoreTable == NULL)
	{
	  std::cout << "score table is uninitialised, doing this now."
		    << std::endl;
	  std::cout << "initialising for image settings: "
		    << getDbImageRows () << "x" << getDbImageCols ()
		    << "@" << m_nKeptCoeffs << std::endl;
	  ImageFeaturesList allImages = m_database->findAll ();

	  std::cout << "loaded " << allImages.size ()
		    << " images from the database." << std::endl;
	  m_nDbImages = allImages.size ();
	  m_scoreTable = new PreSelectScoreTable (getDbImageRows (),
						  getDbImageCols (),
						  m_nKeptCoeffs);
	  m_scoreTable->loadImages (allImages);
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


std::string
DbBasedImageSearchBackend::getImageNameById (const unsigned long id)
{
  std::auto_ptr<DBImage> dbImage (getDbImageById (id));
  return dbImage->getFileName ();
}

void
DbBasedImageSearchBackend::saveDbImage (const DBImage &image)
{
  m_database->save (image);
}

ImageFeaturesList
DbBasedImageSearchBackend::getAllDbImages (void)
{
  return m_database->findAll ();
}

std::auto_ptr<DBImage>
DbBasedImageSearchBackend::getDbImageById (int id)
{
  return m_database->getById (id);
}

int
DbBasedImageSearchBackend::getLastDbImageId (void)
{
  return m_database->getLastId ();
}

std::auto_ptr<DBImage>
DbBasedImageSearchBackend::createDbImage (const std::string &path,
					  const unsigned long id,
					  int rows, int cols)
{
  std::auto_ptr<ImageFeatures> image (createImageFeatures (path, rows, cols));
  return std::auto_ptr<DBImage> (new DBImage (id, *image));
}


