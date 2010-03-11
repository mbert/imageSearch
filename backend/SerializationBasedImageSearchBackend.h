#ifndef IMAGE_SEARCH_SERIALIZATION_BASED_BACKEND_H
#define IMAGE_SEARCH_SERIALIZATION_BASED_BACKEND_H

#include "ImageSearchBackend.h"

namespace ImageSearch
{

  class Database;

  class SerializationBasedImageSearchBackend : public ImageSearchBackend {

  public:
    SerializationBasedImageSearchBackend (const std::string &imageDbPrefix);
    virtual ~SerializationBasedImageSearchBackend (void);
    void addImage (const unsigned long id, const std::string imageName,
		   const int rows, const int cols);

  protected:
    virtual std::string getImageNameById (const unsigned long id);

  private:
    void initScoreTable (void);
  };

};

#endif // IMAGE_SEARCH_SERIALIZATION_BASED_BACKEND_H
