#ifndef IMAGE_SEARCH_BACKEND_H
#define IMAGE_SEARCH_BACKEND_H

#include "BLImage.h"
#include "ReadOnlyImage.h"
#include "ScoreTable.h"

#include <string>
#include <memory>

namespace ImageSearch
{

  class ImageSearchBackend {
  public:
    ImageSearchBackend (const std::string &imageDbPrefix);
    virtual ~ImageSearchBackend (void);

    // image search operations
    void clearCurrentImage (void);
    std::string setImage (const std::string &srcPath,
			  const std::string &clientName);
    std::string setImage (const unsigned long imageId);
    bool isCurrentImageValid (void) const;
    std::string guessMimeType (void) const;
    std::string getDbInfoText(void) const;
    std::string getScoreTableInfoText(void) const;
    BlImageConstIterator performSearch (void);
    bool hasMore (const BlImageConstIterator &it) const;
    int getThumbHeight (void) const;
    int getThumbWidth (void) const;
    int getNKeptCoefficients (void) const { return m_nKeptCoeffs; }
    static std::string thumbName (const std::string &fileName);

    // Operations for indexing
    int getDbImageRows (void) const;
    int getDbImageCols (void) const;
    std::auto_ptr<ReadOnlyImage> createImageFeatures (const std::string &path,
						      int rows, int cols);
    int nImages (void) const { return m_scoreTable->nImages (); }

  protected:
    virtual std::string getImageNameById (const unsigned long id) = 0;
    static ScoreTable *m_scoreTable;
    int m_nKeptCoeffs;
    std::string m_documentRoot;
    std::string m_imageDbPrefix;

  private:
    BLImage p_makeBlImage (const int id, const std::string &fileName,
			   const std::string &text);
    BLImage p_getBlImage (const ImageScore &score);
    void p_sortScores (const ImageScoreList &scores, ImageScoreList &result);
    std::string m_currentTempFile;
    int m_sizeY;
    int m_sizeX;
    int m_maxResults;
    BlImageList m_searchResults;
  };

};

#endif // IMAGE_SEARCH_BACKEND_H
