#ifndef IMAGE_SEARCH_DATABASE_H
#define IMAGE_SEARCH_DATABASE_H

#include "DBImage.h"

namespace ImageSearch
{

  class Database {
  public:
    virtual void save (const DBImage &image) = 0;
  };

};

#endif // IMAGE_SEARCH_DATABASE_H
