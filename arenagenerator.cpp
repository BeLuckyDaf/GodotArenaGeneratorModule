#include "arenagenerator.h"
#include <random>
#include <stdio.h>

ArenaGenerator::ArenaGenerator() 
{
	death_threshold = 3;
	birth_threshold = 5;
}

Array ArenaGenerator::generate(
        const int width,
        const int height,
        const int iterations,
        const int rate,
        const int mirror_iterations
)
{
    this->width = width;
    this->height = height;
    this->iterations = iterations;
    this->rate = rate;
    this->mirror_iterations = mirror_iterations;

    Array result;
    map = new int[width *height];
    buffer = new int[width *height];
    
    generate_random();
    simulate(iterations);
    mirror(false);
    simulate(mirror_iterations);
    mirror(true);
    simulate(mirror_iterations);
    
    for (int i = 0; i < width * height; i++)
    {
        result.append(map[i]);
    }

    delete map;
    delete buffer;
    return result;
}

void ArenaGenerator::generate_random()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution < > distrib(0, 100);
    for (int i = 0; i < width * height; i++)
    {
        map[i] = distrib(gen) <= rate ? 1 : 0;
    }
}

int ArenaGenerator::apply_rules(
        const int c,
        const int tl,
        const int t,
        const int tr,
        const int r,
        const int br,
        const int b,
        const int bl,
        const int l
)
{
    int sum = tl + t + tr + r + br + b + bl + l;
    if (c == 0 && sum >= birth_threshold) return 1;
    else if (c == 1 && sum <= death_threshold) return 0;
    return c;
}

int ArenaGenerator::get_point(const int x, const int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height) return 1;
    return map[y *width + x];
}

void ArenaGenerator::simulate(const int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int tl = get_point(x - 1, y - 1);
                int t = get_point(x, y - 1);
                int tr = get_point(x + 1, y - 1);
                int r = get_point(x + 1, y);
                int br = get_point(x + 1, y + 1);
                int b = get_point(x, y + 1);
                int bl = get_point(x - 1, y + 1);
                int l = get_point(x - 1, y);
                buffer[(y *width) + x] = apply_rules(map[(y *width) + x], tl, t, tr, r, br, b, bl, l);
            }
        }
        
        /* Move data back to map from buffer */
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                map[y * width + x] = buffer[y * width + x];
            }
        }
    }
}

void ArenaGenerator::mirror(bool vertical) 
{
    if (vertical) 
	{
        for (int y = 0; y < height/2; y++) 
		{
            for (int x = 0; x < width; x++) 
			{
                map[(height - y - 1) * width + x] = map[y * width + x];
            }
        }
    } 
	else 
	{
        for (int y = 0; y < height; y++) 
		{
            for (int x = 0; x < width/2; x++) 
			{
                map[y * width + (width - x - 1)] = buffer[y * width + x];
            }
        }
    }
}

void ArenaGenerator::set_thresholds(const int b, const int d) 
{
	birth_threshold = b;
	death_threshold = d;
}

void ArenaGenerator::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("generate", "width", "height", "iterations", "rate", "mirror_iterations"), &ArenaGenerator::generate);
	ClassDB::bind_method(D_METHOD("set_thresholds", "b", "d"), &ArenaGenerator::set_thresholds);
}