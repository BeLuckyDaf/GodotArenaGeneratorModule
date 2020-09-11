#ifndef ARENAGENERATOR_H
#define ARENAGENERATOR_H
#include "core/reference.h"

class ArenaGenerator: public Reference
{
    GDCLASS(ArenaGenerator, Reference);

private:
    int *map;
    int *buffer;
    int width;
    int height;
    int rate;
    int iterations;
    int mirror_iterations;
	int death_threshold;
	int birth_threshold;

    void generate_random();
    int apply_rules(
        const int c,
        const int tl,
        const int t,
        const int tr,
        const int r,
        const int br,
        const int b,
        const int bl,
        const int l
	);
    void mirror(bool vertical);
    void simulate(const int n);
    int get_point(const int x, const int y) const;

protected:
        static void _bind_methods();

public:
    Array generate(
        const int width,
        const int height,
        const int iterations,
        const int rate,
        const int mirror_iterations
    );
	
	void set_thresholds(const int b, const int d);
	
    ArenaGenerator();
};

#endif	// ARENAGENERATOR_H