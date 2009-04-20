#include "ScoreTable.h"
#include "macros.h"

#include <WImage/ColorImage.hh>
#include <WTools/ImageComparison.hh>

#include <boost/timer.hpp>

#include <cmath>
#include <cstdlib>
#include <cassert>

using namespace ImageSearch;

#define MAX_WEIGHT_IDX 5

ScoreTable::ScoreTable (int rows, int cols, int nKeptCoeffs,
			const DbImageList &images)
  : m_rows (rows), m_cols (cols),
    m_nKeptCoeffs (nKeptCoeffs), m_nImages (images.size ())
{
  m_scoreListPerPixelSize = (int)::ceil (m_nImages / 8.0);
  m_lqcache.resize (MAX (m_rows, m_cols));
  for (int i = 0; i < m_lqcache.size (); i++)
    {
      m_lqcache[i] = (int)floor (log ((double)i) / log ((double)2));
    }
  m_bufSize = m_scoreListPerPixelSize * m_rows * m_cols;
  m_averageY.resize (m_nImages);
  m_averageU.resize (m_nImages);
  m_averageV.resize (m_nImages);
  m_positiveY = new unsigned char[m_bufSize];
  m_negativeY = new unsigned char[m_bufSize];
  m_positiveU = new unsigned char[m_bufSize];
  m_negativeU = new unsigned char[m_bufSize];
  m_positiveV = new unsigned char[m_bufSize];
  m_negativeV = new unsigned char[m_bufSize];
  std::cout << "allocated "
	    << m_bufSize << " bytes per feature vector, that is "
	    << m_bufSize / 1024 << " kilobytes." << std::endl;
  int index;
  for (DbImageIterator it = images.begin (); it != images.end (); ++it)
    {
      index = (*it)->getId ();
      m_averageY[index] = (*it)->getAverageY ();
      addImageFeatureVector (index, (*it)->getFeaturesYPlus (), m_positiveY);
      addImageFeatureVector (index, (*it)->getFeaturesYMinus (), m_negativeY);
      m_averageU[index] = (*it)->getAverageU ();
      addImageFeatureVector (index, (*it)->getFeaturesUPlus (), m_positiveU);
      addImageFeatureVector (index, (*it)->getFeaturesUMinus (), m_negativeU);
      m_averageV[index] = (*it)->getAverageV ();
      addImageFeatureVector (index, (*it)->getFeaturesVPlus (), m_positiveV);
      addImageFeatureVector (index, (*it)->getFeaturesVMinus (), m_negativeV);
    }
}

void
ScoreTable::addImageFeatureVector (int index, const Features &src,
				   unsigned char *dest)
{
  int size = m_rows * m_cols;
  int destByte = index / 8;
  int destBit = index % 8;
  int srcByte, srcBit;

  for (int i = 0; i < size; ++i)
    {
      int vectorStart = m_scoreListPerPixelSize * i;
      assert (vectorStart + destByte < m_bufSize);
      srcByte = i / 8;
      srcBit = i % 8;
      assert (srcByte < src.size ());
      if (IS_BIT_SET (src[srcByte], srcBit))
	{
	  SET_BIT (dest[vectorStart + destByte], destBit);
	}
      else
	{
	  RESET_BIT (dest[vectorStart + destByte], destBit);
	}
    }
}

ScoreTable::~ScoreTable (void)
{
  delete [] m_positiveY;
  delete [] m_negativeY;
  delete [] m_positiveU;
  delete [] m_negativeU;
  delete [] m_positiveV;
  delete [] m_negativeV;
}

void
ScoreTable::query (const ColorImage &image, ImageScoreList &scores)
{
  static const float weightY[6] = {  4.04, 0.79, 0.45, 0.42, 0.41, 0.32 };
  static const float weightU[6] = { 15.25, 0.92, 0.53, 0.26, 0.14, 0.07 };
  static const float weightV[6] = { 22.62, 0.40, 0.73, 0.25, 0.15, 0.38 };

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

  CoeffInformation ci;
  for (int i = 0; i < scores.size () ; ++i)
    {
      querySingleColor (*lY, scores[i], m_averageY[i], m_positiveY,
			m_negativeY, weightY);
      //scores[i].setYScore(scores[i].getScore ());
      //if (scores[i].getYScore () > 0)
	//{
	  // positive score? skip any further calculation and make sure
	  // this image does not get ranked high...
	  //scores[i].setScore (scores[i].getScore () * 3);
	//}
      //else
	//{
	  querySingleColor (*lU, scores[i], m_averageU[i], m_positiveU,
			    m_negativeU, weightU);
	  querySingleColor (*lV, scores[i], m_averageV[i], m_positiveV,
			    m_negativeV, weightV);
	//}

    }

  elapsed = (int)(timer.elapsed () * 1000);
  std::cout << "querying closest matches from all " << m_nImages
	    << " images took " << elapsed << " milliseconds." << std::endl;
  timer.restart ();

  std::sort (scores.begin (), scores.end ());
#if 0
  for (int i = 0; i < scores.size () ; ++i)
    {
      std::cout << "image ID: " << scores[i].getId ()
		<< ", yScore: " << scores[i].getYScore ()
		<< ", score: " << scores[i].getScore () << std::endl;
    }
#endif
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

bool
ScoreTable::isSet (const CoeffInformation &ci, int pos,
		   const unsigned char array[])
{
  int start = vectorStart (ci);
  int byte = pos / 8;
  int bit = pos % 8;
  return IS_BIT_SET (array[byte], bit);
}

int
ScoreTable::vectorStart (const CoeffInformation &ci)
{
  return m_scoreListPerPixelSize * (ci.ypos () * m_cols + ci.xpos ());
}

void
ScoreTable::querySingleColor (ImageInformation &truncated,
			      ImageScore &score,
			      float average,
			      const unsigned char *positives,
			      const unsigned char *negatives,
			      const float weights[])
{
  CoeffInformation ci;

  score.addToScore (weights[0] * (::abs (truncated.at(0).val () - average)));
  for (int j = 1; j < truncated.size (); ++j)
    {
      for (int k = 0; k < m_nImages; ++k)
	{
	  ci = truncated.at(j);
	  // feature vector: [01] (negative) [10] positive per pixels pos
	  // score table: 3 instead of 6 arrays, each organised in [00]/[01]/[10] 
	  // tuples according to feature vector, then if ((x & y) != 0) ...
	  if (ci.val () > 0 && isSet (ci, k, positives)
	      || isSet (ci, k, negatives))
	    {
	      score.subFromScore (weights[bin (ci.ypos (), ci.xpos ())]);
	    }
	}
    }
}


int
ScoreTable::getLevel (int i)
{
  return m_lqcache[i];
}

