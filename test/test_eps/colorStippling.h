#pragma once
#include <windows.h> 
#include "epswriter.hpp"
#include "colorTransfer.h"
#include "SImageData.h"
#include<time.h>
#include<vector>
#include<array>
#include<atomic>
#include<thread>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"ivsData.h"
#include<cmath>
//#include"incrementIVS.h"
#define IVS_M_PI	3.14159265358979323846264338327950288
using namespace std;
int count_num = 0;
//=============sigmoid函数====================================
float sigmoid(float x)
{	
	return 1 / (1 + exp(-x));
}

//=============随机数=================================
int acceptColor(float percent)
{
	int rint = (rand() % 100)+1;
	if (rint < 100 * percent)
		return 1;
	else
		return 0;
}
//==================================
//=============deal with RGBcolor=============================
void DealwithColor(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) return;
	float radius = 1.42;//sqrt(float((i_h*i_w)/(n_max*IVS_M_PI)))+0.8;
	float r, g, b,w,k;
	
	float r_new, g_new, b_new;
	float thelta = IVS_M_PI * radius * radius;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{
		int flag = 0;
		vRej[idx].x = ivs[idx][0];
		vRej[idx].y = ivs[idx][1];
		vRej[idx].z = radius;
		int loc_x = int(ivs[idx][0] * i_w);
		int loc_y = int(ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= i_w || loc_y >= i_h)
			return;
		int flag_r=0, flag_g=0, flag_b=0 , flag_k=0;
		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;

		float r_i = 1- r;
		float g_i =  1 - g;
		float b_i =  1- b;
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16/4);
		
		if (r_i >= v_i)
		{	
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
				
			flag_r = 1;
		}
		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_g = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_b = 1;
		}

		/*
		w = minValue(minValue(r, g), b);
		k = 1 - w;
		r_new = (r - w) / (w);
		g_new = (g - w) / (w);
		b_new = (b - w) / (w);
		flag_r = acceptColor(pow(r_new, 1.0));
		flag_g = acceptColor(pow(g_new, 1.0));
		flag_b = acceptColor(pow(b_new, 1.0));
		flag_k = acceptColor(pow(w, 1.0));
		if (flag_r + flag_g + flag_b + flag_k == 4)
		{
			int temp = int(rand());
			if ((temp) % 4 == 0)
				vRej[idx].a = 1;
			else if (temp % 4 == 1)
			{
				vRej[idx].a = 2;
			}
			else if (temp % 4 == 2)
			{
				vRej[idx].a = 3;
			}
			else if (temp % 4 == 3)
			{
				vRej[idx].a = 4;
			}
		}
		if (flag_r + flag_g + flag_b + flag_k == 3)
		{
			if (flag_r == 0)
			{
				int temp = int(rand());
				if (temp % 3 == 0)
					vRej[idx].a = 2;
				else if (temp % 3 == 1)
					vRej[idx].a = 3;
				else if (temp % 3 == 2)
					vRej[idx].a = 4;
			}
			if (flag_g == 0)
			{
				int temp = int(rand());
				if (temp % 3 == 0)
					vRej[idx].a = 1;
				else if (temp % 3 == 1)
					vRej[idx].a = 3;
				else if (temp % 3 == 2)
					vRej[idx].a = 4;
			}
			if (flag_b == 0)
			{
				int temp = int(rand());
				if (temp % 3 == 0)
					vRej[idx].a = 1;
				else if (temp % 3 == 1)
					vRej[idx].a = 2;
				else if (temp % 3 == 2)
					vRej[idx].a = 4;
			}
			if (flag_k == 0)
			{
				int temp = int(rand());
				if (temp % 3 == 0)
					vRej[idx].a = 1;
				else if (temp % 3 == 1)
					vRej[idx].a = 2;
				else if (temp % 3 == 2)
					vRej[idx].a = 3;
			}
		}
		else {
			if (flag_r + flag_g + flag_b + flag_k == 2)
			{
				while (true)
				{
					int temp = (int(rand()));
					if (temp % 4 == 0 && flag_r == 1)
					{
						vRej[idx].a = 1;
						break;
					}
					if (temp % 4 == 1 && flag_g == 1)
					{
						vRej[idx].a = 2;
						break;
					}
					if (temp % 4 == 2 && flag_b == 1)
					{
						vRej[idx].a = 3;
						break;
					}

					if (temp % 4 == 3 && flag_k == 1)
					{
						vRej[idx].a = 4;
						break;
					}
				}
			}
			else {
				if (flag_r == 1)
				{
					vRej[idx].a = 1;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
					//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (flag_g == 1)
				{
					vRej[idx].a = 2;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_b == 1)
				{
					vRej[idx].a = 3;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_k == 1)
				{
					vRej[idx].a = 4;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}

			}
		}
		*/
		/*
		flag_r = acceptColor(pow(r,2.2));
		flag_g = acceptColor(pow(g, 2.2));
		flag_b = acceptColor(pow(b, 2.2));
		*/
		/*
		flag_r = acceptColor(pow(r, 1));
		flag_g = acceptColor(pow(g, 1));
		flag_b = acceptColor(pow(b, 1));
		*/
		float max_v = 0.0;
		
		if (flag_r == 1 && flag_g == 1 && flag_b == 1)
		{
			vRej[idx].a = 1;
			//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
		}
		else
		{
			if (flag_r + flag_g + flag_b == 2)
			{
				if(flag_r == 0)
					vRej[idx].a = 2;
				if (flag_g == 0)
					vRej[idx].a = 3;
				if (flag_b == 0)
					vRej[idx].a = 4;
			}
			
			/*if (flag_r + flag_g + flag_b == 2)
			{
				int temp;
				while (true)
				{
					temp = (int(rand()) % 3);
					if (temp == 0 && flag_r == 1)
					{
						vRej[idx].a = 2;
						break;
					}
					if (temp == 1 && flag_g == 1)
					{
						vRej[idx].a = 3;
						break;
					}
					if (temp == 2 && flag_b == 1)
					{
						vRej[idx].a = 4;
						break;
					}

				}
				
				
			}*/
			else{
				if (flag_r + flag_g + flag_b == 1)
				{
					if (flag_r == 1)
					{
						/*
						if(g_i<=b_i)
							vRej[idx].a = 3;
						else
							vRej[idx].a = 4;*/

						
						int temp = (int(rand()) % 2);
						if(temp%2==0)
							vRej[idx].a = 3;
						else vRej[idx].a = 4;
						

					}
					if (flag_g == 1)
					{
						/*
						if (r_i <= b_i)
							vRej[idx].a = 2;
						else
							vRej[idx].a = 4;*/
						
						int temp = (int(rand()) % 2);
						if (temp % 2 == 0)
							vRej[idx].a = 2;
						else vRej[idx].a = 4;

					}
					if (flag_b == 1)
					{
						/*
						if (r_i <= g_i)
							vRej[idx].a = 2;
						else
							vRej[idx].a = 3;*/
						
						int temp = (int(rand()) % 2);
						if (temp % 2 == 0)
							vRej[idx].a = 2;
						else vRej[idx].a = 3;
						
					}

					/*
					if (flag_r == 1)
					{
						vRej[idx].a = 2;

					}
					if (flag_g == 1)
					{
						vRej[idx].a = 3;

					}
					if (flag_b == 1)
					{
						vRej[idx].a = 4;
					}*/
				}
				
			}
		}
		


	}
}
//参数分别是图像，文件大小
void MLISMultiThread(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();

	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithColor, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	//最后几个进程（分配不均导致的后果）
	
	DealwithColor(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
	
}
//===========================CMYKColor========================================
void DealwithCMYKColor(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) return;
	float radius = 1.42;
	float r, g, b;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{
		int flag = 0;
		vRej[idx].x = ivs[idx][0]*0.5;
		vRej[idx].y = ivs[idx][1]*0.5;
		vRej[idx].z = radius;
		float thelta = IVS_M_PI * radius * radius;
		int loc_x = int(ivs[idx][0] * i_w);
		int loc_y = int(ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= i_w || loc_y >= i_h)
			return;
		int flag_c=0, flag_m=0, flag_y=0,flag_k=0;
		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;
		float c_, m_, y_, k_;
		//RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
		RGBtoCMY(r, g, b, &c_, &m_, &y_);
		/*
		flag_c = acceptColor(pow(c_,1));
		flag_m = acceptColor(pow(m_,1));
		flag_y = acceptColor(pow(y_,1));
		*/
		//flag_k = acceptColor(pow(k_, 1));
		/*
		flag_c = acceptColor(pow(c_,1));
		flag_m = acceptColor(pow(m_, 1));
		flag_y = acceptColor(pow(y_, 1));
		flag_k = acceptColor(pow(k_, 1));
		*/
		float max_v = 0.0;

		
		float r_i = c_;
		float g_i = m_;
		float b_i = y_;
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16/4);

		if (r_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_c = 1;
		}

		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_m = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_y = 1;
		}
		
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
		if (flag_c + flag_m + flag_y /*+ flag_k */== 3)
		{
			vRej[idx].a = 4;
			/*
			int temp = int(rand());
			if ((temp) % 4 == 0)
				vRej[idx].a = 1;
			else if (temp % 4 == 1)
			{
				vRej[idx].a = 2;
			}
			else if (temp % 4 == 2)
			{
				vRej[idx].a = 3;
			}
			else if (temp % 4 == 3)
			{
				vRej[idx].a = 4;
			}*/
		}
		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 2)
		{
			/*
			if (flag_c + flag_m == 2)
			{
				vRej[idx].a = 5;
			}
			if (flag_m + flag_y == 2)
			{
				vRej[idx].a = 6;
			}
			if (flag_c + flag_y == 2)
			{
				vRej[idx].a = 7;
			}*/
			
			int temp;
			if (flag_c == 0)
			{
				vRej[idx].a = 5;
				/*
				int temp = (int(rand()) % 2);
				if (temp % 2 == 0)
					vRej[idx].a = 2;
				else vRej[idx].a = 3;*/
				
			}
			if (flag_m == 0)
			{
				vRej[idx].a = 6;
				/*
				int temp = (int(rand()) % 2);
				if (temp % 2 == 0)
					vRej[idx].a = 1;
				else vRej[idx].a = 3;*/
			}
			if (flag_y == 0)
			{
				vRej[idx].a = 7;
				/*
				int temp = (int(rand()) % 2);
				if (temp % 2 == 0)
					vRej[idx].a = 1;
				else vRej[idx].a = 2;*/
			}
			/*
			while (true)
			{
				temp = (int(rand()) % 3);
				if (temp == 0 && flag_c == 1)
				{
					vRej[idx].a = 1;
					break;
				}
				if (temp == 1 && flag_m == 1)
				{
					vRej[idx].a = 2;
					break;
				}
				if (temp == 2 && flag_y == 1)
				{
					vRej[idx].a = 3;
					break;
				}

			}*/
			/*
			if (flag_c == 0)
			{
				vRej[idx].a = 5;
				
				int temp = int(rand());
				if (temp % 3 == 0)
					vRej[idx].a = 2;
				else if (temp % 3 == 1)
					vRej[idx].a = 3;
				else if (temp % 3 == 2)
					vRej[idx].a = 4;
				
			}
			if (flag_m == 0)
			{
				vRej[idx].a = 6;
				
				int temp = int(rand());
				if (temp % 3 == 0)
					vRej[idx].a = 1;
				else if (temp % 3 == 1)
					vRej[idx].a = 3;
				else if (temp % 3 == 2)
					vRej[idx].a = 4;
					
			}
			if (flag_y == 0)
			{
				vRej[idx].a = 7;
				
				int temp = int(rand());
				if (temp % 3 == 0)
					vRej[idx].a = 1;
				else if (temp % 3 == 1)
					vRej[idx].a = 2;
				else if (temp % 3 == 2)
					vRej[idx].a = 4;
					
			}*/
			/*
			if (flag_k == 0)
			{
				vRej[idx].a = 4;
				
				int temp = int(rand());
				if (temp % 3 == 0)
					vRej[idx].a = 1;
				else if (temp % 3 == 1)
					vRej[idx].a = 2;
				else if (temp % 3 == 2)
					vRej[idx].a = 3;
					
			}*/
		}
		/*
		if (flag_c + flag_m + flag_y + flag_k == 2)
		{
			while (true)
			{	
				int temp = (int(rand()));
				if (temp % 4 == 0 && flag_c == 1)
				{
					vRej[idx].a = 1;
					break;
				}
				if (temp % 4 == 1 && flag_m == 1)
				{
					vRej[idx].a = 2;
					break;
				}
				if (temp % 4 == 2 && flag_y == 1)
				{
					vRej[idx].a = 3;
					break;
				}
						
				if (temp % 4 == 3 && flag_k == 1)
				{
					vRej[idx].a = 4;
					break;
				}
			}
		}*/
		if(flag_c + flag_m + flag_y /*+ flag_k*/ == 1)
		{
				if (flag_c == 1)
				{
					vRej[idx].a = 1;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
					//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (flag_m == 1)
				{
					vRej[idx].a = 2;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_y == 1)
				{
					vRej[idx].a = 3;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}
				/*
				if (flag_k == 1)
				{
					vRej[idx].a = 4;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}*/

		}
		
	}


	
}
//参数分别是图像，文件大小
void MLISMultiThreadCMYK(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();
	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithCMYKColor, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	//最后几个进程（分配不均导致的后果）
	DealwithCMYKColor(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}
//===========================RGBtoRYBK===================================
void DealwithRYBKColor(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) return;
	
	float radius = 1.42;
	float r, g, b;
	float thelta = IVS_M_PI * radius * radius;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{
		int flag = 0;
		vRej[idx].x = ivs[idx][0];
		vRej[idx].y = ivs[idx][1];
		vRej[idx].z = radius;
		int loc_x = int(ivs[idx][0] * i_w);
		int loc_y = int(ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= i_w || loc_y >= i_h)
			return;
		int flag_r=0, flag_y=0, flag_b=0, flag_k=0;
		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;
		float r_, y_, b_, k_;
		//RGBtoRYBK(r, g, b, &r_, &y_, &b_, &k_);
		RGBtoRYB(r, g, b, &r_, &y_, &b_);
		
		/*
		
		flag_r = acceptColor(pow(r_,1));
		flag_y = acceptColor(pow(y_, 1));
		flag_b = acceptColor(pow(b_, 1));
		*/
		//flag_k = acceptColor(pow(k_, 1));
		float r_i = r_;
		float g_i = y_;
		float b_i = b_;
		/*
		cout << "==============" << endl;
		cout << r_i << endl;
		cout << g_i << endl;
		cout << b_i << endl;
		*/
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16.0/4);
		
		if (r_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_r = 1;
		}

		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_y = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_b = 1;
		}

		float max_v = 0.0;
		
		//cout << "test" << endl;


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

		
		if (flag_r + flag_y + flag_b /*+ flag_k */== 3)
		{
			vRej[idx].a = 4;
			/*
			if ((int(rand())) % 4 == 0)
				vRej[idx].a = 1;
			else if ((int(rand())) % 4 == 1)
			{
				vRej[idx].a = 2;
			}
			else if ((int(rand())) % 4 == 2)
			{
				vRej[idx].a = 3;
			}
			else if ((int(rand())) % 4 == 3)
			{
				vRej[idx].a = 4;
			}
			*/
		}
		if (flag_r + flag_y + flag_b /*+ flag_k*/ == 2)
		{
			int temp;
			while (true)
			{
				temp = (int(rand()) % 3);
				if (temp == 0 && flag_r == 1)
				{
					vRej[idx].a = 1;
					break;
				}
				if (temp == 1 && flag_y == 1)
				{
					vRej[idx].a = 2;
					break;
				}
				if (temp == 2 && flag_b == 1)
				{
					vRej[idx].a = 3;
					break;
				}

			}
			/*
			if (flag_r == 0)
			{
				if ((int(rand())) % 3 == 0)
					vRej[idx].a = 2;
				else if ((int(rand())) % 3 == 1)
					vRej[idx].a = 3;
				else if ((int(rand())) % 3 == 2)
					vRej[idx].a = 4;
			}
			if (flag_y == 0)
			{
				if ((int(rand())) % 3 == 0)
					vRej[idx].a = 1;
				else if ((int(rand())) % 3 == 1)
					vRej[idx].a = 3;
				else if ((int(rand())) % 3 == 2)
					vRej[idx].a = 4;
			}
			if (flag_b == 0)
			{
				if ((int(rand())) % 3 == 0)
					vRej[idx].a = 1;
				else if ((int(rand())) % 3 == 1)
					vRej[idx].a = 2;
				else if ((int(rand())) % 3 == 2)
					vRej[idx].a = 4;
			}
			if (flag_k == 0)
			{
				if ((int(rand())) % 3 == 0)
					vRej[idx].a = 1;
				else if ((int(rand())) % 3 == 1)
					vRej[idx].a = 2;
				else if ((int(rand())) % 3 == 2)
					vRej[idx].a = 3;
			}
			*/
		}
		else {
			if (flag_r + flag_y + flag_b /*+ flag_k*/ == 1)
			{
				if (flag_r == 1)
				{
					vRej[idx].a = 1;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
					//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (flag_y == 1)
				{
					vRej[idx].a = 2;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_b == 1)
				{
					vRej[idx].a = 3;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}
				/*
				if(flag_r +flag_y == 2)
					vRej[idx].a = 5;
				else if (flag_y + flag_b == 2)
					vRej[idx].a = 6;
				else if (flag_r + flag_b == 2)
					vRej[idx].a = 7;
				else if(flag_k == 1)
					vRej[idx].a = 4;

				*/
				/*
				while (true)
				{
					int temp = (int(rand()));
					if (temp % 4 == 0 && flag_r == 1)
					{
						vRej[idx].a = 1;
						break;
					}
					if (temp % 4 == 1 && flag_y == 1)
					{
						vRej[idx].a = 2;
						break;
					}
					if (temp % 4 == 2 && flag_b == 1)
					{
						vRej[idx].a = 3;
						break;
					}

					if (temp % 4 == 3 && flag_k == 1)
					{
						vRej[idx].a = 4;
						break;
					}
				}*/
			}
			/*
			else {
				if (flag_r == 1)
				{
					vRej[idx].a = 1;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
					//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (flag_y == 1)
				{
					vRej[idx].a = 2;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_b == 1)
				{
					vRej[idx].a = 3;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_k == 1)
				{
					vRej[idx].a = 4;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}

			}*/
			else if (flag_r + flag_y + flag_b /*+ flag_k*/ == 0)
			{
				vRej[idx].a = 5;
			}
		}
		
	}



}
//参数分别是图像，文件大小
void MLISMultiThreadRYBK(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();
	
	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithRYBKColor, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	
	//最后几个进程（分配不均导致的后果）
	DealwithRYBKColor(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}
//==================================================================
//=============deal with RGBcolor=============================
void DealwithColorVaryRadius(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) return;
	float minRadius = 1.2;
	float maxRadius = 4;
	float r, g, b;
	int x_top, x_bottom, y_top, y_bottom;
	int kernel_size = 5;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{
		vRej[idx].x = ivs[idx][0];
		vRej[idx].y = ivs[idx][1];
		//vRej[idx].z = radius;
		int loc_x = int(ivs[idx][0] * i_w);
		int loc_y = int(ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= i_w || loc_y >= i_h)
			return;
		int flag_r = 0, flag_g=0, flag_b=0;
		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;
		x_top = max(loc_x - kernel_size, 0);
		x_bottom = min(loc_x + kernel_size, i_w-1);
		y_top = max(loc_y - kernel_size, 0);
		y_bottom = min(loc_y + kernel_size, i_h-1);
		float sum_r = 0.0,sum_g=0.0,sum_b=0.0;
		int count=0;
		SColor tempDitherPixel;
		for(int i = x_top;i<=x_bottom;i++)
			for (int j = y_top; j <= y_bottom; j++)
			{		
				count++;
				tempDitherPixel = imageData.GetPixel(i, j);
				sum_r += float(tempDitherPixel.R/225.0);
				sum_g += float(tempDitherPixel.G/255.0);
				sum_b += float(tempDitherPixel.B/255.0);
			}
		
		float mean_r = 0.0, mean_g = 0.0, mean_b = 0.0;
		mean_r = float(sum_r / count);
		mean_g = float(sum_g / count);
		mean_b = float(sum_b / count);
		float distance = max((abs(mean_r - r) + abs(mean_g - g) + abs(mean_b - b)),float(0.01));
		vRej[idx].z = max((maxRadius - ((maxRadius - minRadius)  * distance/0.7)), 1.2);



		
		flag_r = acceptColor(r);
		flag_g = acceptColor(g);
		flag_b = acceptColor(b);
		
		float max_v = 0.0;

		if (flag_r == 1 && flag_g == 1 && flag_b == 1)
		{
			vRej[idx].a = 1;
			//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
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
			if (flag_r + flag_g + flag_b == 2)
			{
				int temp;
				while (true)
				{
					temp = (int(rand()) % 3);
					if (temp == 0 && flag_r == 1)
					{
						vRej[idx].a = 2;
						break;
					}
					if (temp == 1 && flag_g == 1)
					{
						vRej[idx].a = 3;
						break;
					}
					if (temp == 2 && flag_b == 1)
					{
						vRej[idx].a = 4;
						break;
					}

				}
			}
			else {
				if (flag_r == 1)
				{
					vRej[idx].a = 2;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
					//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
				}
				if (flag_g == 1)
				{
					vRej[idx].a = 3;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
				}
				if (flag_b == 1)
				{
					vRej[idx].a = 4;
					//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
				}
			}
		}


	}
}
//参数分别是图像，文件大小
void MLISMultiThreadVaryRadius(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();
	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithColorVaryRadius, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	//最后几个进程（分配不均导致的后果）
	DealwithColorVaryRadius(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}

//=======================varyRiauds cmyk===================================

void DealwithCMYKColorVaryRaidus(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) return;
	float minRadius = 1.1;
	float maxRadius = 4;
	float r, g, b;
	int x_top, x_bottom, y_top, y_bottom;
	int kernel_size = 2;
	float radius = 2.0;
	//radius = sqrt(radius);
	float thelta = IVS_M_PI * radius * radius;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{
		int flag = 0;
		vRej[idx].x = ivs[idx][0];
		vRej[idx].y = ivs[idx][1];
		//vRej[idx].z = radius;
		int loc_x = int(ivs[idx][0] * i_w);
		int loc_y = int(ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= i_w || loc_y >= i_h)
			return;
		int flag_c = 0, flag_m = 0, flag_y= 0, flag_k=0;
		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;
		/*
		x_top = max(loc_x - kernel_size, 0);
		x_bottom = min(loc_x + kernel_size, i_w - 1);
		y_top = max(loc_y - kernel_size, 0);
		y_bottom = min(loc_y + kernel_size, i_h - 1);
		float sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
		int count = 0;
		SColor tempDitherPixel;
		for (int i = x_top; i <= x_bottom; i++)
			for (int j = y_top; j <= y_bottom; j++)
			{
				count++;
				tempDitherPixel = imageData.GetPixel(i, j);
				sum_r += float(tempDitherPixel.R / 225.0);
				sum_g += float(tempDitherPixel.G / 255.0);
				sum_b += float(tempDitherPixel.B / 255.0);
			}

		float mean_r = 0.0, mean_g = 0.0, mean_b = 0.0;
		mean_r = float(sum_r / count);
		mean_g = float(sum_g / count);
		mean_b = float(sum_b / count);
		float distance = max((abs(mean_r - r) + abs(mean_g - g) + abs(mean_b - b)), float(0.01));
		vRej[idx].z = max((maxRadius - ((maxRadius - minRadius)  * distance / 0.7)), 1.2);


		*/


		float c_, m_, y_, k_;
		//RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
		RGBtoCMY(r, g, b, &c_, &m_, &y_);
		float r_i = c_;
		float g_i = m_;
		float b_i = y_;
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16);

		if (r_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_c = 1;
		}

		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_m = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_y = 1;
		}
		/*
		flag_c = acceptColor(c_);
		flag_m = acceptColor(m_);
		flag_y = acceptColor(y_);
		flag_k = acceptColor(k_);
		*/
		float max_v = 0.0;

		if (flag_c + flag_m + flag_y /*+ flag_k */ == 3)
		{
			vRej[idx].a = 4;
		}
		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 2)
		{
			int temp;
			while (true)
			{
				temp = (int(rand()) % 3);
				if (temp == 0 && flag_c == 1)
				{
					vRej[idx].a = 1;
					break;
				}
				if (temp == 1 && flag_m == 1)
				{
					vRej[idx].a = 2;
					break;
				}
				if (temp == 2 && flag_y == 1)
				{
					vRej[idx].a = 3;
					break;
				}

			}
			
			
		}
		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 1)
		{
			if (flag_c == 1)
			{
				vRej[idx].a = 1;
				//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 0, 0)), Stroke(0, Color(255, 255, 255)));
				//c.filledCircle(int(x * o_w), int(y * o_h), 0.6, (1) * 65535, (0) * 65535, (0) * 65535, (0) * 65535);
			}
			if (flag_m == 1)
			{
				vRej[idx].a = 2;
				//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 255, 0)), Stroke(0, Color(255, 255, 255)));
			}
			if (flag_y == 1)
			{
				vRej[idx].a = 3;
				//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
			}
			/*
			if (flag_k == 1)
			{
			vRej[idx].a = 4;
			//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(0, 0, 255)), Stroke(0, Color(255, 255, 255)));
			}*/

		}


		
		
		
		if (vRej[idx].a == 1)
		{
			//float w = 0.30*r + 0.59*g + 0.11*b;
			float tt = c_*255.0;//和纯色相差多少，相差越大，就越小
			tt = sqrt(tt + 1)*1.8;
			radius = sqrt(float((i_h*i_w) / (n_max*IVS_M_PI)));
			//cout << radius << endl;
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w* sigmoid(w)) / (n_max*IVS_M_PI))) + 0.8,double(minRadius));
			vRej[idx].z = tempRadius;

		}
		else if (vRej[idx].a == 2)
		{
			//float r_ = (r) / (r + g + b);
			float tt = m_*255.0;
			tt = sqrt(tt + 1) * 1.8;
			radius = sqrt(float((i_h*i_w) / (n_max*IVS_M_PI)));
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w * sigmoid(r_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = tempRadius;
		}
		else if (vRej[idx].a == 3)
		{
			//float g_ = (g) / (r + g + b);
			float tt = y_*255.0;
			tt = sqrt(tt + 1) * 1.8;
			radius = sqrt(float((i_h*i_w) / (n_max*IVS_M_PI)));
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w *sigmoid(g_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = tempRadius;
		}
		else if (vRej[idx].a == 4)
		{
			k_ = 1 - (0.30*r + 0.59*g + 0.11*b);
			//float b_ = (b) / (r + g + b);
			float tt = k_*255.0;
			tt = sqrt(tt + 1) * 1.8;
			radius = sqrt(float((i_h*i_w) / (n_max*IVS_M_PI)));
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w * sigmoid(b_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = tempRadius;
		}
	}



}
//参数分别是图像，文件大小
void MLISMultiThreadCMYKVaryRaidus(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();
	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithCMYKColorVaryRaidus, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	//最后几个进程（分配不均导致的后果）
	DealwithCMYKColorVaryRaidus(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}
//===================================================================
//=============deal with RGBcolor1=============================
void DealwithColorVaryRadius2(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	
	if (id_end > 1048575) return;
	float minRadius = 0.9;
	float radius=2.0;
	//float maxRadius = 4;
	float r, g, b;
	int x_top, x_bottom, y_top, y_bottom;
	int kernel_size = 5;
	float thelta = IVS_M_PI * radius * radius;
	
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{
		int flag = 0;
		vRej[idx].x = ivs[idx][0];
		vRej[idx].y = ivs[idx][1];
		//vRej[idx].z = radius;
		int loc_x = int(ivs[idx][0] * i_w);
		int loc_y = int(ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= i_w || loc_y >= i_h)
			return;
		int flag_r=0, flag_g=0, flag_b=0;
		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;
		/*
		x_top = max(loc_x - kernel_size, 0);
		x_bottom = min(loc_x + kernel_size, i_w - 1);
		y_top = max(loc_y - kernel_size, 0);
		y_bottom = min(loc_y + kernel_size, i_h - 1);
		float sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
		int count = 0;
		SColor tempDitherPixel;
		for (int i = x_top; i <= x_bottom; i++)
			for (int j = y_top; j <= y_bottom; j++)
			{
				count++;
				tempDitherPixel = imageData.GetPixel(i, j);
				sum_r += float(tempDitherPixel.R / 225.0);
				sum_g += float(tempDitherPixel.G / 255.0);
				sum_b += float(tempDitherPixel.B / 255.0);
			}

		float mean_r = 0.0, mean_g = 0.0, mean_b = 0.0;
		mean_r = float(sum_r / count);
		mean_g = float(sum_g / count);
		mean_b = float(sum_b / count);
		float distance = max((abs(mean_r - r) + abs(mean_g - g) + abs(mean_b - b)), float(0.01));
		//vRej[idx].z = max((maxRadius - ((maxRadius - minRadius)  * distance / 0.7)), 1.2);
		*/


		/*
		flag_r = acceptColor(r);
		flag_g = acceptColor(g);
		flag_b = acceptColor(b);
		*/

		float r_i = r;
		float g_i = g;
		float b_i = b;
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16/4);
		float max_v = 0.0;

		if (r_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}

			flag_r = 1;
		}
		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_g = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_b = 1;
		}
		//==================================================
		if (flag_r == 1 && flag_g == 1 && flag_b == 1)
		{
			vRej[idx].a = 1;
			//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
		}
		else
		{

			if (flag_r + flag_g + flag_b == 2)
			{
				int temp;
				while (true)
				{
					temp = (int(rand()) % 3);
					if (temp == 0 && flag_r == 1)
					{
						vRej[idx].a = 2;
						break;
					}
					if (temp == 1 && flag_g == 1)
					{
						vRej[idx].a = 3;
						break;
					}
					if (temp == 2 && flag_b == 1)
					{
						vRej[idx].a = 4;
						break;
					}

				}


			}
			else {
				if (flag_r == 1)
				{
					vRej[idx].a = 2;

				}
				if (flag_g == 1)
				{
					vRej[idx].a = 3;

				}
				if (flag_b == 1)
				{
					vRej[idx].a = 4;
				}
			}
		}
		//cout << "test" << endl;
		if (vRej[idx].a == 1)
		{
			float w = 0.30*r + 0.59*g + 0.11*b;
			float tt = w*255.0;//和纯色相差多少，相差越大，就越小
			tt = sqrt(tt + 1)*1.8;	
			radius = sqrt(float((i_h*i_w) / (n_max*IVS_M_PI))) ;
			//cout << radius << endl;
			double tempRadius = max(radius*tt / 12.0 , double(minRadius));
			//radius = max(sqrt(float((i_h*i_w* sigmoid(w)) / (n_max*IVS_M_PI))) + 0.8,double(minRadius));
			vRej[idx].z = tempRadius;
			
		}
		else if (vRej[idx].a == 2)
		{
			//float r_ = (r) / (r + g + b);
			float tt = r*255.0;
			tt = sqrt(tt + 1) * 1.8;
			radius = sqrt(float((i_h*i_w) / (n_max*IVS_M_PI)));
			double tempRadius = max(radius*tt / 12.0 ,double(minRadius));
			//radius = max(sqrt(float((i_h*i_w * sigmoid(r_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = tempRadius;
		}
		else if (vRej[idx].a == 3)
		{
			//float g_ = (g) / (r + g + b);
			float tt = g*255.0;
			tt = sqrt(tt + 1) * 1.8;
			radius = sqrt(float((i_h*i_w) / (n_max*IVS_M_PI))) ;
			double tempRadius = max(radius*tt / 12.0 , double(minRadius));
			//radius = max(sqrt(float((i_h*i_w *sigmoid(g_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = tempRadius;
		}
		else if (vRej[idx].a == 4)
		{
			//float b_ = (b) / (r + g + b);
			float tt =  b*255.0;
			tt = sqrt(tt + 1) * 1.8;
			radius = sqrt(float((i_h*i_w) / (n_max*IVS_M_PI))) ;
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w * sigmoid(b_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = tempRadius;
		}

	}
}
//参数分别是图像，文件大小
void MLISMultiThreadVaryRadius2(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();
	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithColorVaryRadius2, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	//最后几个进程（分配不均导致的后果）
	DealwithColorVaryRadius2(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}
//=======================varyRiauds RYBK===================================

void DealwithRYBKColorVaryRaidus(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) return;
	float minRadius = 0.5;
	float maxRadius = 1.42;
	float r, g, b;
	int x_top, x_bottom, y_top, y_bottom;
	int kernel_size = 2;
	float radius=1.42;
	double nn_radius = sqrt(radius);
	float thelta = IVS_M_PI * radius * radius;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{
		int flag = 0;
		vRej[idx].x = ivs[idx][0];
		vRej[idx].y = ivs[idx][1];
		//vRej[idx].z = radius;
		int loc_x = int(ivs[idx][0] * i_w);
		int loc_y = int(ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= i_w || loc_y >= i_h)
			return;
		int flag_r=0, flag_g=0, flag_b=0, flag_k=0;
		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;

		float r_, y_, b_, k_;
		RGBtoRYB(r, g, b, &r_, &y_, &b_);
		float r_i = r_;
		float g_i = y_;
		float b_i = b_;
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16/4 );
		float max_v = 0.0;

		if (r_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}

			flag_r = 1;
		}
		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_g = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_b = 1;
		}
		//RGBtoCMYK(r, g, b, &c_, &m_, &y_, &k_);
		/*
		flag_c = acceptColor(r_);
		flag_m = acceptColor(y_);
		flag_y = acceptColor(b_);
		flag_k = acceptColor(k_);
		*/
		//float max_v = 0.0;




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
		if (flag_r == 1 && flag_g == 1 && flag_b == 1)
		{
			vRej[idx].a = 4;
			//doc << Circle(Point(int(x * o_w), int(y * o_h)), 1.3, Fill(Color(255, 255, 255)), Stroke(0, Color(255, 255, 255)));
		}
		else
		{

			if (flag_r + flag_g + flag_b == 2)
			{
				int temp;
				while (true)
				{
					temp = (int(rand()) % 3);
					if (temp == 0 && flag_r == 1)
					{
						vRej[idx].a = 1;
						break;
					}
					if (temp == 1 && flag_g == 1)
					{
						vRej[idx].a = 2;
						break;
					}
					if (temp == 2 && flag_b == 1)
					{
						vRej[idx].a = 3;
						break;
					}

				}


			}
			else {
				if (flag_r == 1)
				{
					vRej[idx].a = 1;

				}
				if (flag_g == 1)
				{
					vRej[idx].a = 2;

				}
				if (flag_b == 1)
				{
					vRej[idx].a = 3;
				}
			}
		}

		if (vRej[idx].a == 1)
		{
			//float w = 0.30*r + 0.59*g + 0.11*b;
			float tt = r_*255.0;//和纯色相差多少，相差越大，就越小
			tt = sqrt(tt + 1);
			radius = nn_radius;
			//cout << radius << endl;
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w* sigmoid(w)) / (n_max*IVS_M_PI))) + 0.8,double(minRadius));
			vRej[idx].z = min(tempRadius,double(maxRadius));

		}
		else if (vRej[idx].a == 2)
		{
			//float r_ = (r) / (r + g + b);
			float tt = y_*255.0;
			tt = sqrt(tt + 1) ;
			radius = nn_radius /*sqrt(float((i_h*i_w) / (n_max*IVS_M_PI)))*/;
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w * sigmoid(r_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = min(tempRadius, double(maxRadius));;
		}
		else if (vRej[idx].a == 3)
		{
			//float g_ = (g) / (r + g + b);
			float tt = b_*255.0;
			tt = sqrt(tt + 1);
			radius = nn_radius;
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w *sigmoid(g_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = min(tempRadius, double(maxRadius));;
		}
		else if (vRej[idx].a == 4)
		{
			//float b_ = (b) / (r + g + b);
			k_ = 1-(0.30*r + 0.59*g + 0.11*b);
			float tt = k_*255.0;
			tt = sqrt(tt + 1);
			radius = nn_radius;
			double tempRadius = max(radius*tt / 12.0, double(minRadius));
			//radius = max(sqrt(float((i_h*i_w * sigmoid(b_)) / (n_max*IVS_M_PI))) + 0.8, double(minRadius));
			vRej[idx].z = min(tempRadius, double(maxRadius));;
		}
	}



}
//参数分别是图像，文件大小
void MLISMultiThreadRYBKVaryRaidus(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();
	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithRYBKColorVaryRaidus, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	//最后几个进程（分配不均导致的后果）
	DealwithRYBKColorVaryRaidus(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}
//===================================================================


void DealwithCMYKColor2(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) { cout << "test" << endl; return; }
	
	float radius = 1.42;
	float r, g, b;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{
		
		int flag = 0;
		vRej[idx].x = 0.5 + ivs[idx][0]*0.5;
		vRej[idx].y = ivs[idx][1]*0.5;
		vRej[idx].z = radius;
		//cout << "test" << endl;
		float thelta = IVS_M_PI * radius * radius;
		int loc_x = int(i_w + ivs[idx][0] * i_w);
		int loc_y = int(ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= 2*i_w || loc_y >= 2*i_h)
			return;
		int flag_c = 0, flag_m = 0, flag_y = 0, flag_k = 0;
		
		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;
			
		float c_, m_, y_, k_;
		RGBtoCMY(r, g, b, &c_, &m_, &y_);
		float max_v = 0.0;
		float r_i = c_;
		float g_i = m_;
		float b_i = y_;
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16 / 4);

		if (r_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_c = 1;
		}

		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_m = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_y = 1;
		}

		
		if (flag_c + flag_m + flag_y /*+ flag_k */ == 3)
		{
			vRej[idx].a = 4;
			
		}
		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 2)
		{
			

			int temp;
			if (flag_c == 0)
			{
				vRej[idx].a = 5;
				

			}
			if (flag_m == 0)
			{
				vRej[idx].a = 6;
				
			}
			if (flag_y == 0)
			{
				vRej[idx].a = 7;
				
			}
			
		
		
		}
	
		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 1)
		{
			if (flag_c == 1)
			{
				vRej[idx].a = 1;
				
			}
			if (flag_m == 1)
			{
				vRej[idx].a = 2;
				
			}
			if (flag_y == 1)
			{
				vRej[idx].a = 3;
				
			}
			

		}

	}



}
//参数分别是图像，文件大小
void MLISMultiThreadCMYK2(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();
	
	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithCMYKColor2, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	DealwithCMYKColor2(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}
//=====================================================
void DealwithCMYKColor3(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) { cout << "test" << endl; return; }

	float radius = 1.42;
	float r, g, b;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{

		int flag = 0;
		vRej[idx].x = ivs[idx][0]*0.5;
		vRej[idx].y = 0.5+ivs[idx][1]*0.5;
		vRej[idx].z = radius;
		//cout << "test" << endl;
		float thelta = IVS_M_PI * radius * radius;
		int loc_x = int(ivs[idx][0] * i_w);
		int loc_y = int(i_h+ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= 2 * i_w || loc_y >= 2 * i_h)
			return;
		int flag_c = 0, flag_m = 0, flag_y = 0, flag_k = 0;

		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;

		float c_, m_, y_, k_;
		RGBtoCMY(r, g, b, &c_, &m_, &y_);
		float max_v = 0.0;
		float r_i = c_;
		float g_i = m_;
		float b_i = y_;
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16 / 4);

		if (r_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_c = 1;
		}

		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_m = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_y = 1;
		}


		if (flag_c + flag_m + flag_y /*+ flag_k */ == 3)
		{
			vRej[idx].a = 4;

		}
		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 2)
		{


			int temp;
			if (flag_c == 0)
			{
				vRej[idx].a = 5;


			}
			if (flag_m == 0)
			{
				vRej[idx].a = 6;

			}
			if (flag_y == 0)
			{
				vRej[idx].a = 7;

			}



		}

		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 1)
		{
			if (flag_c == 1)
			{
				vRej[idx].a = 1;

			}
			if (flag_m == 1)
			{
				vRej[idx].a = 2;

			}
			if (flag_y == 1)
			{
				vRej[idx].a = 3;

			}


		}

	}



}
//参数分别是图像，文件大小
void MLISMultiThreadCMYK3(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();

	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithCMYKColor3, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	DealwithCMYKColor3(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}
//=====================================================================
void DealwithCMYKColor4(SImageData & imageData, vector<glm::vec4>& vRej, int n_max, const int i_w, const int i_h, int id_start, int id_end)
{
	if (id_end > 1048575) { cout << "test" << endl; return; }

	float radius = 1.42;
	float r, g, b;
	for (uint32_t idx = id_start; idx < id_end; idx++)
	{

		int flag = 0;
		vRej[idx].x = 0.5 + ivs[idx][0] * 0.5;
		vRej[idx].y = 0.5 + ivs[idx][1] * 0.5;
		vRej[idx].z = radius;
		//cout << "test" << endl;
		float thelta = IVS_M_PI * radius * radius;
		int loc_x = int(i_w + ivs[idx][0] * i_w);
		int loc_y = int(i_h + ivs[idx][1] * i_h);
		if (loc_x < 0 || loc_y < 0 || loc_x >= 2 * i_w || loc_y >= 2 * i_h)
			return;
		int flag_c = 0, flag_m = 0, flag_y = 0, flag_k = 0;

		SColor& destDitherPixel = imageData.GetPixel(loc_x, loc_y);
		r = (float)destDitherPixel.R / 255.0;
		g = (float)destDitherPixel.G / 255.0;
		b = (float)destDitherPixel.B / 255.0;

		float c_, m_, y_, k_;
		RGBtoCMY(r, g, b, &c_, &m_, &y_);
		float max_v = 0.0;
		float r_i = c_;
		float g_i = m_;
		float b_i = y_;
		float v_i = (idx + 1) * thelta / (i_w * i_h * 16 / 4);

		if (r_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_c = 1;
		}

		if (g_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_m = 1;
		}
		if (b_i >= v_i)
		{
			if (flag == 0)
			{
				count_num++;
				flag = 1;
			}
			flag_y = 1;
		}


		if (flag_c + flag_m + flag_y /*+ flag_k */ == 3)
		{
			vRej[idx].a = 4;

		}
		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 2)
		{


			int temp;
			if (flag_c == 0)
			{
				vRej[idx].a = 5;


			}
			if (flag_m == 0)
			{
				vRej[idx].a = 6;

			}
			if (flag_y == 0)
			{
				vRej[idx].a = 7;

			}



		}

		if (flag_c + flag_m + flag_y /*+ flag_k*/ == 1)
		{
			if (flag_c == 1)
			{
				vRej[idx].a = 1;

			}
			if (flag_m == 1)
			{
				vRej[idx].a = 2;

			}
			if (flag_y == 1)
			{
				vRej[idx].a = 3;

			}


		}

	}



}
//参数分别是图像，文件大小
void MLISMultiThreadCMYK4(SImageData & imageData, vector<glm::vec4>& vRej, const int i_w, const int i_h, int num_thread)
{
	vector<thread> thread_pool;
	int job_size = vRej.size() / num_thread;
	int n_max = vRej.size();

	for (int i = 0; i < num_thread - 1; i++)
	{
		int start = i*job_size + 1;
		thread_pool.push_back(thread(DealwithCMYKColor4, ref(imageData), ref(vRej), n_max, i_w, i_h, start, start + job_size));
	}
	DealwithCMYKColor4(ref(imageData), ref(vRej), n_max, i_w, i_h, job_size*(num_thread - 1) + 1, vRej.size());
	for (int i = 0; i < num_thread - 1; i++)
	{
		thread_pool[i].join();
	}
}