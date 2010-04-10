#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>

#include <cstdio>
#include <cerrno>
#include <climits>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "util.h"

#ifndef PATH_MAX
#define PATH_MAX 512
#endif

using namespace ImageSearch;

static bool
endsWith (const std::string s, const char c)
{
  return s[s.size () - 1] == c;
}

/* yes, i've done some perl before :) */
static std::string
chop (const std::string s)
{
  return s.substr (0, s.size () - 1);
}

bool
Util::dirExists (const std::string &path)
{
  struct stat s;
  if (stat (path.c_str (), &s) < 0)
    {
      return false;
    }
  if (S_ISDIR (s.st_mode))
    {
      return true;
    }

  // not a real dir? Check if it's a symlink pointing to a dir
  std::string oldPath = getCurrentDir ();
  if (oldPath.size () > 0)
    {
      if (changeDir (path))
        {
          (void)changeDir (oldPath);
          return true;
        }
    }
  return false;
}

int
Util::fileSize (const std::string &path)
{
  struct stat s;
  if (stat (path.c_str (), &s) < 0)
    {
      return -1;
    }
  return s.st_size;
}

std::string
Util::getCurrentDir (void)
{
  static char buf[PATH_MAX];
  std::string result;
  if (getcwd (buf, sizeof buf) != NULL)
    {
      result = buf;
    }
  return result;
}

bool
Util::changeDir (const std::string &path)
{
  return chdir (path.c_str ()) == 0;
}

void
Util::makeDir (const std::string &path)
{
  (void)mkdir (path.c_str (), 0775);
}

void
Util::mkDirHier (const std::string &path)
{
  typedef boost::char_separator<char> pathsep;
  typedef boost::tokenizer<pathsep> pathtokenizer;
  const std::string tmpath = endsWith (path, '/')? chop (path): path;

  pathtokenizer tok (tmpath, pathsep ("/"));
  std::string theDir = (tmpath[0] == '/'? "/": "");
  for (pathtokenizer::iterator beg = tok.begin (); beg != tok.end (); ++beg)
    {
      theDir += *beg + "/";
      if (!dirExists (theDir))
	{
	  makeDir (theDir);
	}
    }
}

std::string
Util::baseFileName (const std::string &path)
{
  std::string::size_type lastDelim = path.rfind ('/');
  if (lastDelim != std::string::npos && lastDelim < path.size () - 1)
    {
      return path.substr (lastDelim + 1);
    }
  return path;
}

std::string
Util::dirName (const std::string &path)
{
  std::string ret = path;
  std::string::size_type lastDelim = ret.rfind ('/');
  if (lastDelim == std::string::npos)
    {
      return "";
    }
  ret.erase (lastDelim, std::string::npos);
  return ret;
}

std::string
Util::changeExtension (const std::string &path,
		       const std::string &newExtension)
{
  std::string theExtension;
  if (newExtension[0] == '.')
    {
      theExtension = newExtension;
    }
  else
    {
      theExtension = "." + newExtension;
    }
  size_t pos = path.rfind (".");
  if (pos == std::string::npos)
    {
      return path + theExtension;
    }
  return path.substr (0, pos) + theExtension;
}

