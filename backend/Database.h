#ifndef IMAGE_SEARCH_DATABASE_H
#define IMAGE_SEARCH_DATABASE_H

#include "DBImage.h"

#include <memory>

// TODO try and test hiberlite, sqlite

namespace ImageSearch
{

  class Database {
  public:
    inline void save (const DBImage &image) { p_save (image); }
    inline int getLastId (void) { return p_getLastId (); }
    inline ImageFeaturesList findAll (int nMax = 0) { return p_findAll (nMax); }
    inline std::auto_ptr<DBImage> getById (int id) { return p_getById (id); }
  private:
    virtual void p_save (const DBImage &image) = 0;
    virtual int p_getLastId (void) = 0;
    virtual ImageFeaturesList p_findAll (int nMax = 0) = 0;
    virtual std::auto_ptr<DBImage> p_getById (int id) = 0;
  };

};

#endif // IMAGE_SEARCH_DATABASE_H
