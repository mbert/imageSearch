#include "ScoreTable.h"

#include <WImage/ColorImage.hh>
#include <WTools/ImageComparison.hh>

#include <boost/timer.hpp>

#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cassert>

using namespace ImageSearch;

#define MAX_WEIGHT_IDX 5

//#define MY_WEIGHTS

#ifdef MY_WEIGHTS

static const float gl_weights[][N_WEIGHTS] = {
  {  2.50, 1.66, 2.02, 1.04, 0.94, 0.60 },
  {  9.60, 2.52, 0.88, 1.06, 0.56, 0.28 },
  { 17.23, 0.72, 0.90, 0.28, 0.36, 0.54 }
};
const float* ScoreTable::m_weightY = gl_weights[0];
const float* ScoreTable::m_weightU = gl_weights[1];
const float* ScoreTable::m_weightV = gl_weights[2];

#else
const float*
ScoreTable::m_weightY = ImageComparison::getWeights (SCANNED, 0);
const float*
ScoreTable::m_weightU = ImageComparison::getWeights (SCANNED, 1);
const float*
ScoreTable::m_weightV = ImageComparison::getWeights (SCANNED, 2);

#endif

ScoreTable::ScoreTable (int rows, int cols, int nKeptCoeffs,
			const DbImageList &images)
  : m_rows (rows), m_cols (cols),
    m_nKeptCoeffs (nKeptCoeffs), m_nImages (images.size ())
{
  m_lqcache.resize (MAX (m_rows, m_cols));
  for (int i = 0; i < m_lqcache.size (); i++)
    {
      m_lqcache[i] = 1 + (int)floor (log ((double)i) / log ((double)2));
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

std::string
ScoreTable::getWeightsInfo () const
{
  std::stringstream result;
  result.setf (std::ios::fixed, std::ios::floatfield);
  result << "Y: ";
  for (int i = 0; i < 6; ++i)
    {
      result << std::setprecision (4) << std::setw (7) << m_weightY[i] << ' ';
    }
  result << "U: ";
  for (int i = 0; i < 6; ++i)
    {
      result << std::setprecision (4) << std::setw (7) << m_weightU[i] << ' ';
    }
  result << "V: ";
  for (int i = 0; i < 6; ++i)
    {
      result << std::setprecision (4) << std::setw (7) << m_weightV[i] << ' ';
    }
  return result.str ();
}

void
ScoreTable::query (const ColorImage &image, ImageScoreList &scores, bool debug)
{
  boost::timer timer;
  std::auto_ptr<ColorImage> scaled (image.fitInto (m_rows, m_cols,
						   ef_outerBorder));
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

  if (debug)
    {
      std::cerr << "query image: avg(y): " << lY->at(0).val () << std::endl;
      std::cerr << "query image: avg(u): " << lU->at(0).val () << std::endl;
      std::cerr << "query image: avg(V): " << lV->at(0).val () << std::endl;
    }

  int elapsed = (int)(timer.elapsed () * 1000);
  std::cout << "creating feature vector from the the image took "
	    << elapsed << " milliseconds." << std::endl;
  timer.restart ();

  p_query (*lY, *lU, *lV, scores, debug);

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

