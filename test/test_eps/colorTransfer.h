#pragma once
#include<stdio.h>
#include<math.h>
//perform a biased(non-linear) interpolation between values A and B
//using t as the interpolatation factor
//reference Paint Inspired Color Compositing
float cubicInt(float t, float A, float B)
{
	float weight = t*t*(3 - 2 * t);
	return A + weight*(B - A);
}
//Given RYB values iR,iY,iB,return RGB value oR,oG,oB
void RYBtoRGB_(float iR, float iY, float iB, float* oR, float *oG, float *oB)
{
	float x0, x1, x2, x3, y0, y1;
	//red
	x0 = cubicInt(iB, 1.0f, 0.163f);
	x1 = cubicInt(iB, 1.0f, 0.0f);
	x2 = cubicInt(iB, 1.0f, 0.5f);
	x3 = cubicInt(iB, 1.0f, 0.2f);
	y0 = cubicInt(iY, x0, x1);
	y1 = cubicInt(iY, x2, x3);
	*oR = cubicInt(iR, y0, y1);

	//green
	x0 = cubicInt(iB, 1.0f, 0.373f);
	x1 = cubicInt(iB, 1.0f, 0.66f);
	x2 = cubicInt(iB, 0.0f, 0.0f);
	x3 = cubicInt(iB, 0.5f, 0.094f);
	y0 = cubicInt(iY, x0, x1);
	y1 = cubicInt(iY, x2, x3);
	*oG = cubicInt(iR, y0, y1);

	//blue
	x0 = cubicInt(iB, 1.0f, 0.6f);
	x1 = cubicInt(iB, 0.0f, 0.2f);
	x2 = cubicInt(iB, 0.0f, 0.5f);
	x3 = cubicInt(iB, 0.0f, 0.0f);
	y0 = cubicInt(iY, x0, x1);
	y1 = cubicInt(iY, x2, x3);
	*oB = cubicInt(iR, y0, y1);
}

// a simple program is used to convert RGB to CMYK 
//reference https://www.codeproject.com/Articles/4488/XCmyk-CMYK-to-RGB-Calculator-with-source-code
/*
Black   = minimum(1-Red,1-Green,1-Blue)
Cyan    = (1-Red-Black)/(1-Black)
Magenta = (1-Green-Black)/(1-Black)
Yellow  = (1-Blue-Black)/(1-Black)
they are between 0 and 1.0
*/
float minValue(float a, float b)
{
	if (a > b)
		return b;
	else
		return a;
}
float maxValue(float a, float b)
{
	if (a > b)
		return a;
	else
		return b;
}
void RGBtoCMY(float iR, float iG, float iB, float *oC, float* oM, float *oY)
{
	*oC = 1 - iR;
	*oM = 1 - iG;
	*oY = 1 - iB;
	
}
void RGBtoCMYK(float iR, float iG, float iB, float *oC, float* oM, float *oY, float* oK)
{

	*oK = minValue(minValue(1 - iR, 1 - iG), 1 - iB);
	*oC = (1 - iR - *oK) / (1 - *oK);
	*oM = (1 - iG - *oK) / (1 - *oK);
	*oY = (1 - iB - *oK) / (1 - *oK);
}
//a simple process 
//reference https://blog.csdn.net/jiushidt/article/details/43309111
//or inverse derived formula in RGBtoCMYK
//Ranges iC,iM,iY,iK[0 1],
/*
R = (1 - C)*(1 - K)£»
G = (1 - M)*(1 - K)£»
B = (1 - Y)*(1 - K)£»
*/
void CMYKtoRGB(float iC, float iM, float iY, float iK, float *oR, float *oG, float *oB)
{
	*oR = (1 - iK)*(1 - iC);
	*oG = (1 - iK)*(1 - iM);
	*oB = (1 - iK)*(1 - iY);
}

//Given RGB values iR,iG,iB,return RYB value oR,oY,oB
//reference Paint-like Compositing Based on RYB Color Model. SIGGRAPH '15
void RGBtoRYB(float iR, float iG, float iB, float *oR, float *oY, float *oB)
{
	//I_w is a white component in rgb space
	//n is normalized
	//I_b is a black component
	float I_w, n, I_b;
	float r, y, b;
	float ir, ig, ib;
	I_w = minValue(minValue(iR, iG), iB);

	ir = iR - I_w;
	ig = iG - I_w;
	ib = iB - I_w;

	r = ir - minValue(ir, ig);
	y = (ig + minValue(ir, ig)) / 2;
	b = (ib + ig - minValue(ir, ig)) / 2;

	n = maxValue(maxValue(r, y), b) / maxValue(maxValue(maxValue(ir, ig), ib), 0.000001);;

	r = r / (n + 0.000001);
	y = y / (n + 0.000001);
	b = b / (n + 0.000001);

	I_b = minValue(minValue(1 - iR, 1 - iG), 1 - iB);

	*oR = r + I_b;
	*oY = y + I_b;
	*oB = b + I_b;

}

//Given RYB values iR,iY,iB,return RGB value oR,oG,oB
//reference Paint-like Compositing Based on RYB Color Model. SIGGRAPH '15
void RYBtoRGB(float iR, float iY, float iB, float *oR, float *oG, float *oB)
{
	//I_w is a white component in rgb space
	//n is normalized
	//I_b is a black component
	float I_w, n, I_b;
	float r, g, b;
	float ir, iy, ib;
	I_b = minValue(minValue(iR, iY), iB);

	ir = iR - I_b;
	iy = iY - I_b;
	ib = iB - I_b;

	r = ir + iy - minValue(iy, ib);
	g = (iy + 2 * minValue(iy, ib));
	b = 2 * (ib - minValue(iy, ib));

	n = maxValue(maxValue(r, g), b) / maxValue(maxValue(ir, iy), ib);

	r = r / n;
	g = g / n;
	b = b / n;

	I_w = minValue(minValue(1 - iR, 1 - iY), 1 - iB);

	*oR = r + I_w;
	*oG = g + I_w;
	*oB = b + I_w;

}


//Given RGB values iR,iG,iB,return RYBK value oR,oY,oB
//reference Paint-like Compositing Based on RYB Color Model. SIGGRAPH '15
void RGBtoRYBK(float iR, float iG, float iB, float *oR, float *oY, float *oB, float *oK)
{
	//I_w is a white component in rgb space
	//n is normalized
	//I_b is a black component
	float I_w, n, I_b;
	float r, y, b;
	float ir, ig, ib;
	float tmp;
	I_w = minValue(minValue(iR, iG), iB);

	ir = iR - I_w;
	ig = iG - I_w;
	ib = iB - I_w;

	r = ir - minValue(ir, ig);
	y = (ig + minValue(ir, ig)) / 2;
	b = (ib + ig - minValue(ir, ig)) / 2;

	//n = maxValue(maxValue(r, y), b) / maxValue(maxValue(ir, ig), ib);
	n = maxValue(maxValue(r, y), b) / maxValue(maxValue(maxValue(ir, ig), ib), 0.000001);
	r = r / (n + 0.000001);
	y = y / (n + 0.000001);
	b = b / (n + 0.000001);

	I_b = minValue(minValue(1 - iR, 1 - iG), 1 - iB);

	*oR = r + I_b;
	*oY = y + I_b;
	*oB = b + I_b;

	*oK = minValue(minValue(*oR, *oY), *oB);
	//*oK = 1- (0.30*iR + 0.59*iG + 0.11*iB);
	//*oK = 1 - tmp;
	*oR = (*oR - *oK)/ ( 1- *oK);
	*oY = (*oY - *oK) / ( 1-*oK);
	*oB = (*oB - *oK)/ (  1-*oK);
	/*
	if (tmp > 1)
	{
		*oR = *oR / tmp;
		*oY = *oY / tmp;
		*oB = *oB / tmp;
		*oK = 0.0;
	}
	else
	{
		*oK = 1.0 - tmp;
	}*/
	
}

void  RYBKtoRGB(float iR, float iY, float iB, float iK, float *oR, float *oG, float *oB)
{
	RYBtoRGB(iR, iY, iB, oR, oG, oB);
}