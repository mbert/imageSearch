#include "Backend.h"

#include <WImage/ColorImage.hh>
#include <WImage/FileName.hh>

#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <memory>

using namespace ImageSearch;

ImageSearchBackend::ImageSearchBackend (int sizeY, int sizeX)
  : m_sizeY (sizeY), m_sizeX (sizeX)
{
}

ImageSearchBackend::~ImageSearchBackend (void)
{
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
ImageSearchBackend::isCurrentImageValid (void)
{
  return m_currentTempFile.size () > 0;
}

std::string
ImageSearchBackend::guessMimeType (void)
{
  FileName fn (m_currentTempFile.c_str ());
  std::string type;
  switch (fn.guess ())
    {
    case fn_jpg:
      type = "jpeg";
      break;
    case fn_png:
      type = "png";
      break;
    case fn_gif:
      type = "gif";
      break;
    default:
      return "";
    }
  return "image/" + type;
}

