#include "BitArrayScoreTable.h"
#include "macros.h"

#include <boost/timer.hpp>

#include <cmath>
#include <cstdlib>
#include <cassert>

using namespace ImageSearch;

BitArrayScoreTable::BitArrayScoreTable (int rows, int cols, int nKeptCoeffs,
					const DbImageList &images)
  : ScoreTable (rows, cols, nKeptCoeffs, images)
{
  boost::timer timer;
  m_scoreListPerPixelSize = (int)::ceil (m_nImages / 8.0);
  m_bufSize = m_scoreListPerPixelSize * m_rows * m_cols;
  m_positiveY = new unsigned char[m_bufSize];
  m_negativeY = new unsigned char[m_bufSize];
  m_positiveU = new unsigned char[m_bufSize];
  m_negativeU = new unsigned char[m_bufSize];
  m_positiveV = new unsigned char[m_bufSize];
  m_negativeV = new unsigned char[m_bufSize];
  std::cout << "allocated "
	    << m_bufSize * 6 << " bytes, that is "
	    << m_bufSize * 6 / 1024 << " kilobytes." << std::endl;
  int index;
  for (DbImageConstIterator it = images.begin (); it != images.end (); ++it)
    {
      index = (*it)->getId ();
      addImageFeatureVector (index, (*it)->getFeaturesYPlus (), m_positiveY);
      addImageFeatureVector (index, (*it)->getFeaturesYMinus (), m_negativeY);
      addImageFeatureVector (index, (*it)->getFeaturesUPlus (), m_positiveU);
      addImageFeatureVector (index, (*it)->getFeaturesUMinus (), m_negativeU);
      addImageFeatureVector (index, (*it)->getFeaturesVPlus (), m_positiveV);
      addImageFeatureVector (index, (*it)->getFeaturesVMinus (), m_negativeV);
    }
  int elapsed = (int)(timer.elapsed () * 1000);
  std::cout << "creating the BitArrayScoreTable for all " << m_nImages
	    << " images took " << elapsed << " milliseconds." << std::endl;
}

void
BitArrayScoreTable::addImageFeatureVector (int index, const Features &src,
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

BitArrayScoreTable::~BitArrayScoreTable (void)
{
  delete [] m_positiveY;
  delete [] m_negativeY;
  delete [] m_positiveU;
  delete [] m_negativeU;
  delete [] m_positiveV;
  delete [] m_negativeV;
}

void
BitArrayScoreTable::p_query (ImageInformation &qY, ImageInformation &qU,
			     ImageInformation &qV, ImageScoreList &scores)
{

  CoeffInformation ci;
  for (int i = 0; i < scores.size () ; ++i)
    {
      querySingleColor (qY, i, scores[i], m_averageY[i], m_positiveY,
			m_negativeY, m_weightY);
      querySingleColor (qU, i, scores[i], m_averageU[i], m_positiveU,
			m_negativeU, m_weightU);
      querySingleColor (qV, i, scores[i], m_averageV[i], m_positiveV,
			m_negativeV, m_weightV);
    }

}

bool
BitArrayScoreTable::isSet (const CoeffInformation &ci, int pos,
			   const unsigned char positives[],
			   const unsigned char negatives[])
{
  int start = vectorStart (ci);
  int byte = pos / 8;
  int bit = pos % 8;
  return IS_BIT_SET ((ci.val () > 0 ? positives : negatives)[byte], bit);
}

int
BitArrayScoreTable::vectorStart (const CoeffInformation &ci)
{
  return m_scoreListPerPixelSize * (ci.ypos () * m_cols + ci.xpos ());
}

void
BitArrayScoreTable::querySingleColor (ImageInformation &truncated, int pos,
				      ImageScore &score, float average,
				      const unsigned char *positives,
				      const unsigned char *negatives,
				      const float weights[])
{
  CoeffInformation ci;

  int difference = (int)(truncated.at(0).val () - average);
  score.addToScore (weights[0] * (::abs (difference)));
  for (int j = 1; j < truncated.size (); ++j)
    {
      ci = truncated.at(j);
      if (isSet (ci, pos, positives, negatives))
	{
	  score.subFromScore (weights[bin (ci.ypos (), ci.xpos ())]);
	}
    }
}


