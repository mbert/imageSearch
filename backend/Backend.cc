#include "ScoreTableFactory.h"
#include "Backend.h"
#include "ImageScore.h"
#include "postgresql/PostgresQl.h"
#include "macros.h"

#include <cxxutil/utils.h>

#include <boost/timer.hpp>
#include <boost/thread/mutex.hpp>

#include <WImage/ColorImage.hh>
#include <WImage/FileName.hh>
#include <WTools/ImageComparison.hh>

#include <unistd.h>
#include <climits>
#include <cstdio>
#include <cmath>

#include <stdexcept>
#include <iostream>
#include <memory>

using namespace ImageSearch;

#define THUMB_ROWS 250
#define THUMB_COLS 250

#define DB_IMAGE_COLS 256
#define DB_IMAGE_ROWS 256

#define KEPT_COEFFS 60
#define MAX_RESULTS 40

#define DB_NAME "images"
#define HOSTADDR "127.0.0.1"
#define USERNAME "md"
#define PASSWORD "md"

boost::mutex scoreTableMutex;
ScoreTable * ImageSearchBackend::m_scoreTable = NULL;

static void fillFeatureVectors (const Image &img,
				Features &posFeatures, Features &negFeatures);

static std::string guessMimeType (const std::string &fileName);

ImageSearchBackend::ImageSearchBackend (const std::string &imageDbPrefix)
  : m_sizeY (THUMB_ROWS), m_sizeX (THUMB_COLS), m_maxResults (MAX_RESULTS),
    m_nKeptCoeffs (KEPT_COEFFS), m_imageDbPrefix (imageDbPrefix)
{
  char buf[PATH_MAX];
  m_documentRoot = std::string (getcwd (buf, sizeof buf));
  m_database = new PostgresQl (HOSTADDR, DB_NAME, USERNAME, PASSWORD,
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

void
ImageSearchBackend::initScoreTable (void)
{
  if (m_scoreTable == NULL)
    {
      boost::mutex::scoped_lock lock(scoreTableMutex);
      if (m_scoreTable == NULL)
	{
	  std::cout << "score table is uninitialised, doing this now."
		    << std::endl;
	  DbImageList allImages = m_database->findAll ();
	  m_nDbImages = allImages.size ();
	  m_scoreTable = ScoreTableFactory::create (getDbImageRows (),
						    getDbImageCols (),
						    m_nKeptCoeffs, allImages,
						    ScoreTableFactory::GOOD);
	}
      else
	{
	  std::cout << "seems like someone else's just created the score table."
		    << std::endl;
	  m_nDbImages = -1;
	}
    }
  else
    {
      std::cout << "using existing score table." << std::endl;
      m_nDbImages = -1;
    }
  if (m_nDbImages < 0)
    {
      m_nDbImages = m_database->getLastId () + 1;
    }
}

ImageSearchBackend::~ImageSearchBackend (void)
{
  delete m_database;
}

std::string
ImageSearchBackend::setImage (const std::string &srcPath, const std::string &clientName)
{
  clearCurrentImage ();
  m_currentTempFile = "";
  std::string targetName = "/tmp/" + clientName;
  try
    {
      FileName fn (clientName.c_str ());
      std::auto_ptr<ColorImage> img (new ColorImage ());
      img->read (srcPath.c_str (), fn.guess ());
      std::auto_ptr<ColorImage> scaled (img->fitInto ((int)(m_sizeY / 1.5),
						      (int)(m_sizeX / 1.5)));
      scaled->write (targetName.c_str ());
      m_currentTempFile = targetName;
    }
  catch (const std::exception &e)
    {
      std::cerr << "Exception caught: " << e.what () << std::endl;
    }
  return m_currentTempFile;
}

void
ImageSearchBackend::clearCurrentImage (void)
{
  if (m_currentTempFile.size () > 0)
    {
      (void)remove (m_currentTempFile.c_str ());
    }
}

bool
ImageSearchBackend::isCurrentImageValid (void) const
{
  return m_currentTempFile.size () > 0;
}

int
ImageSearchBackend::getThumbHeight (void) const
{
  return THUMB_ROWS;
}

int
ImageSearchBackend::getThumbWidth (void) const
{
  return THUMB_COLS;
}

std::string
ImageSearchBackend::guessMimeType (void) const
{
  return ::guessMimeType (m_currentTempFile);
}

BLImage
ImageSearchBackend::makeBlImage (const std::string &fileName,
				 const std::string &text)
{
  std::string thumbNail = m_documentRoot + m_imageDbPrefix + "/thumb_"
    + CxxUtil::itoa (THUMB_ROWS) + "x" + CxxUtil::itoa (THUMB_COLS)
    + "_" + fileName;
  std::string targetUrl = m_imageDbPrefix + "/" + fileName;
  return ImageSearch::BLImage (thumbNail, ::guessMimeType (fileName),
			       text, targetUrl);
}

BlImageConstIterator
ImageSearchBackend::performSearch (void)
{
  m_searchResults.clear ();
  if (isCurrentImageValid ())
    {
      ImageScoreList result;
      for (int i = 0; i < m_nDbImages; ++i)
	{
	  result.push_back (ImageScore (i));
	}
      assert (result.size () == m_nDbImages);
      std::auto_ptr<ColorImage> image (new ColorImage ());
      image->read (m_currentTempFile.c_str ());
      m_scoreTable->query (*image, result);
      boost::timer timer;
      for (int i = 0; i < result.size () && i < m_maxResults; ++i)
	{
	  m_searchResults.push_back (getBlImage (result[i]));
	}
      int elapsed = (int)(timer.elapsed () * 1000);
      std::cout << "loading the images took "
		<< elapsed << " milliseconds." << std::endl;
    }

  return m_searchResults.begin ();
}

BLImage
ImageSearchBackend::getBlImage (const ImageScore &score)
{
  std::auto_ptr<DBImage> dbImage (m_database->getById (score.getId ()));
  std::string fileName = dbImage->getFileName ();
  std::string text = "File: " + fileName
    + ", score: " + CxxUtil::dtoa (score.getScore ());
  return makeBlImage (fileName, text);
}

bool
ImageSearchBackend::hasMore (const BlImageConstIterator &it) const
{
  return m_searchResults.end () != it;
}

int
ImageSearchBackend::getDbImageRows (void) const
{
  return DB_IMAGE_ROWS;
}

int
ImageSearchBackend::getDbImageCols (void) const
{
  return DB_IMAGE_COLS;
}


void
ImageSearchBackend::saveDbImage (const DBImage &image)
{
  m_database->save (image);
}

int
ImageSearchBackend::getLastDbImageId (void)
{
  return m_database->getLastId ();
}

DbImageList
ImageSearchBackend::getAllDbImages (void)
{
  return m_database->findAll ();
}

std::auto_ptr<DBImage>
ImageSearchBackend::getDbImageById (int id)
{
  return m_database->getById (id);
}


std::auto_ptr<DBImage>
ImageSearchBackend::createDbImage (const std::string &path,
				   int id, int rows, int cols)
{
  std::auto_ptr<ColorImage> img (new ColorImage ());
  img->read (path.c_str ());
  size_t pos = path.rfind ("/");
  std::string fileName ((pos != std::string::npos && pos < path.size () - 1)
			? path.substr (pos + 1) : path);

  std::auto_ptr<ColorImage> scaled (img->fitInto (rows, cols, true));
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

static std::string
guessMimeType (const std::string &fileName)
{
  FileName fn (fileName.c_str ());
  return fn.guessedMimeType ();
}

