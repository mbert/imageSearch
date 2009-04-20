#include "PreSelectScoreTable.h"


using namespace ImageSearch;

PreSelectScoreTable::PreSelectScoreTable (int rows, int cols, int nKeptCoeffs,
					  const DbImageList &images)
  : ScoreTable (rows, cols, nKeptCoeffs, images)
{
}

PreSelectScoreTable::~PreSelectScoreTable (void)
{
}

void
PreSelectScoreTable::p_query (ImageInformation &qY, ImageInformation &qU,
			      ImageInformation &qV, ImageScoreList &scores)
{

}
