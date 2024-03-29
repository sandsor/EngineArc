#pragma once

//#include <cstdlib>
//#include <iostream>
//#include <time.h>

//
//float Randfloat(float size, int MaxNumb)
//{
//	srand(time(0));
//
//	for (int i = 0; i < size; i++)
//		return (rand() % MaxNumb);
//}
//
//int RandInt(int size, int MaxNumb)
//{
//	srand(time(0));
//
//	for (int i = 0; i < size; i++)
//		return (rand() % MaxNumb);
// 
//}

// in the func bin lies some code snippets taken from open framework to make the perlin noise smooth
// found at https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/utils/ofNoise.h
//  and     https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/math/ofMath.cpp

#define OFNOISE_FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

//* Permutation table.This is just a random jumble of all numbers 0 - 255,
//* repeated twice to avoid wrapping the index at 255 for each lookup.
//* This needs to be exactly the same for all instances on all platforms,
//* so it's easiest to just keep it as inline explicit data.
//* This also removes the need for any initialisation of this class.

namespace {
    unsigned char perm[512] = { 151,160,137,91,90,15,
     131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
     190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
     88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
     77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
     102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
     135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
     5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
     223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
     129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
     251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
     49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
     138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
     151,160,137,91,90,15,
     131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
     190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
     88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
     77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
     102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
     135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
     5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
     223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
     129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
     251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
     49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
     138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };
}

 inline float  grad2(int hash, float x, float y) {
    int h = hash & 7;      /* Convert low 3 bits of hash code */
    float u = h < 4 ? x : y;  /* into 8 simple gradient directions, */
    float v = h < 4 ? y : x;  /* and compute the dot product with (x,y). */
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

 inline float _slang_library_noise2(float x, float y)
//  An implementation typically involves four steps: coordinate skewing, simplicial subdivision, gradient selection, and kernel summation.
{
   constexpr float F2 = 0.366025403f; /* F2 = 0.5*(sqrt(3.0)-1.0) */
   constexpr float G2 = 0.211324865f; /* G2 = (3.0-Math.sqrt(3.0))/6.0 */

    float n0, n1, n2; /* Noise contributions from the three corners */

    /* Skew the input space to determine which simplex cell we're in */  //******* Step 1 *******
    float s = (x + y) * F2; /* Hairy factor for 2D */
    float xs = x + s;
    float ys = y + s;
    int i = OFNOISE_FASTFLOOR(xs);
    int j = OFNOISE_FASTFLOOR(ys);

    float t = (float)(i + j) * G2;
    float X0 = i - t; /* Unskew the cell origin back to (x,y) space */
    float Y0 = j - t;
    float x0 = x - X0; /* The x,y distances from the cell origin */
    float y0 = y - Y0;

    float x1, y1, x2, y2;
    int ii, jj;
    float t0, t1, t2;

    /* For the 2D case, the simplex shape is an equilateral triangle. */   //******* Step 2 *******
    /* Determine which simplex we are in. */
    int i1, j1; /* Offsets for second (middle) corner of simplex in (i,j) coords */
    if (x0 > y0) { i1 = 1; j1 = 0; } /* lower triangle, XY order: (0,0)->(1,0)->(1,1) */
    else { i1 = 0; j1 = 1; }      /* upper triangle, YX order: (0,0)->(0,1)->(1,1) */

    /* A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and */
    /* a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where */
    /* c = (3-sqrt(3))/6 */

    x1 = x0 - i1 + G2; /* Offsets for middle corner in (x,y) unskewed coords */
    y1 = y0 - j1 + G2;
    x2 = x0 - 1.0f + 2.0f * G2; /* Offsets for last corner in (x,y) unskewed coords */
    y2 = y0 - 1.0f + 2.0f * G2;

    /* Wrap the integer indices at 256, to avoid indexing perm[] out of bounds */
    ii = i % 256;
    jj = j % 256;
    
    /* Calculate the contribution from the three corners */                             //******* Step 3 *******
    t0 = 0.5f - x0 * x0 - y0 * y0;
    if (t0 < 0.0f) n0 = 0.0f;
    else {
        t0 *= t0;
        n0 = t0 * t0 * grad2(perm[ii + perm[jj]], x0, y0); //grad 2 is a gradient calculation to find the steepest points
    }

    t1 = 0.5f - x1 * x1 - y1 * y1;
    if (t1 < 0.0f) n1 = 0.0f;
    else {
        t1 *= t1;
        n1 = t1 * t1 * grad2(perm[ii + i1 + perm[jj + j1]], x1, y1);
    }

    t2 = 0.5f - x2 * x2 - y2 * y2;
    if (t2 < 0.0f) n2 = 0.0f;
    else {
        t2 *= t2;
        n2 = t2 * t2 * grad2(perm[ii + 1 + perm[jj + 1]], x2, y2);
    }

    /* Add contributions from each corner to get the final noise value. */
    /* The result is scaled to return values in the interval [-1,1]. */
    return 40.0f * (n0 + n1 + n2); /* TODO: The scale factor is preliminary! */
}
inline float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

     if (fabs(inputMin - inputMax) < FLT_EPSILON) {  // flt_epsilon is the smallest float point, fabs is an absolute value
         return outputMin;
     }
     else {
         float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

         if (clamp) {
             if (outputMax < outputMin) {
                 if (outVal < outputMax)outVal = outputMax;
                 else if (outVal > outputMin)outVal = outputMin;
             }
             else {
                 if (outVal > outputMax)outVal = outputMax;
                 else if (outVal < outputMin)outVal = outputMin;
             }
         }
         return outVal; // the value returned is between outputmin and outputmax, the diffrence is equal to the difrence x has to inputmin and inputmax
     }                  // for example x= 1, mininput = 0, maxinput = 10  x= 1/10 || x= mininput/maxinput
                        // minoutput is equal 1 and maxoutput is equal 2 then outvalue will be 1.1
 }

 inline float ofNoise(float x, float y) {
     return _slang_library_noise2(x, y) * 0.5f + 0.5f; //the floats here makes it more leveled when returning (not necessary)
 }

