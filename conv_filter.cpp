#include "omp.h"

#include "emmintrin.h"
#include "nmmintrin.h"
#include "immintrin.h"

#include "defs.h"

//#define SIMA_C

#ifdef SIMA_C

void conv_filter(int imgHeight, int imgWidth, int imgWidthF,
				 short *filter, unsigned char *imgSrcExt, unsigned char *imgDst)
{
	// Végiglépkedünk a kimeneti kép sorain
	for (int row=0; row<imgHeight; row++)
	{
		// Végiglépkedünk a kimeneti kép oszlopain
		for (int col=0; col<imgWidth; col++)
		{
			// A 3 komponens akkumulátora
			short fval[3] = {0,0,0};

			// Végiglépkedünk a szűrő által lefedett bemeneti pixelek mindhárom komponensén, konvolválunk
			for (int fy=0; fy<FILTER_H; fy++)
			{
				for (int fx=0; fx<FILTER_W; fx++)
				{
					for (int rgba=0; rgba<3; rgba++)
					{
						fval[rgba] += (filter[fx + fy * FILTER_W] * imgSrcExt[(col + fy + ((row + fx) * imgWidthF)) * 3 + rgba]);
					}
				}
			}

			// Megvizsgáljuk, hogy a konvolúció eredménye benne van-e a 0...255 tartományban,
			//   amit a kimeneti fájlban el tudunk tárolni
			for (int rgba = 0; rgba<3; rgba++)
			{
				fval[rgba] = (fval[rgba] > 255) ? 255 : (fval[rgba] < 0) ? 0 : fval[rgba];
			}

			// Kiírjuk a 3 kompponenst 8 bites unsigned értékként
			for (int rgba=0; rgba<3; rgba++)
			{
				imgDst[(row * imgWidth + col) * 3 + rgba] = fval[rgba];
			}
		}
	}
}

#else

void conv_filter(int imgHeight, int imgWidth, int imgWidthF,
	short *filter, unsigned char *imgSrcExt, unsigned char *imgDst)
{
	// Végiglépkedünk a kimeneti kép sorain
#pragma omp	parallel for
	for (int row = 0; row<imgHeight; row++)
	{
		// Végiglépkedünk a kimeneti kép oszlopain
		for (int col = 0; col<imgWidth; col++)
		{
			// A 3 komponens akkumulátora
			short fval[3] = { 0,0,0 };

			// Végiglépkedünk a szűrő által lefedett bemeneti pixelek mindhárom komponensén, konvolválunk
			for (int fy = 0; fy<FILTER_H; fy++)
			{
				for (int fx = 0; fx<FILTER_W; fx++)
				{
					for (int rgba = 0; rgba<3; rgba++)
					{
						fval[rgba] += (filter[fx + fy * FILTER_W] * imgSrcExt[(col + fy + ((row + fx) * imgWidthF)) * 3 + rgba]);
					}
				}
			}

			// Megvizsgáljuk, hogy a konvolúció eredménye benne van-e a 0...255 tartományban,
			//   amit a kimeneti fájlban el tudunk tárolni
			for (int rgba = 0; rgba<3; rgba++)
			{
				fval[rgba] = (fval[rgba] > 255) ? 255 : (fval[rgba] < 0) ? 0 : fval[rgba];
			}

			// Kiírjuk a 3 kompponenst 8 bites unsigned értékként
			for (int rgba = 0; rgba<3; rgba++)
			{
				imgDst[(row * imgWidth + col) * 3 + rgba] = fval[rgba];
			}
		}
	}
}

#endif //SIMA_C