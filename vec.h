#pragma once

#define EPS 0.00001

#include <math.h>
#include "rand.h"

typedef struct vector {
	double x;
	double y;
	double z;
} vec;

typedef struct ray {
	vec o, d;
} ray;

typedef enum matenum {
	Diffuse,
	Metallic,
	Other
} emat;

typedef struct sphere {
	double rad;
	vec o, c, n, h, e;
	emat m;
} sphere;

const vec vzero = {0,0,0};

vec vdebug(vec a) {
	printf("X: %f, Y: %f, Z: %f\n", a.x, a.y, a.z);
}

bool vecerr(vec a) {
	double e = 1e-8;
	return (fabs(a.x) < e) && (fabs(a.y) < e) && (fabs(a.z) < e);
}

vec vaddeq(vec a, vec b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

vec vadd(vec a, vec b) {
	return (vec) {a.x + b.x, a.y + b.y, a.z + b.z};
}

vec vsub(vec a, vec b) {
	return (vec) {a.x - b.x, a.y - b.y, a.z - b.z};
}

vec vmul(vec a, vec b) {
	return (vec) {a.x * b.x, a.y * b.y, a.z * b.z};
}

vec vdiv(vec a, vec b) {
	return (vec) {a.x / b.x, a.y / b.y, a.z / b.z};
}

vec vmuld(vec a, double d) {
	return (vec) {a.x * d, a.y * d, a.z * d};
}

vec vdivd(vec a, double d) {
	//return (vec) {a.x / d, a.y / d, a.z / d};
	return vmuld(a, (1/d));
}

double dot(vec a, vec b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec cross(vec a, vec b) {
	return (vec) {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

double len(vec v) {
	return sqrt(dot(v,v));
}

vec unitvec(vec v){
	return vdivd(v, len(v));
}

vec vnorm(vec a)	{
	return a = vmuld(a, 1/sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

vec refl(vec v, vec n) {
	return vsub(v, vmuld(n, 2*dot(v,n)));
}

vec vrand(double min, double max) {
	return (vec) {random_double_range(min, max), random_double_range(min, max), random_double_range(min, max)};
}

vec random_unit_vector() {
	while (1) {
		vec r  = vrand(-1, 1);
		double dr = dot(r, r);
		if (1e-160 < dr && dr <= 1) {
			return vdivd(r, sqrt(dr));
		}
	}
}
