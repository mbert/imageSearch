#ifndef IMAGE_SEARCH_BACKEND_H
#define IMAGE_SEARCH_BACKEND_H

#include "BLImage.h"
#include "DBImage.h"
#include "ScoreTable.h"

#include <string>
#include <memory>

namespace ImageSearch
{

  class Database;

  class ImageSearchBackend {
  public:
    ImageSearchBackend (const std::string &imageDbPrefix);
    ~ImageSearchBackend (void);

    // image search operations
    void clearCurrentImage (void);
    std::string setImage (const std::string &srcPath,
			  const std::string &clientName);
    bool isCurrentImageValid (void) const;
    std::string guessMimeType (void) const;
    BlImageConstIterator performSearch (void);
    bool hasMore (const BlImageConstIterator &it) const;
    int getThumbHeight (void) const;
    int getThumbWidth (void) const;

    // database operations
    void saveDbImage (const DBImage &image);
    int getLastDbImageId (void);
    int getDbImageRows (void) const;
    int getDbImageCols (void) const;
    DbImageList getAllDbImages (void);
    std::auto_ptr<DBImage> getDbImageById (int id);
    std::auto_ptr<DBImage> createDbImage (const std::string &path,
					  int id, int rows, int cols);

  private:
    BLImage makeBlImage (const std::string &fileName, const std::string &text);
    BLImage getBlImage (const ImageScore &score);
    void initScoreTable (void);
    std::string m_currentTempFile;
    std::string m_documentRoot;
    static ScoreTable *m_scoreTable;
    int m_nDbImages;
    int m_sizeY;
    int m_sizeX;
    int m_maxResults;
    int m_nKeptCoeffs;
    std::string m_imageDbPrefix;
    BlImageList m_searchResults;
    Database *m_database;
  };

};

#endif // IMAGE_SEARCH_BACKEND_H
