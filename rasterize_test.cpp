#include <gpurasterizer.h>
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
	GpuRasterizer rasterizer;

	for (int i = 0; i < 10; i++)
	{
		rasterizer.Rasterize();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return 0;
}

