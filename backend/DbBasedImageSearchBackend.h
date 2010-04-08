#ifndef IMAGE_SEARCH_DB_BASED_BACKEND_H
#define IMAGE_SEARCH_DB_BASED_BACKEND_H

#include "ImageSearchBackend.h"

namespace ImageSearch
{

  class Database;

  class DbBasedImageSearchBackend : public ImageSearchBackend {

  public:
    DbBasedImageSearchBackend (const std::string &imageDbPrefix);
    virtual ~DbBasedImageSearchBackend (void);

    // stuff used by importer
    std::auto_ptr<DBImage> createDbImage (const std::string &path,
					  const unsigned long id,
					  int rows, int cols);
    int getLastDbImageId (void);
    void saveDbImage (const DBImage &image);

  protected:
    virtual std::string getImageNameById (const unsigned long id);

  private:
    void p_initScoreTable (void);
    std::auto_ptr<DBImage> p_getDbImageById (int id);
    ImageFeaturesList p_getAllDbImages (void);
    Database *m_database;
  };

};

#endif // IMAGE_SEARCH_DB_BASED_BACKEND_H
