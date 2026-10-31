#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "vec.h"
#include "rand.h"
#include "sdnfile.h"

vec raypos( ray r, double t ) {
	return vadd( r.o, vmuld( r.d, t ) );
}

double clamp( double value ) {
	return ( value < 0 ) ? 0 : ( value > 1 ) ? 1 : value;
}

double gamma( double linearcomp ) {
	return ( linearcomp > 0 ) ? sqrt( linearcomp ) : 0;
}

bool isects( sphere* s, ray r ) {
	vec sr = vsub( s->o, r.o );
	double a = dot( r.d, r.d );
	double b = dot( r.d, sr );
	double c = dot( sr, sr ) - s->rad * s->rad;
	double d = b * b - a * c;
	if ( d <= 0 ) return 0;
	d = sqrt( d );
	d = ( b - d ) / a;
	if ( d <= EPS ) {
		d = ( b + d ) / a;
		if ( d <= EPS ) return 0;
	}

	s->h = raypos( r, d );
	vec outward_n = vdivd( vsub( s->h, s->o ), s->rad );
	bool frontface = dot( r.d, outward_n ) < 0;
	s->n = frontface ? outward_n : vmuld( outward_n, -1 );

	return 1;
}

bool diffuse_scatter( sphere s, ray* bounce_ray ) {
	vec	scatter_dir = vadd( s.n, random_unit_vector() );

	if ( vecerr( scatter_dir ) ) {
		scatter_dir = s.n;
	}

	*bounce_ray = ( ray ){ s.h, scatter_dir };
	return 1;
}

bool metal_scatter( ray r, sphere s, ray* bounce_ray ) {
	vec reflect_dir = refl( r.d, s.n );

	*bounce_ray = ( ray ){ s.h, reflect_dir };
	return 1;
}

vec rgbcolor( int r, int g, int b ) {
	double _r = ( double )r / 255;
	double _g = ( double )g / 255;
	double _b = ( double )b / 255;

	return ( vec ) { _r, _g, _b };
}

vec trace( ray r, int depth, int scenesize, sphere scn[], vec sky ) {
	vec zenith = rgbcolor( 210 * 0.8, 235 * 0.8, 247 * 0.8 );
	vec horizon = rgbcolor( 248 * 0.8, 241 * 0.8, 233 * 0.8 );

	if ( depth <= 0 ) {
		return ( vec ) { 0, 0, 0 };
	}

	for ( int i = 0; i < scenesize; i++ ) {
		if ( isects( &scn[i], r ) ) {
			switch ( scn[i].m ) {
				case Diffuse:
				{
					ray path;
					vec emission_color = scn[i].e;

					if ( !diffuse_scatter( scn[i], &path ) ) {
						return emission_color;
					}

					vec color_scatter = vmul( scn[i].c, trace( path, depth - 1, scenesize, scn, sky ) );
					return vadd( emission_color, color_scatter );
				}
			}
		}
	}

	return sky;

	vec unit_dir = unitvec( r.d );
	double a = 0.85 * ( unit_dir.y + 1.1 );
	//return vadd(vmuld(horizon, (1.0 - a)), vmuld(zenith, a));
}

int main( int argc, char** argv ) {
	initrand();
	static sphere scene[MAXSCENE];
	int imgw, imgh, ss, spp;
	vec sky, eye;

	if ( loadscene( "test.sdn", scene, 0, &ss, &sky, &eye, &imgw, &imgh, &spp ) == 1 ) {
		printf( "Failed to load scene.\n" );
		return EXIT_FAILURE;
	};

	double fl = 1.3, vh = 2.0, vw = vh * ( ( double )imgw / imgh ), dx = vw / imgw, dy = vh / imgh;
	vec prm_ray = { ( eye.x - ( vw * 0.5 ) ) + ( dx * 0.5 ),
		( eye.y + ( vh * 0.5 ) ) - ( dy * 0.5 ), eye.z - fl };

	FILE* f = fopen( "out.ppm", "w" );
	fprintf( f, "P3\n%i %i\n255\n", imgw, imgh );

	for ( int y = 0; y < imgh; y++ ) {
		printf( "\rRendering image . . . %5.f%%", 100. * y / ( imgh - 1 ) );
		for ( int x = 0; x < imgw; x++ ) {
			vec c = { 0,0,0 };
			vec next = { prm_ray.x + ( dx * x ), prm_ray.y - ( dy * y ), prm_ray.z };
			vec v = vsub( next, eye );
			ray r = { eye, v };

			for ( int samp = 0; samp < spp; samp++ ) {
				c = ( vec ){
					c.x += ( trace( r, 3, ss, scene, sky ).x ) / spp,
					c.y += ( trace( r, 3, ss, scene, sky ).y ) / spp,
					c.z += ( trace( r, 3, ss, scene, sky ).z ) / spp,
				};
			}
			//vdebug(c);
			c.x = gamma( c.x );
			c.y = gamma( c.y );
			c.z = gamma( c.z );
			fprintf( f, "%i %i %i ", ( int )( clamp( c.x ) * 255 ), ( int )( clamp( c.y ) * 255 ), ( int )( clamp( c.z ) * 255 ) );
		}
	}

	fclose( f );
}
