#include<time.h>
#include<iostream>
#include<cmath>
#include"colorStippling.h"
#include"simple_svg_1.0.0.hpp"
#include"simpletime.h"

#define N 6
using namespace svg;
using namespace std;


float maxValue_(float a, float b)
{
	if (a < b)
		return b;
	return a ;
}

/*
int findMaxColor(float c, float m, float y, float k)
{
	float maxv = 0.0;
	int flag;
	maxv = maxValue(maxValue(maxValue(c, m),y),k);
	if (maxv == c)
		return 1;
	if (maxv == m)
		return 2;
	if (maxv == y)
		return 3;
	if (maxv == k)
		return 4;
}*/
void main()
{
	srand((unsigned)time(0));
	SImageData imageData;
	char fileName[] = "E:/demo/xiula2048.bmp";
	
	ImageLoad(fileName, imageData);
	
	#if N==0//================按小到大的顺序来=================================
	{
		float r, g, b;
		float c_, m_, y_, k_;
		std::cout << rand() % 100 << std::endl;
		std::cout << rand() % 100 << std::endl;
		epswriter c("circle.eps", 0, 0, imageData.m_width * 2, imageData.m_height * 2,0);
		for (size_t y = 0; y < imageData.m_height; ++y)
		{
			SColor* pixel = (SColor*)&imageData.m_pixels[y * imageData.m_pitch];
			//把R通道
			for (size_t x = 0; x <imageData.m_width; ++x)
			{
				//g_value是吧所有的都加起来？？？
				r = (double)pixel->R / 255.0;
				g = (double)pixel->G / 255.0;
				b = (double)pixel->B / 255.0;
				RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
				if (acceptColor(c_) == 1)
				{
					c.filledCircle(int(x * 2), int(y * 2), 0.5, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (acceptColor(m_) == 1)
				{
					c.filledCircle(int(x * 2+1), int(y * 2), 0.5, (0) * 65535, (1) * 65535, (0) * 65535, (0) * 65535);
				}
				if (acceptColor(y_) == 1)
				{
					c.filledCircle(int(x * 2 ), int(y * 2+1), 0.5, (0) * 65535, (0) * 65535, (1) * 65535, (0) * 65535);
				}
				if (acceptColor(k_) == 1)
				{
					c.filledCircle(int(x * 2 + 1), int(y * 2+1), 0.5, (0) * 65535, (0) * 65535, (0) * 65535, (1) * 65535);
				}

				++pixel;
			
			}
		}

	}
	#elif N == 1  //按ivs的顺序来
	{
		//对于CMYK的通道来说，图片最后要扩大到原来的四倍
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		size_t o_w = i_w * 8;
		size_t o_h = i_h * 8;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = o_w * o_h;
		
		for (int i = 0; i < numseed/2; i++)
		{
			double x = ivs[i][0];
			double y = ivs[i][1];
			size_t k = x * i_w;
			size_t t = y * i_h;
			SColor& destDitherPixel = imageData.GetPixel(k, t);
			r = (float)destDitherPixel.R / 255.0;
			g = (float)destDitherPixel.G / 255.0;
			b = (float)destDitherPixel.B / 255.0;
			
			RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
			//std::cout << "test" << std::endl;
			/*
			if (i < numseed/4)
			{
				if (acceptColor(c_) == 1)
				{
					c.filledCircle(int(x * o_w), int(y * o_h),0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
			}
			else if (i < 2*numseed/4)
			{
				if (acceptColor(m_) == 1)
				{
					c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (1) * 65535, (0) * 65535, (0) * 65535);
				}
			}
			else if (i < 3 * numseed/4)
			{
				if (acceptColor(y_) == 1)
				{
					c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (0) * 65535, (1) * 65535, (0) * 65535);
				}
			}
			else if (i < 4 * numseed/4)
			{
				if (acceptColor(k_) == 1)
				{
					c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (0) * 65535, (0) * 65535, (1) * 65535);
				}
			}
			}*/
			if (acceptColor(c_) == 1)
			{
				c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
			}
			if (acceptColor(m_) == 1)
			{
				c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (1) * 65535, (0) * 65535, (0) * 65535);
			}
			if (acceptColor(y_) == 1)
			{
				c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (0) * 65535, (1) * 65535, (0) * 65535);
			}
			if (acceptColor(k_) == 1)
			{
				c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (0) * 65535, (0) * 65535, (1) * 65535);
			}
			
		}
		
				
	}
#elif N == 2  //按ivs的顺序来
	{
		//对于CMYK的通道来说，图片最后要扩大到原来的四倍
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		size_t o_w = i_w * 8;
		size_t o_h = i_h * 8;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(o_w,o_h);
		Document doc("my_svg.svg", Layout(dimensions, Layout::BottomLeft));
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = o_w * o_h;

		for (int i = 0; i < numseed / 2; i++)
		{
			double x = ivs[i][0];
			double y = ivs[i][1];
			size_t k = x * i_w;
			size_t t = y * i_h;
			SColor& destDitherPixel = imageData.GetPixel(k, t);
			r = (float)destDitherPixel.R / 255.0;
			g = (float)destDitherPixel.G / 255.0;
			b = (float)destDitherPixel.B / 255.0;

			RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
			//std::cout << "test" << std::endl;
			/*
			if (i < numseed/4)
			{
			if (acceptColor(c_) == 1)
			{
			c.filledCircle(int(x * o_w), int(y * o_h),0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
			}
			}
			else if (i < 2*numseed/4)
			{
			if (acceptColor(m_) == 1)
			{
			c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (1) * 65535, (0) * 65535, (0) * 65535);
			}
			}
			else if (i < 3 * numseed/4)
			{
			if (acceptColor(y_) == 1)
			{
			c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (0) * 65535, (1) * 65535, (0) * 65535);
			}
			}
			else if (i < 4 * numseed/4)
			{
			if (acceptColor(k_) == 1)
			{
			c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (0) * 65535, (0) * 65535, (0) * 65535, (1) * 65535);
			}
			}
			}
			
			float max_= 0.0;
			max_ = maxValue_(maxValue_(maxValue_(c_*acceptColor(c_), m_*acceptColor(m_)),y_*acceptColor(y_)),k_*acceptColor(k_));
			//int flag;
			//flag = findMaxColor(acceptColor(c_)*c_, acceptColor(m_)*m_, acceptColor(y_)*y_, acceptColor(k_)*k_);
			//int flagc = 0,flagm = 0,flagy = 0,flagk = 0;
			*/
			if (acceptColor(c_) == 1)
			{
				
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 2, Fill(Color(0, 255, 255)), Stroke(0, Color(255, 255, 255)));
				//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
			}
			if (acceptColor(m_) == 1)
			{
				
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 2, Fill(Color(255, 0, 255)), Stroke(0, Color(255, 255, 255)));
			}
			if (acceptColor(y_) == 1)
			{
				
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 2, Fill(Color(255, 255, 0)), Stroke(0, Color(255, 255, 255)));
			}
			if (acceptColor(k_) == 1)
			{
				
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 2, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
			}
			
			

		}

		doc.save();
	}
	#elif N == 3
	{
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		size_t o_w = i_w * 1;
		size_t o_h = i_h * 1;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(i_w, i_h);
		Document doc("my_svg.svg", Layout(dimensions, Layout::BottomLeft));
		doc << svg::Rectangle(Point(0, i_h), i_w, i_h, Color::Black);
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = o_w * o_h;

		for (int i = 0; i < numseed/2; i++)
		{
			double x = ivs[i][0];
			double y = ivs[i][1];
			size_t k = x * i_w;
			size_t t = y * i_h;
			SColor& destDitherPixel = imageData.GetPixel(k, t);
			r = (float)destDitherPixel.R / 255.0;
			g = (float)destDitherPixel.G / 255.0;
			b = (float)destDitherPixel.B / 255.0;

			//RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
			int flag_r, flag_g, flag_b;
			flag_r = acceptColor(r);
			flag_g = acceptColor(g);
			flag_b = acceptColor(b);
			float max_v = 0.0;
			
			if (flag_r == 1 && flag_g == 1 && flag_b == 1)
			{
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
			}
			else
			{
				/*
				max_v = max(max(r*flag_r, g*flag_g), b*flag_b);
				//max_v = std::max(std::max(float(r*flag_r), float(g*flag_g)), b*flag_b);
				if(max_v == r)
					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
				else if(max_v == g)
					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				else
					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				*/
				
				if (flag_r == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
					//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (flag_g == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_b == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}
			}
			



		}

		doc.save();
	}
	#elif N == 4
	{
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		size_t o_w = i_w * 8;
		size_t o_h = i_h * 8;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(o_w, o_h);
		Document doc("my_svg.svg", Layout(dimensions, Layout::BottomLeft));
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = o_w * o_h;
		float r_r, g_r, b_r;
		float r_y, g_y, b_y;
		float r_b, g_b, b_b;
		RYBtoRGB(1, 0, 0, &r_r, &g_r, &b_r);
		RYBtoRGB(0, 1, 0, &r_y, &g_y, &b_y);
		RYBtoRGB(0, 0, 1, &r_b, &g_b, &b_b);
		for (int i = 0; i < numseed / 2; i++)
		{
			double x = ivs[i][0];
			double y = ivs[i][1];
			size_t k = x * i_w;
			size_t t = y * i_h;
			SColor& destDitherPixel = imageData.GetPixel(k, t);
			r = (float)destDitherPixel.R / 255.0;
			g = (float)destDitherPixel.G / 255.0;
			b = (float)destDitherPixel.B / 255.0;
			float r_, y_, b_;
			RGBtoRYB(r, g, b, &r_, &y_, &b_);
			//RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
			int flag_r, flag_y, flag_b;
			flag_r = acceptColor(r_);
			flag_y = acceptColor(y_);
			flag_b = acceptColor(b_);
			if (flag_r == 1 && flag_y == 1 && flag_b == 1)
			{
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 2, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
			}
			else
			{
				if (flag_r == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 2, Fill(Color(int(r_r*255), int(g_r)*255, int(b_r*255))), Stroke(0, Color(255, 255, 255)));
					//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (flag_y == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 2, Fill(Color(int(r_y * 255), int(g_y) * 255, int(b_y * 255))), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_b == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 2, Fill(Color(int(r_b * 255), int(g_b) * 255, int(b_b * 255))), Stroke(0, Color(255, 255, 255)));
				}
			}




		}

		doc.save();
	}
	#elif N == 5
	{
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		float ratio = 8;
		size_t o_w = i_w *ratio;
		size_t o_h = i_h *ratio;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(o_w, o_h);
		Document doc("my_svgrgb.svg", Layout(dimensions, Layout::BottomLeft));
		
		doc << svg::Rectangle(Point(0, o_h), o_w, o_h, Color::White);
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = i_w * i_h-3;
		vector<glm::vec4> vRej;
		vRej.resize(int(numseed));
		float eps_time;
		simpletime::start();
		MLISMultiThread(imageData,  vRej, i_w, i_h, 8);
		eps_time = simpletime::milliseconds();
		cout << eps_time << endl;
		cout << count_num << endl;
		//cout << vRej[0].z << endl;
		for (int i = 0; i < vRej.size(); i++)
		{
			
			if(vRej[i].a == 1)//怎么在svg上获取背景颜色
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
		}
		/*
		for (size_t y = 0; y < imageData.m_height; ++y)
		{
			SColor* pixel = (SColor*)&imageData.m_pixels[y * imageData.m_pitch];
			for (size_t x = 0; x < imageData.m_width; ++x)
			{
				r = (double)pixel->R;
				g = (double)pixel->G;
				b = (double)pixel->B;
				doc << Circle(Point(int(x), int(y)), 1.2, Fill(Color(r, g, b)), Stroke(0, Color(255, 255, 255)));
				++pixel;

			}
		}*/
		/*
		for (int i = 0; i < numseed / 2; i++)
		{
			double x = ivs[i][0];
			double y = ivs[i][1];
			size_t k = x * i_w;
			size_t t = y * i_h;
			SColor& destDitherPixel = imageData.GetPixel(k, t);
			r = (float)destDitherPixel.R / 255.0;
			g = (float)destDitherPixel.G / 255.0;
			b = (float)destDitherPixel.B / 255.0;

			//RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
			int flag_r, flag_g, flag_b;
			flag_r = acceptColor(r);
			flag_g = acceptColor(g);
			flag_b = acceptColor(b);
			float max_v = 0.0;

			if (flag_r == 1 && flag_g == 1 && flag_b == 1)
			{
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
			}
			else
			{
				
				max_v = max(max(r*flag_r, g*flag_g), b*flag_b);
				//max_v = std::max(std::max(float(r*flag_r), float(g*flag_g)), b*flag_b);
				if(max_v == r)
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
				else if(max_v == g)
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				else
				doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				

				if (flag_r == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
					//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (flag_g == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_b == 1)
				{

					doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}
			}




		}*/

		doc.save();
	}
	#elif N == 6
	{
		
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		float ratio = 8;
		size_t o_w = i_w *ratio;
		size_t o_h = i_h *ratio;
		//std::cout << int(i_w) << std::endl;
		//std::cout << int(i_h) << std::endl;
		//increSet();
		
		Dimensions dimensions(o_w, o_h);
		Document doc("my_svgcmyk.svg", Layout(dimensions, Layout::BottomLeft));
		doc << svg::Rectangle(Point(0, o_h), o_w, o_h, Color::White);
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = (i_w * i_h-3)/4;
		vector<glm::vec4> vRej;
		vRej.resize(int(numseed));
		//cout << vRej.size() << endl;
		float eps_time;
		simpletime::start();
		MLISMultiThreadCMYK(imageData, vRej, i_w/2, i_h/2, 8);
		eps_time = simpletime::milliseconds();
		std::cout << eps_time << std::endl;
		
		//cout << vRej[0].z << endl;
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 5)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 6)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 7)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
			
			/*
			if (vRej[i].a == 5)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 6)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 7)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
			*/
			
		}
		vRej.clear();
		vRej.resize(int(numseed));
		MLISMultiThreadCMYK2(imageData, vRej, i_w / 2, i_h / 2, 8);
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 5)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 6)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 7)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
		}
		vRej.clear();
		vRej.resize(int(numseed));
		MLISMultiThreadCMYK3(imageData, vRej, i_w / 2, i_h / 2, 8);
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 5)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 6)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 7)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
		}
		vRej.clear();
		vRej.resize(int(numseed));
		MLISMultiThreadCMYK4(imageData, vRej, i_w / 2, i_h / 2, 8);
		std::cout << count_num << std::endl;
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 5)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 6)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 7)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*ratio, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
		}
		doc.save();
	}
	#elif N == 7
	{
		
		float r, g, b;
		float eps_time;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		int output_ri = 8;
		size_t o_w = i_w*output_ri;
		size_t o_h = i_h*output_ri;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(o_w, o_h);
		Document doc("my_svgrybk.svg", Layout(dimensions, Layout::BottomLeft));
		doc << svg::Rectangle(Point(0, o_h), o_w, o_h, Color::White);
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = i_w * i_h-3;
		vector<glm::vec4> vRej;
		vRej.resize(int(numseed));
		simpletime::start();
		MLISMultiThreadRYBK(imageData, vRej, i_w, i_h, 8);
		eps_time = simpletime::milliseconds();
		std::cout << eps_time << std::endl;
		std::cout << count_num << std::endl;
		//cout << vRej[0].z << endl;
		//===========颜色转化==================
		/*
		float r_r, r_g, r_b, y_r, y_g, y_b, b_r, b_g, b_b;
		RYBtoRGB(1.0, 0, 0, &r_r, &r_g, &r_b);
		RYBtoRGB(0, 1.0, 0, &y_r, &y_g, &y_b);
		RYBtoRGB(0, 0, 1.0, &b_r, &b_g, &b_b);
		*/
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*output_ri, Fill(Color(255, 0, 0)), Stroke(0, Color(0, 0, 0)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*output_ri, Fill(Color(230, 227,5)), Stroke(0, Color(0, 0, 0)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*output_ri, Fill(Color(0, 0, 255)), Stroke(0, Color(0, 0, 0)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z*output_ri, Fill(Color(0, 0, 0)), Stroke(0, Color(0, 0, 0)));
			/*
			if (vRej[i].a == 5)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 165, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 6)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 7)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 0, 255)), Stroke(0, Color(255, 255, 255)));
			*/
		}
		doc.save();
	}
	#elif N == 8//varyradius rgb
	{
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		size_t o_w = i_w * 1;
		size_t o_h = i_h * 1;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(i_w, i_h);
		Document doc("my_svgrgb.svg", Layout(dimensions, Layout::BottomLeft));
		doc << svg::Rectangle(Point(0, i_h), i_w, i_h, Color::Black);
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = o_w * o_h;
		vector<glm::vec4> vRej;
		vRej.resize(int(numseed/2));
		MLISMultiThreadVaryRadius(imageData, vRej, i_w, i_h, 6);
		//cout << vRej[0].z << endl;
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
		}
		/*
		for (int i = 0; i < numseed / 2; i++)
		{
		double x = ivs[i][0];
		double y = ivs[i][1];
		size_t k = x * i_w;
		size_t t = y * i_h;
		SColor& destDitherPixel = imageData.GetPixel(k, t);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;

		//RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
		int flag_r, flag_g, flag_b;
		flag_r = acceptColor(r);
		flag_g = acceptColor(g);
		flag_b = acceptColor(b);
		float max_v = 0.0;

		if (flag_r == 1 && flag_g == 1 && flag_b == 1)
		{
		doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
		}
		else
		{

		max_v = max(max(r*flag_r, g*flag_g), b*flag_b);
		//max_v = std::max(std::max(float(r*flag_r), float(g*flag_g)), b*flag_b);
		if(max_v == r)
		doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
		else if(max_v == g)
		doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
		else
		doc << Circle(Point(int(x * o_w), int(y * o_h)), 1, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));


		if (flag_r == 1)
		{

		doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
		//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
		}
		if (flag_g == 1)
		{

		doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
		}
		if (flag_b == 1)
		{

		doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
		}
		}




		}*/

		doc.save();
	}
	#elif N == 9
	{
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		size_t o_w = i_w * 1;
		size_t o_h = i_h * 1;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(o_w, o_h);
		Document doc("my_svgcmyk.svg", Layout(dimensions, Layout::BottomLeft));
		doc << svg::Rectangle(Point(0, o_h), o_w, o_h, Color::White);
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = o_w * o_h;
		vector<glm::vec4> vRej;
		vRej.resize(int(numseed));
		MLISMultiThreadCMYKVaryRaidus(imageData, vRej, i_w, i_h, 6);
		//cout << vRej[0].z << endl;
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 255, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 0, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
		}
		doc.save();
	}
	#elif N==10
	{
		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		size_t o_w = i_w * 1;
		size_t o_h = i_h * 1;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(o_w, o_h);
		Document doc("my_svgrgb.svg", Layout(dimensions, Layout::BottomLeft));
		doc << svg::Rectangle(Point(0, o_h), o_w, o_h, Color::Black);
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = o_w * o_h-3;
		vector<glm::vec4> vRej;
		vRej.resize(int(numseed));
		MLISMultiThreadVaryRadius2(imageData, vRej, i_w, i_h, 1);
		//cout << vRej[0].z << endl;
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
		}
		doc.save();
	}
	#elif N==11
	{

		float r, g, b;
		float c_, m_, y_, k_;
		size_t i_w = imageData.m_width;
		size_t i_h = imageData.m_height;
		int raito = 8;
		size_t o_w = i_w *raito;
		size_t o_h = i_h  *raito;
		//std::cout << rand() % 100 << std::endl;
		//std::cout << rand() % 100 << std::endl;
		Dimensions dimensions(o_w, o_h);
		Document doc("my_svgrybk.svg", Layout(dimensions, Layout::BottomLeft));
		doc << svg::Rectangle(Point(0, o_h), o_w, o_h, Color::White);
		//epswriter c("circle.eps", 0, 0, o_w, o_h, 0);
		int numseed = i_w * i_h-3;
		vector<glm::vec4> vRej;
		vRej.resize(int(numseed));
		MLISMultiThreadRYBKVaryRaidus(imageData, vRej, i_w, i_h, 6);
		//cout << vRej[0].z << endl;
		//===========颜色转化==================
		float r_r, r_g, r_b, y_r, y_g, y_b, b_r, b_g, b_b;
		RYBtoRGB(1.0, 0, 0, &r_r, &r_g, &r_b);
		RYBtoRGB(0, 1.0, 0, &y_r, &y_g, &y_b);
		RYBtoRGB(0, 0, 1.0, &b_r, &b_g, &b_b);
		for (int i = 0; i < vRej.size(); i++)
		{
			if (vRej[i].a == 1)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z *raito, Fill(Color(int(255), int(0), int(0))), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 2)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z *raito, Fill(Color(int(255), int(255), int(0))), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 3)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z *raito, Fill(Color(int(0), int(0), int(255))), Stroke(0, Color(255, 255, 255)));
			if (vRej[i].a == 4)
				doc << Circle(Point(int(vRej[i].x * o_w), int(vRej[i].y * o_h)), vRej[i].z *raito, Fill(Color(0, 0, 0)), Stroke(0, Color(255, 255, 255)));
		}
		doc.save();
	}
	#endif
	system("pause");

}