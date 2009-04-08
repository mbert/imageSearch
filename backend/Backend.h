#ifndef IMAGE_SEARCH_BACKEND_H
#define IMAGE_SEARCH_BACKEND_H

#include <string>

namespace ImageSearch
{

  class ImageSearchBackend {
  public:
    ImageSearchBackend (int sizeY, int sizeX);
    ~ImageSearchBackend (void);
    void clearCurrentImage (void);
    std::string setImage (const std::string &srcPath, const std::string &clientName);
    bool isCurrentImageValid (void);
    void showCurrentSearch (const std::string &fileName);
    void hideCurrentSearch (void);
    std::string guessMimeType (void);

  private:
    std::string m_currentTempFile;
    int sizeY;
    int sizeX;
  };

};

#endif // IMAGE_SEARCH_BACKEND_H
