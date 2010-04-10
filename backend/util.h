#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>

namespace ImageSearch 
{
  namespace Util
  {

    bool dirExists (const std::string &path);

    int fileSize (const std::string &path);

    void makeDir (const std::string &path);

    std::string getCurrentDir (void);

    bool changeDir (const std::string &path);

    void mkDirHier (const std::string &path);

    std::string baseFileName (const std::string &path);
    std::string dirName (const std::string &path);
    std::string changeExtension (const std::string &path,
				 const std::string &newExtension);

  }
}


#endif // UTIL_H_INCLUDED
