#ifndef IMAGE_SEARCH_DATABASE_H
#define IMAGE_SEARCH_DATABASE_H

#include "DBImage.h"

#include <memory>

// TODO try and test hiberlite, sqlite

namespace ImageSearch
{

  class Database {
  public:
    virtual void save (const DBImage &image) = 0;
    virtual int getLastId (void) = 0;
    virtual DbImageList findAll (void) = 0;
    virtual std::auto_ptr<DBImage> getById (int id) = 0;
  };

};

#endif // IMAGE_SEARCH_DATABASE_H
