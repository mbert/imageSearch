#include "ImageSearchBackend.h"
#include "ImageScore.h"
#include "macros.h"
#include "config.h"

#include <cxxutil/utils.h>

#include <boost/timer.hpp>

#include <WImage/ColorImage.hh>
#include <WImage/FileName.hh>

#include <unistd.h>
#include <climits>
#include <cerrno>
#include <cstdio>
#include <cmath>
#include <cstring>

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <memory>

using namespace ImageSearch;

ScoreTable * ImageSearchBackend::m_scoreTable = NULL;
unsigned long ImageSearchBackend::m_nDbImages = 0;


static std::string guessMimeType (const std::string &fileName);
static std::string tempImageName (const std::string &baseName);
static void copyFile (const std::string &srcPath,
		      const std::string &targetPath);

ImageSearchBackend::ImageSearchBackend (const std::string &imageDbPrefix)
  : m_sizeY (THUMB_ROWS), m_sizeX (THUMB_COLS), m_maxResults (MAX_RESULTS),
    m_nKeptCoeffs (KEPT_COEFFS), m_imageDbPrefix (imageDbPrefix)
{
  char buf[PATH_MAX];
  m_documentRoot = std::string (getcwd (buf, sizeof buf));
}

ImageSearchBackend::~ImageSearchBackend (void)
{
}

std::string
ImageSearchBackend::thumbName (const std::string &fileName)
{
  FileName fn(fileName.c_str ());
  const char* ext = fn.ext ();
  if (ext == NULL || *ext == '\0')
    {
      return fileName + "_thumb";
    }
  std::string fExt = ext;
  std::string result = fileName;
  std::string::size_type pos = result.rfind (fExt);
  return result.replace (pos, fExt.size (), "_thumb" + fExt);
}

std::string
ImageSearchBackend::setImage (const unsigned long imageId)
{
  const std::string clientName = getImageNameById (imageId);
  std::string srcPath = m_documentRoot + m_imageDbPrefix + "/" + clientName;
  return setImage (srcPath, clientName);
}

std::string
ImageSearchBackend::setImage (const std::string &srcPath,
			      const std::string &clientName)
{
  clearCurrentImage ();
  m_currentTempFile = "";
  std::string targetName = tempImageName (clientName);
  try
    {
      FileName fn (clientName.c_str ());
      std::auto_ptr<ColorImage> img (new ColorImage ());
      img->read (srcPath.c_str (), fn.guess ());
      std::auto_ptr<ColorImage> thumb (img->fitInto ((int)(m_sizeY / 1.5),
						     (int)(m_sizeX / 1.5)));
      thumb->write (thumbName (targetName).c_str ());
      //img->write (targetName.c_str ());
      copyFile (srcPath, targetName);
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
      (void)remove (thumbName (m_currentTempFile).c_str ());
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

std::string
ImageSearchBackend::getDbInfoText(void) const
{
  return std::string ("Database: " DB_NAME ", table: " TABLE_NAME "_")
    + CxxUtil::itoa (DB_IMAGE_ROWS) + "_" + CxxUtil::itoa (DB_IMAGE_COLS)
    + "_" + CxxUtil::itoa (KEPT_COEFFS);
}

std::string
ImageSearchBackend::getScoreTableInfoText(void) const
{
  return m_scoreTable->getWeightsInfo ();
}

BLImage
ImageSearchBackend::makeBlImage (const int id, const std::string &fileName,
				 const std::string &text)
{
  std::string thumbNail = m_documentRoot + m_imageDbPrefix + "/thumb_"
    + CxxUtil::itoa (THUMB_ROWS) + "x" + CxxUtil::itoa (THUMB_COLS)
    + "_" + fileName;
  std::string targetUrl = m_imageDbPrefix + "/" + fileName;
  return ImageSearch::BLImage (id, thumbNail, ::guessMimeType (fileName),
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
      m_scoreTable->query (*image, result, false);
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
  std::string fileName = getImageNameById (score.getId ());
  std::string text = "File: " + fileName
    + ", score: " + CxxUtil::dtoa (score.getScore ());
  return makeBlImage (score.getId (), fileName, text);
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



static std::string
guessMimeType (const std::string &fileName)
{
  FileName fn (fileName.c_str ());
  return fn.guessedMimeType ();
}

static std::string
tempImageName (const std::string &baseName)
{
  static char buf[512];
  ::memset (buf, 0, sizeof buf);
  pid_t pid = getpid();
  snprintf (buf, sizeof buf - 1, "/tmp/util-tmp-%05d-XXXXXX", (int)pid);
  unsigned seed = (unsigned)pid;
  int rc = CxxUtil::tempFileName (buf, baseName.c_str (), seed);
  if (rc < 0)
    {
      throw std::invalid_argument ("Could not create temp file, rc: "
        + CxxUtil::itoa (rc) + ", system error is: "
	+ std::string (strerror (errno)).c_str ());
    }
  return std::string (buf);
}

static void
copyFile (const std::string &srcPath, const std::string &targetPath)
{
  std::ifstream from (srcPath.c_str ());
  if (!from)
    {
      throw std::ios::failure ("Unable to open src file: " + srcPath);
    }

  std::ofstream to (targetPath.c_str ());
  if (!to)
    {
      throw std::ios::failure ("Unable to open target file: " + targetPath);
    }

    char ch;
    while (from.get(ch))
      {
	to.put(ch);
      }

    if (!from.eof() || !to)
      {
	throw std::ios::failure ("Error copying: " + srcPath
				 + " -> " + targetPath);
      }
}
