#include "ScoreTableFactory.h"
#include "BitArrayScoreTable.h"
#include "PreSelectScoreTable.h"

using namespace ImageSearch;

ScoreTable*
ScoreTableFactory::create (int rows, int cols, int nKeptCoeffs,
			   const DbImageList &images, Strategy strategy)
{
  if (strategy == BAD)
    {
      return new BitArrayScoreTable (rows, cols, nKeptCoeffs, images);
    }
  return new PreSelectScoreTable (rows, cols, nKeptCoeffs, images);
}

