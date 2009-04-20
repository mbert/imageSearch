#include "ScoreTableFactory.h"
#include "BitArrayScoreTable.h"

using namespace ImageSearch;

ScoreTable*
ScoreTableFactory::create (int rows, int cols, int nKeptCoeffs,
			   const DbImageList &images)
{
  return new BitArrayScoreTable (rows, cols, nKeptCoeffs, images);
}

