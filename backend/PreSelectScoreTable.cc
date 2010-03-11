#include "PreSelectScoreTable.h"
#include "macros.h"

#include <boost/timer.hpp>

using namespace ImageSearch;

PreSelectScoreTable::PreSelectScoreTable (int rows, int cols, int nKeptCoeffs )
  : ScoreTable (rows, cols, nKeptCoeffs)
{
  int size = rows * cols;
  std::cout << "resizing id-list-lists..." << std::endl;
  m_positiveY.resize (size);
  m_negativeY.resize (size);
  m_positiveU.resize (size);
  m_negativeU.resize (size);
  m_positiveV.resize (size);
  m_negativeV.resize (size);

}

PreSelectScoreTable::~PreSelectScoreTable (void)
{
}

void
PreSelectScoreTable::doLoadImages (const ImageFeaturesList &images)
{
  boost::timer timer;
  ScoreTable::doLoadImages (images);

  // a single vector has about 16 bytes of size
  // each color channel has m_nKeptCoeffs nonzero coeffs, i.e. we get
  // 3 * 60 * sizeof int
  int usedSpace = 6 * 16 * m_rows * m_cols
    + 3 * m_nImages * m_nKeptCoeffs * sizeof (int);
  std::cout << "allocated "
	    << usedSpace << " bytes, that is "
	    << usedSpace / 1024 << " kilobytes." << std::endl;

  std::cout << "filling them..." << std::endl;
  unsigned long id = 0;
  for (ImageFeaturesConstIterator it = images.begin (); it != images.end (); ++it)
    {
      appendImage (id, **it);
      ++id;
    }
  std::cout << "done." << std::endl;
  int elapsed = (int)(timer.elapsed () * 1000);
  std::cout << "creating the PreSelectScoreTable for all " << m_nImages
	    << " images took " << elapsed << " milliseconds." << std::endl;

}

void
PreSelectScoreTable::doAppendImage (const unsigned long id, const ImageFeatures &image)
{
  ScoreTable::doAppendImage (id, image);

  addImageFeatureVector (id, image.getFeaturesYPlus (), m_positiveY);
  addImageFeatureVector (id, image.getFeaturesYMinus (), m_negativeY);
  addImageFeatureVector (id, image.getFeaturesUPlus (), m_positiveU);
  addImageFeatureVector (id, image.getFeaturesUMinus (), m_negativeU);
  addImageFeatureVector (id, image.getFeaturesVPlus (), m_positiveV);
  addImageFeatureVector (id, image.getFeaturesVMinus (), m_negativeV);
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

void
PreSelectScoreTable::p_query (ImageInformation &qY, ImageInformation &qU,
			      ImageInformation &qV, ImageScoreList &scores,
			      bool debug)
{

  double avgY = qY.at(0).val ();
  double avgU = qU.at(0).val ();
  double avgV = qV.at(0).val ();

  int difference;
  for (int i = 0; i < scores.size (); ++i)
    {
      if (debug && (i == 8337 || i == 9995))
	{
	  std::cerr << "average Y: " << m_averageY[i]
		    << " vs. " << avgY << std::endl;
	  std::cerr << "score before Y: "
		    << scores[i].getScore() << std::endl;
	}
      difference = (int)(avgY - m_averageY[i]);
      scores[i].addToScore (m_weightY[0] * (::abs (difference)));
      if (debug && (i == 8337 || i == 9995))
	{
	  std::cerr << "average U: " << m_averageU[i]
		    << " vs. " << avgU << std::endl;
	  std::cerr << "score before U: "
		    << scores[i].getScore() << std::endl;
	}
      difference = (int)(avgU - m_averageU[i]);
      scores[i].addToScore (m_weightU[0] * (::abs (difference)));
      if (debug && (i == 8337 || i == 9995))
	{
	  std::cerr << "average V: " << m_averageV[i]
		    << " vs. " << avgV << std::endl;
	  std::cerr << "score before V: "
		    << scores[i].getScore() << std::endl;
	}
      difference = (int)(avgV - m_averageV[i]);
      scores[i].addToScore (m_weightV[0] * (::abs (difference)));
      if (debug && (i == 8337 || i == 9995))
	{
	  std::cerr << "score after V: "
		    << scores[i].getScore() << std::endl;
	}
    }

  if (debug)
    {
      std::cerr << "score before Y: " << scores[9995].getScore() << " / " << scores[8337].getScore() << std::endl;
    }
  querySingleColor (qY, scores, m_positiveY, m_negativeY, m_weightY, debug);
  if (debug)
    {
      std::cerr << "score before U: " << scores[9995].getScore() << " / " << scores[8337].getScore()  << std::endl;
    }
  querySingleColor (qU, scores, m_positiveU, m_negativeU, m_weightU, debug);
  if (debug)
    {
      std::cerr << "score before V: " << scores[9995].getScore() << " / " << scores[8337].getScore()  << std::endl;
    }
  querySingleColor (qV, scores, m_positiveV, m_negativeV, m_weightV, debug);
  if (debug)
    {
      std::cerr << "score after V: " << scores[9995].getScore() << " / " << scores[8337].getScore()  << std::endl;
    }
}

void
PreSelectScoreTable::querySingleColor (ImageInformation &truncated,
				       ImageScoreList &scores,
				       IdListList &positives,
				       IdListList &negatives,
				       const float weights[], bool debug)
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

