#include <iostream>
#include <chrono>
#include <ostream>
#include <random>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define CIRCLE_RADIUS 1
#define PI 3.1415926535
// I also don't usually comment this much but I do think this is meaning
// full for non-c++ programmers to read i guess.
//
// Forgive me I don't code on linux much.
// however, I do want to give this a shot on linux
// if this doesn't cross compile don't blame me lol

// point_t should fit in a x64 bit register so hopefully
// the compiler realizes this or uses intrisics / AVX stuff 
struct point_t
{
	float x;
	float y;
};

float randf()
{
	return std::abs((float)(rand()) / (float)(RAND_MAX/1.f));
}

///
// Rejection Sampling
///
namespace rejection
{
	bool in_circle(const point_t& point)
	{
	    return (point.x*point.x + point.y*point.y) < CIRCLE_RADIUS;  
	}
	
	point_t gen_point()
	{
		point_t point;
		while(true)
		{
			point.x = (randf() * 2) - 1;
			point.y = (randf() * 2) - 1; 
			
			if(in_circle(point))
				return point;
	  }
	}
}
///
// Polar
///
namespace polar
{
	point_t gen_point()
	{
	    point_t point;
	    float theta = randf() * 2 * PI;
	    float r = sqrt(randf());
	    return {r * cos(theta), r * sin(theta)};
	}
}

///
// Maximum
///

namespace maximum
{
	point_t gen_point()
	{
		point_t point;
		float theta = randf() * 2 * PI;
		float r = randf() + randf();
		if(r >= 1)
			r = 2 - r;
		return {r * cos(theta), r * sin(theta)};
	}
}

namespace maximum_2
{
	point_t gen_point()
	{
		point_t point;
		float theta = randf() * 2 * PI;
		float r = randf();
		float x = randf();
		if(x > r)
			r = x;
		return {r * cos(theta), r * sin(theta)};
	}
}

using namespace std::chrono;
using namespace std::chrono_literals;

int main()
{
	srand(time(0));
	std::cout << "starting" << std::endl;
	auto clock = high_resolution_clock();

	constexpr unsigned test_len = 0x100000; 

	//0x10000 will be our space for testing purposes 
	auto start = clock.now();
	for(auto i = 0u; i < test_len; ++i)
	{
	    point_t point = polar::gen_point();
	}
	auto end = clock.now();

	auto polar_time = std::chrono::duration_cast<milliseconds>(
					end - start).count();

	start = clock.now();
	for(auto i = 0u; i < test_len; ++i)
	{
		point_t point = rejection::gen_point(); 
	}
	end = clock.now();

	auto rejection_time = std::chrono::duration_cast<milliseconds>(
					                      end - start).count();

	start = clock.now();
    for(auto i = 0u; i < test_len; ++i)
	{
		point_t point = maximum::gen_point();
	}
	end = clock.now();

	auto sum_time = std::chrono::duration_cast<milliseconds>(
								                        end - start).count();
	start = clock.now();
    for(auto i = 0u; i < test_len; ++i)
    {
        point_t point = maximum::gen_point();
    }
    end = clock.now();
    auto max_time = std::chrono::duration_cast<milliseconds>(
                                                          end - start).count();
				  

	std::cout << "Rejection Time:" << std::endl;
	std::cout << rejection_time << std::endl;
	std::cout << "Polar Time:" << std::endl;
	std::cout << polar_time << std::endl;
	std::cout << "Maximum Time:" << std::endl;
	std::cout << max_time << std::endl;
	std::cout << "Sum Time:" << std::endl;
	std::cout << sum_time << std::endl;


	std::cout << std::endl;
}
