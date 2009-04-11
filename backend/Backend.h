#ifndef IMAGE_SEARCH_BACKEND_H
#define IMAGE_SEARCH_BACKEND_H

#include "BLImage.h"
#include "DBImage.h"

#include <string>

namespace ImageSearch
{

  class Database;

  class ImageSearchBackend {
  public:
    ImageSearchBackend (const std::string &imageDbPrefix);
    ~ImageSearchBackend (void);
    void clearCurrentImage (void);
    std::string setImage (const std::string &srcPath,
			  const std::string &clientName);
    bool isCurrentImageValid (void) const;
    void showCurrentSearch (const std::string &fileName);
    void hideCurrentSearch (void);
    std::string guessMimeType (void) const;
    BLImageIterator performSearch (void);
    bool hasMore (const BLImageIterator &it) const;
    int getThumbHeight (void) const;
    int getThumbWidth (void) const;
    void saveDbImage (const DBImage &image);

  private:
    BLImage makeBlImage (const std::string &fileName, const std::string &text);
    std::string m_currentTempFile;
    std::string m_imageDbPrefix;
    std::string m_documentRoot;
    int m_sizeY;
    int m_sizeX;
    BLImageList m_searchResults;
    Database *m_database;
  };

};

#endif // IMAGE_SEARCH_BACKEND_H
