#include "Backend.h"
#include "postgresql/PostgresQl.h"

#include <cxxutil/utils.h>

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

#define THUMB_ROWS 250
#define THUMB_COLS 250

#define DB_IMAGE_COLS 256
#define DB_IMAGE_ROWS 256

using namespace ImageSearch;

#define DB_NAME "images"
#define HOSTADDR "127.0.0.1"
#define USERNAME "md"
#define PASSWORD "md"

#define KEPT_COEFFS 50

static void fillFeatureVectors (const Image &img,
				Features &posFeatures, Features &negFeatures);

static std::string guessMimeType (const std::string &fileName);

ImageSearchBackend::ImageSearchBackend (const std::string &imageDbPrefix)
  : m_sizeY (THUMB_ROWS), m_sizeX (THUMB_COLS),
    m_imageDbPrefix (imageDbPrefix), m_nKeptCoeffs (KEPT_COEFFS)
{
  char buf[PATH_MAX];
  m_documentRoot = std::string (getcwd (buf, sizeof buf));
  m_database = new PostgresQl (HOSTADDR, DB_NAME, USERNAME, PASSWORD,
			       DB_IMAGE_ROWS, DB_IMAGE_COLS, KEPT_COEFFS);
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
      img->fitInto (m_sizeY, m_sizeX);
      img->write (targetName.c_str ());
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

ImageSearch::BLImage
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

BlImageIterator
ImageSearchBackend::performSearch (void)
{
  m_searchResults.clear ();
  if (isCurrentImageValid ())
    {
      m_searchResults.push_back (makeBlImage ("044_ukraine_2008.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("035_ukraine_2008.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("dscn0351.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("019_ukraine_2004_07.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("011_ukraine_2005_01.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("020_ukraine_2005.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("021_ukraine_2008.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("020_ukraine_2008.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("046_ukraine_2008.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("002_ukraine_2004_07.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("011_ukraine_2006.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("013_ukraine_2008.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("003_ukraine_2005_01.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("010_ukraine_2004_07.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("038_ukraine_2006.jpg", "bla"));
      m_searchResults.push_back (makeBlImage ("007_london_2004_11.jpg", "bla"));
    }

  return m_searchResults.begin ();
}

bool
ImageSearchBackend::hasMore (const BlImageIterator &it) const
{
  return m_searchResults.end () != it;
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
  typedef std::auto_ptr<ColorImage> CiP;
  typedef std::auto_ptr<Image> GiP;

  CiP img (new ColorImage ());
  img->read (path.c_str ());

  CiP scaled (img->fitInto (rows, cols, true));
  if (scaled->colormodel () != cm_yuv)
    {
      scaled->colormodel (cm_yuv);
    }
  scaled->write ("foo.ppm");

  GiP lY (ImageComparison::truncateForLq
	  (scaled->channel (0), m_nKeptCoeffs, Haar));
  Features pfY, nfY;
  ::fillFeatureVectors (*lY, pfY, nfY);
  float aY = lY->at (0, 0);

  GiP lU (ImageComparison::truncateForLq
	  (scaled->channel (1), m_nKeptCoeffs, Haar));
  Features pfU, nfU;
  ::fillFeatureVectors (*lU, pfU, nfU);
  float aU = lU->at (0, 0);

  GiP lV (ImageComparison::truncateForLq
	  (scaled->channel (2), m_nKeptCoeffs, Haar));
  Features pfV, nfV;
  ::fillFeatureVectors (*lV, pfV, nfV);
  float aV = lV->at (0, 0);


  return std::auto_ptr<DBImage> (new DBImage (id, path, pfY, nfY, pfU, nfU,
					      pfV, nfV, aY, aU, aV));
}

#define SINGLE_BIT_AT(i) (((unsigned char)1) << (7 - i))

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
	  posFeatures[byte] |= SINGLE_BIT_AT (bit);
	}
      else if (img.at (i) < 0)
	{
	  negFeatures[byte] |= SINGLE_BIT_AT (bit);
	}
    }
}

static std::string
guessMimeType (const std::string &fileName)
{
  FileName fn (fileName.c_str ());
  return fn.guessedMimeType ();
}

