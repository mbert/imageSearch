#include "UrlPrefixStrategy.h"
#include "util.h"

using namespace ImageSearch;

std::string
CoverScanUrlPrefixStrategy::doMakePrefix (const std::string &fileName) const
{
  std::string result = "";
  size_t found = fileName.find_first_of ("-");
  if (found == std::string::npos)
    {
      return result;
    }
  std::string tmp = fileName.substr (0, found);
  if (tmp.size () > 2)
    {
      result = tmp.substr (tmp.size () - 2);
    }
  return "/" + result;
}

