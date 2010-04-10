#ifndef IMAGE_SEARCH_SERIALIZATION_BASED_BACKEND_H
#define IMAGE_SEARCH_SERIALIZATION_BASED_BACKEND_H

#include "ImageSearchBackend.h"

namespace ImageSearch
{

  class Database;

  class SerializationBasedImageSearchBackend : public ImageSearchBackend {

  public:
    SerializationBasedImageSearchBackend (const std::string &imageDbPrefix,
					  const std::string &archiveFileName);
    SerializationBasedImageSearchBackend (const std::string &archiveFileName);
    virtual ~SerializationBasedImageSearchBackend (void);
    void addImage (const std::string imageName,
		   const int rows, const int cols);
    void addImage (const ImageFeatures &image);
    void save (void);

  protected:
    virtual std::string getImageNameById (const unsigned long id);

  private:
    typedef enum { EMPTY, LOAD } InitMode;
    void p_initScoreTable (const InitMode &initMode);
    std::string m_archiveFileName;
  };

};

#endif // IMAGE_SEARCH_SERIALIZATION_BASED_BACKEND_H
