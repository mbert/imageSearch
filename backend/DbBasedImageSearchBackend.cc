#include "DbBasedImageSearchBackend.h"
#include "PreSelectScoreTable.h"
#include "postgresql/PostgresQl.h"
#include "macros.h"
#include "config.h"

#include <WImage/ColorImage.hh>
#include <WTools/ImageComparison.hh>

#include <boost/thread/mutex.hpp>

#include <iostream>

using namespace ImageSearch;

boost::mutex scoreTableMutex;

static void fillFeatureVectors (const Image &img,
				Features &posFeatures, Features &negFeatures);

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
	  DbImageList allImages = m_database->findAll ();

	  std::cout << "loaded " << allImages.size ()
		    << " images from the database." << std::endl;
	  m_nDbImages = allImages.size ();
	  m_scoreTable = new PreSelectScoreTable (getDbImageRows (),
						  getDbImageCols (),
						  m_nKeptCoeffs, allImages);
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

DbImageList
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
				   int id, int rows, int cols)
{
  std::auto_ptr<ColorImage> img (new ColorImage ());
  img->read (path.c_str ());
  size_t pos = path.rfind ("/");
  std::string fileName ((pos != std::string::npos && pos < path.size () - 1)
			? path.substr (pos + 1) : path);

  std::auto_ptr<ColorImage> scaled (img->fitInto (rows, cols, ef_outerBorder));
  img.reset ();
  if (scaled->colormodel () != cm_yuv)
    {
      scaled->colormodel (cm_yuv);
    }
  //scaled->write ("foo.ppm");

  std::auto_ptr<Image> lY (ImageComparison::truncateForLq
			   (scaled->channel (0), m_nKeptCoeffs, Haar));
  Features pfY, nfY;
  ::fillFeatureVectors (*lY, pfY, nfY);
  float aY = lY->at (0, 0);
  lY.reset ();

  std::auto_ptr<Image> lU (ImageComparison::truncateForLq
			   (scaled->channel (1), m_nKeptCoeffs, Haar));
  Features pfU, nfU;
  ::fillFeatureVectors (*lU, pfU, nfU);
  float aU = lU->at (0, 0);
  lU.reset ();

  std::auto_ptr<Image> lV (ImageComparison::truncateForLq
			   (scaled->channel (2), m_nKeptCoeffs, Haar));
  Features pfV, nfV;
  ::fillFeatureVectors (*lV, pfV, nfV);
  float aV = lV->at (0, 0);
  lV.reset ();


  return std::auto_ptr<DBImage> (new DBImage (id, fileName, pfY, nfY, pfU, nfU,
					      pfV, nfV, aY, aU, aV));
}

static void
fillFeatureVectors (const Image &img,
		    Features &posFeatures, Features &negFeatures)
{
  size_t size = (size_t)ceil (img.size () / 8.0);
  posFeatures.assign (size, (unsigned char)0);
  negFeatures.assign (size, (unsigned char)0);

  for (int i = 0; i < img.size (); ++i)
    {
      // todo: only if non zero!
      int byte = i / 8;
      int bit = i % 8;
      if (img.at (i) > 0)
	{
	  SET_BIT (posFeatures[byte], bit);
	}
      else if (img.at (i) < 0)
	{
	  SET_BIT (negFeatures[byte], bit);
	}
    }
}

