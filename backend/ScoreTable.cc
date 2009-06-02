#include "ScoreTable.h"

#include <WImage/ColorImage.hh>
#include <WTools/ImageComparison.hh>

#include <boost/timer.hpp>

#include <cmath>
#include <cstdlib>
#include <cassert>

using namespace ImageSearch;

#define MAX_WEIGHT_IDX 5

const float ScoreTable::m_weightY[6] = {  4.04, 0.79, 0.45, 0.42, 0.41, 0.32 };
const float ScoreTable::m_weightU[6] = { 15.25, 0.92, 0.53, 0.26, 0.14, 0.07 };
const float ScoreTable::m_weightV[6] = { 22.62, 0.40, 0.73, 0.25, 0.15, 0.38 };

ScoreTable::ScoreTable (int rows, int cols, int nKeptCoeffs,
			const DbImageList &images)
  : m_rows (rows), m_cols (cols),
    m_nKeptCoeffs (nKeptCoeffs), m_nImages (images.size ())
{
  m_lqcache.resize (MAX (m_rows, m_cols));
  for (int i = 0; i < m_lqcache.size (); i++)
    {
      m_lqcache[i] = (int)floor (log ((double)i) / log ((double)2));
    }
  m_averageY.resize (m_nImages);
  m_averageU.resize (m_nImages);
  m_averageV.resize (m_nImages);
  int index;
  for (DbImageConstIterator it = images.begin (); it != images.end (); ++it)
    {
      index = (*it)->getId ();
      assert (index < m_averageY.size ());
      m_averageY[index] = (*it)->getAverageY ();
      assert (index < m_averageU.size ());
      m_averageU[index] = (*it)->getAverageU ();
      assert (index < m_averageV.size ());
      m_averageV[index] = (*it)->getAverageV ();
    }
}

ScoreTable::~ScoreTable (void)
{
}

void
ScoreTable::query (const ColorImage &image, ImageScoreList &scores)
{
  boost::timer timer;
  std::auto_ptr<ColorImage> scaled (image.fitInto (m_rows, m_cols, true));
  if (scaled->colormodel () != cm_yuv)
    {
      scaled->colormodel (cm_yuv);
    }

  std::auto_ptr<ImageInformation> lY (ImageComparison::imageInfoForLq
				      (scaled->channel (0),
				       m_nKeptCoeffs, Haar));
  std::auto_ptr<ImageInformation> lU (ImageComparison::imageInfoForLq
				      (scaled->channel (1),
				       m_nKeptCoeffs, Haar));
  std::auto_ptr<ImageInformation> lV (ImageComparison::imageInfoForLq
				      (scaled->channel (2),
				       m_nKeptCoeffs, Haar));

  int elapsed = (int)(timer.elapsed () * 1000);
  std::cout << "creating feature vector from the the image took "
	    << elapsed << " milliseconds." << std::endl;
  timer.restart ();

  p_query (*lY, *lU, *lV, scores);

  elapsed = (int)(timer.elapsed () * 1000);
  std::cout << "querying closest matches from all " << m_nImages
	    << " images took " << elapsed << " milliseconds." << std::endl;
  timer.restart ();

  std::sort (scores.begin (), scores.end ());
  elapsed = (int)(timer.elapsed () * 1000);
  std::cout << "sorting the query results took "
	    << elapsed << " milliseconds." << std::endl;
}

int
ScoreTable::bin (int y, int x)
{
  assert (!(y == 0 && x == 0));
  return MIN (getLevel (MAX (y, x)), MAX_WEIGHT_IDX);
}

int
ScoreTable::getLevel (int i)
{
  return m_lqcache[i];
}

