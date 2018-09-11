#include "omp.h"

#include "emmintrin.h"
#include "nmmintrin.h"
#include "immintrin.h"


#include "defs.h"

#define UNROLL 1

void conv_filter_avx(int imgHeight, int imgWidth, int imgWidthF,
				 short *filter, unsigned char *imgSrcExt, unsigned char *imgDst)
{
	// Az együtthatókból generálunk vektor tömböt
	__m256i filter_laplace[25];
	for (int i = 0; i < 25; i++)
	{
		filter_laplace[i] = _mm256_set1_epi16(filter[i]);
	}

	// Végiglépkedünk a kimeneti kép sorain
	for (int row=0; row<imgHeight; row++)
	{
		// Végiglépkedünk a kimeneti kép komponensein
		//    Short feldolgozás esetén egy AVX regiszter 256/16=16 komponenst tartalmaz
		for (int col=0; col<(imgWidth*3); col=col+16)
		{
			// Akkumulátor 0-zása
			__m256i fval;
			fval = _mm256_set1_epi16(0);

			// Konvolúció
			for (int fy=0; fy<FILTER_H; fy++)
			{
				for (int fx=0; fx<FILTER_W; fx++)
				{
					int pixel_src = rd_base + rd_offset;
					__m128i load_data;
					__m256i conv_res, mul_res;

				}
			}

			// Szaturáció a konvolúció eredményén 0...255 tartományra
			

			// Kiírás pointer cast-olással
			__m256i *res_vec_ptr;
			res_vec_ptr = &fval;
			short *res_short_ptr;
			res_short_ptr = (short*)(res_vec_ptr);

			int wr_base;	// A kiírandó 16 komponens báziscíme
			// Kiírás
			for (int i = 0; i < 16; i++)
			{
				*(imgDst + wr_base + i) = *(res_short_ptr + i);
			}
		}
	}
}
