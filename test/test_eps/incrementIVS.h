#include"ivsData.h"
#include<iostream>
#define N 1048576
double ivs_i[N * 4][2];
void increSet()
{
	int index = 0;
	int Incre_index = 0;
	for (index = 0; index < N; index++)
	{
		ivs_i[index][0] = ivs[index][0] * 0.5;
		ivs_i[index][1] = ivs[index][1] * 0.5;
		
	}
	for (index = 0; index < N; index++)
	{
		ivs_i[N + index][0] = 0.5 + (ivs[index][0] * 0.5);
		ivs_i[N + index][1] = ivs[index][1] * 0.5;
		//std::cout << ivs_i[N + index][1] << std::endl;
	}
	for (index = 0; index < N; index++)
	{
		ivs_i[2 * N + index][0] = ivs[index][0] * 0.5;

		ivs_i[2 * N + index][1] = 0.5 + (ivs[index][1] * 0.5);

	}
	for (index = 0; index < N; index++)
	{
		ivs_i[3 * N + index][0] = 0.5 + (ivs[index][0] * 0.5);
		ivs_i[3 * N + index][1] = 0.5 + (ivs[index][1] * 0.5);
	}
}