#ifndef IMAGE_SEARCH_DBIMAGE_H
#define IMAGE_SEARCH_DBIMAGE_H

#include <string>
#include <vector>

namespace ImageSearch
{

  typedef std::basic_string<unsigned char> Features;

  class DBImage {
  public:
    DBImage (void) { }

    DBImage (unsigned long id,
	     const std::string &fileName,
	     int nRows, int nCols,
	     const Features &featureVector)
      : m_id (id), m_fileName (fileName),
      m_nRows (nRows), m_nCols (nCols), m_featureVector (featureVector) { }

    unsigned long getId (void) const { return m_id; }
    const std::string &getFilename (void) const { return m_fileName; }
    int getNRows (void) const { return m_nRows; }
    int getNCols (void) const { return m_nCols; }
    const Features &getFeatures (void) const { return m_featureVector; }

    void setId (unsigned long id) { m_id = id; }
    void setFilename (std::string &fileName) { m_fileName = fileName; }
    void setNRows (int nRows) { m_nRows = nRows; }
    void setNCols (int nCols) { m_nCols = nCols; }
    setFeatures (Features &featureVector) { m_featureVector = featureVector; }

  private:
    unsigned long m_id;
    std::string m_fileName;
    int m_nRows;
    int m_nCols;
    Features m_featureVector;
  };

  typedef std::vector<DBImage> DBImageList;
  typedef DBImageList::const_iterator DBImageIterator;
};


#endif // IMAGE_SEARCH_DBIMAGE_H
