#pragma once

#include <stdio.h>
#include <stdint.h>

void initrand() {
	int sd = (unsigned)time(NULL);
	srand(sd);
}

int random_range(int min, int max)
{	
	return min + (rand() % (max - min + 1));
}

double random_double() {
	int rand = random_range(0, 10);
	return rand * 0.1;
}

double random_double_range(double min, double max) {
	return min + (max - min) * random_double();
}
