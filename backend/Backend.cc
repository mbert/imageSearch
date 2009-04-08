#include "Backend.h"

using namespace ImageSearch;

ImageSearchBackend::ImageSearchBackend (void)
{
}

ImageSearchBackend::~ImageSearchBackend (void)
{
}

std::string
ImageSearchBackend::setImage (const std::string &srcPath, const std::string &clientName)
{
  clearCurrentImage ();
  return "/home/md/doerte.jpg";
}

void
ImageSearchBackend::clearCurrentImage (void)
{
}

bool
ImageSearchBackend::isCurrentImageValid (void)
{
  return true;
}

std::string
ImageSearchBackend::guessMimeType (void)
{
  return "image/jpeg";
}

