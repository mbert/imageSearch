#include "PreSelectScoreTable.h"
#include "macros.h"

#include <boost/timer.hpp>

using namespace ImageSearch;

PreSelectScoreTable::PreSelectScoreTable (int rows, int cols, int nKeptCoeffs,
					  const DbImageList &images)
  : ScoreTable (rows, cols, nKeptCoeffs, images)
{
  boost::timer timer;
  int size = rows * cols;
  std::cout << "resizing id-list-lists..." << std::endl;
  m_positiveY.resize (size);
  m_negativeY.resize (size);
  m_positiveU.resize (size);
  m_negativeU.resize (size);
  m_positiveV.resize (size);
  m_negativeV.resize (size);

  // a single vector has about 16 bytes of size
  // each color channel has m_nKeptCoeffs nonzero coeffs, i.e. we get
  // 3 * 60 * sizeof int
  int usedSpace = 6 * 16 * size
    + 3 * images.size () * m_nKeptCoeffs * sizeof (int);
  std::cout << "allocated "
	    << usedSpace << " bytes, that is "
	    << usedSpace / 1024 << " kilobytes." << std::endl;

  std::cout << "filling them..." << std::endl;
  int id;
  for (DbImageConstIterator it = images.begin (); it != images.end (); ++it)
    {
      id = (*it)->getId ();
      addImageFeatureVector (id, (*it)->getFeaturesYPlus (), m_positiveY);
      addImageFeatureVector (id, (*it)->getFeaturesYMinus (), m_negativeY);
      addImageFeatureVector (id, (*it)->getFeaturesUPlus (), m_positiveU);
      addImageFeatureVector (id, (*it)->getFeaturesUMinus (), m_negativeU);
      addImageFeatureVector (id, (*it)->getFeaturesVPlus (), m_positiveV);
      addImageFeatureVector (id, (*it)->getFeaturesVMinus (), m_negativeV);
    }
  std::cout << "done." << std::endl;
  int elapsed = (int)(timer.elapsed () * 1000);
  std::cout << "creating the PreSelectScoreTable for all " << m_nImages
	    << " images took " << elapsed << " milliseconds." << std::endl;

}

void
PreSelectScoreTable::addImageFeatureVector (int id, const Features &src,
					    IdListList &dest)
{
  int size = m_rows * m_cols;
  int srcByte, srcBit;

  for (int i = 0; i < size; ++i)
    {
      srcByte = i / 8;
      srcBit = i % 8;
      assert (srcByte < src.size ());
      if (IS_BIT_SET (src[srcByte], srcBit))
	{
	  dest[i].push_back (id);
	}
    }
}

PreSelectScoreTable::~PreSelectScoreTable (void)
{
}

void
PreSelectScoreTable::p_query (ImageInformation &qY, ImageInformation &qU,
			      ImageInformation &qV, ImageScoreList &scores)
{

  std::cout << "p_query" << std::endl;
  double avgY = qY.at(0).val ();
  double avgU = qU.at(0).val ();
  double avgV = qV.at(0).val ();
  int difference;
  for (int i = 0; i < scores.size (); ++i)
    {
      difference = (int)(avgY - m_averageY[i]);
      scores[i].addToScore (m_weightY[0] * (::abs (difference)));
      difference = (int)(avgU - m_averageU[i]);
      scores[i].addToScore (m_weightU[0] * (::abs (difference)));
      difference = (int)(avgV - m_averageV[i]);
      scores[i].addToScore (m_weightV[0] * (::abs (difference)));
    }

  querySingleColor (qY, scores, m_positiveY, m_negativeY, m_weightY);
  querySingleColor (qU, scores, m_positiveU, m_negativeU, m_weightU);
  querySingleColor (qV, scores, m_positiveV, m_negativeV, m_weightV);
}

void
PreSelectScoreTable::querySingleColor (ImageInformation &truncated,
				       ImageScoreList &scores,
				       IdListList &positives,
				       IdListList &negatives,
				       const float weights[])
{
  CoeffInformation ci;
  IdListIterator it;
  IdListIterator end;
  int pos;

  for (int j = 1; j < truncated.size (); ++j)
    {

      ci = truncated.at(j);
      pos = ci.xpos () * ci.ypos ();
      it = ci.val () > 0? positives[pos].begin () : negatives[pos].begin ();
      end = ci.val () > 0? positives[pos].end () : negatives[pos].end ();

      for (; it != end; ++it)
	{
	  scores[*it].subFromScore (weights[bin (ci.ypos (), ci.xpos ())]);
	}

    }
}

