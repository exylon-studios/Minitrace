#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "vec.h"

#define MAXSCENE 8192

typedef struct vecf {
	float x, y, z;
} vecf;

typedef struct spheref {
	float rad;
	vecf o, c, n, h, e;
	emat m;
} spheref;

void floattodouble(double *d, float f) {
	*d = (double)(f);
}

int loadscene(const char *filename, sphere scn[], int debug, int *szscene, vec *psky, vec *peye, int *pw, int *ph, int *pspp) {
	FILE *f = fopen(filename, "r");
	
	spheref tempscn[MAXSCENE];
	vecf tempsky, tempeye;
	vec skycolor, eye;
	int n, w, h, spp;
	char mat[MAXSCENE];
	
	if (f == NULL) {
		printf("Error opening file \"%s\": ", filename);
		perror("");
		return EXIT_FAILURE;
	}
	
	if (f) {
		fscanf(f, "SCENECOUNT(%i)\n", &n);
		*szscene = n;
		
		fscanf(f, "RES(%ix%i)\n", &w, &h);
		*pw = w;
		*ph = h;
		
		fscanf(f, "SPP(%i)\n", &spp);
		*pspp = spp;
		
		fscanf(f, "SKY(%f,%f,%f)\n", &tempsky.x, &tempsky.y, &tempsky.z);
		
		fscanf(f, "EYE(%f,%f,%f)\n", &tempeye.x, &tempeye.y, &tempeye.z);
		
		for (int j = 0; j < n; j++) {
			fscanf(f, "SPH(%f,%f,%f,%f,%c)MAT(%f,%f,%f)EMIT(%f,%f,%f)\n", &tempscn[j].rad, &tempscn[j].o.x,  &tempscn[j].o.y,  &tempscn[j].o.z, &mat[j], 
				 &tempscn[j].c.x, &tempscn[j].c.y,  &tempscn[j].c.z, &tempscn[j].e.x, &tempscn[j].e.y, &tempscn[j].e.z);
				 
			floattodouble(&scn[j].rad, tempscn[j].rad);
			floattodouble(&scn[j].o.x, tempscn[j].o.x);
			floattodouble(&scn[j].o.y, tempscn[j].o.y);
			floattodouble(&scn[j].o.z, tempscn[j].o.z);
			floattodouble(&scn[j].c.x, tempscn[j].c.x);
			floattodouble(&scn[j].c.y, tempscn[j].c.y);
			floattodouble(&scn[j].c.z, tempscn[j].c.z);
			floattodouble(&scn[j].e.x, tempscn[j].e.x);
			floattodouble(&scn[j].e.y, tempscn[j].e.y);
			floattodouble(&scn[j].e.z, tempscn[j].e.z);
			floattodouble(&skycolor.x, tempsky.x);
			floattodouble(&skycolor.y, tempsky.y);
			floattodouble(&skycolor.z, tempsky.z);
			floattodouble(&eye.x, tempeye.x);
			floattodouble(&eye.y, tempeye.y);
			floattodouble(&eye.z, tempeye.z);
		}
		
		*psky = skycolor;
		*peye = eye;
		
		fclose(f);
	}
	
	for (int j = 0; j < n; j++) {
		if (mat[j] == 'D') {
				scn[j].m = Diffuse;
		} else if (mat[j] == 'M') {
				scn[j].m = Metallic;
		} else {
				scn[j].m = Other;
		}
		
		if (debug) {
			printf("Sky color: %f %f %f", skycolor.x, skycolor.y, skycolor.z);
			if (scn[j].m == Diffuse) {
				printf("Sphere radius: %f, Center: %f %f %f, Material Type: Diffuse, Color: %f %f %f\n", scn[j].rad, scn[j].o.x, scn[j].o.y, scn[j].o.z,
					scn[j].c.x, scn[j].c.y, scn[j].c.z);
			} else if (scn[j].m == Metallic) {
				printf("Sphere radius: %f, Center: %f %f %f, Material Type: Metallic, Color: %f %f %f\n", scn[j].rad, scn[j].o.x, scn[j].o.y, scn[j].o.z,
					scn[j].c.x, scn[j].c.y, scn[j].c.z);
			} else if (scn[j].m == Other) {
				printf("Sphere radius: %f, Center: %f %f %f, Material Type: Other, Color: %f %f %f\n", scn[j].rad, scn[j].o.x, scn[j].o.y, scn[j].o.z,
					scn[j].c.x, scn[j].c.y, scn[j].c.z);
			}
		}
	}
	
	return EXIT_SUCCESS;
}
