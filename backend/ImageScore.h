#ifndef IMAGE_SEARCH_IMAGE_SCORE_H
#define IMAGE_SEARCH_IMAGE_SCORE_H

#include <vector>

namespace ImageSearch
{

  class ImageScore {
  public:
    ImageScore (int id, float score) : m_id (id), m_score (score) {}
    ImageScore () : m_id (-1), m_score (0) {}
    virtual ~ImageScore (void) {}
    inline int getId (void) const { return m_id; }
    inline float getScore (void) const { return m_score; }
    inline void setId (int id) { m_id = id; }
    inline void setScore (float score) { m_score = score; }
    inline void addToScore (float score) { m_score += score; }
    inline void subFromScore (float score) { m_score -= score; }
    friend inline bool operator< (const ImageScore &left, const
				  ImageScore &right) {
      return left.getScore () < right.getScore ();
    }
  private:
    int m_id;
    float m_score;
  };

  typedef std::vector<ImageScore> ImageScoreList;
  typedef ImageScoreList::iterator ImageScoreIterator;

};

#endif // IMAGE_SEARCH_IMAGE_SCORE_H
