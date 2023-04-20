#include <omp.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>

using namespace std;
const float percentage = 100.0;

int main(int argc, char** argv)
{
	double min = 0;
	double max = 0;
	int tid;
	int numot = omp_get_max_threads();
	MEMORYSTATUSEX stat;
	stat.dwLength = sizeof(stat);
	GlobalMemoryStatusEx(&stat);
	long long size = (long long)round((stat.ullAvailPhys / 8.0 * percentage / 100.0));
	cout << "Free memory : " << round(stat.ullAvailPhys / 1000000000.0 * 9.313) / 10 << "GB" << endl;
	cout << "Vector of " << size << " doubles (~" << round(sizeof(double) * size / 100000000.0 * 9.313) / 100 << " GB)\n";
	for (int thrd = 1; thrd <= numot; thrd++) {
		vector<double> array;
		array.resize(size);

		auto start = chrono::steady_clock::now();
#pragma omp parallel num_threads(thrd) private (tid)
		{
			tid = omp_get_thread_num();
#pragma omp  for
			for (long long i = 0; i < array.size(); i++) {
				array[i] = -10 * rand() % 3061 + rand() % 1006 * rand() % 13061 / 13.5;
				array[i] = 5 * sin(0.3 * pow(array[i], 4)) + 0.5 * array[i];
			}
			if (tid == 0)
			{
				max = array[0];
				min = array[0];
			}
#pragma omp  for
			for (long long i = 0; i < array.size(); i++) {
				if (max < array[i]) max = array[i];
				if (min > array[i]) min = array[i];
			}
			if (tid == 0) cout << "Max = " << max << " min = " << min << endl;
		}
		auto end = chrono::steady_clock::now();
		cout << "Time for " << thrd << " threads : " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
	}
	system("pause");
	return 0;
}
