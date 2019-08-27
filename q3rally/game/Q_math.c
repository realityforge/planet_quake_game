// Copyright (C) 1999-2000 Id Software, Inc.
//
// q_math.c -- stateless support routines that are included in each code module
#include "q_shared.h"


vec3_t	vec3_origin = {0,0,0};
vec3_t	axisDefault[3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };


vec4_t		colorBlack	= {0, 0, 0, 1};
vec4_t		colorRed	= {1, 0, 0, 1};
vec4_t		colorGreen	= {0, 1, 0, 1};
vec4_t		colorBlue	= {0, 0, 1, 1};
vec4_t		colorYellow	= {1, 1, 0, 1};
vec4_t		colorMagenta= {1, 0, 1, 1};
vec4_t		colorCyan	= {0, 1, 1, 1};
vec4_t		colorWhite	= {1, 1, 1, 1};
vec4_t		colorLtGrey	= {0.75, 0.75, 0.75, 1};
vec4_t		colorMdGrey	= {0.5, 0.5, 0.5, 1};
vec4_t		colorDkGrey	= {0.25, 0.25, 0.25, 1};

vec4_t	g_color_table[8] =
	{
	{0.0, 0.0, 0.0, 1.0},
	{1.0, 0.0, 0.0, 1.0},
	{0.0, 1.0, 0.0, 1.0},
	{1.0, 1.0, 0.0, 1.0},
	{0.0, 0.0, 1.0, 1.0},
	{0.0, 1.0, 1.0, 1.0},
	{1.0, 0.0, 1.0, 1.0},
	{1.0, 1.0, 1.0, 1.0},
	};


vec3_t	bytedirs[NUMVERTEXNORMALS] =
{
{-0.525731f, 0.000000f, 0.850651f}, {-0.442863f, 0.238856f, 0.864188f}, 
{-0.295242f, 0.000000f, 0.955423f}, {-0.309017f, 0.500000f, 0.809017f}, 
{-0.162460f, 0.262866f, 0.951056f}, {0.000000f, 0.000000f, 1.000000f}, 
{0.000000f, 0.850651f, 0.525731f}, {-0.147621f, 0.716567f, 0.681718f}, 
{0.147621f, 0.716567f, 0.681718f}, {0.000000f, 0.525731f, 0.850651f}, 
{0.309017f, 0.500000f, 0.809017f}, {0.525731f, 0.000000f, 0.850651f}, 
{0.295242f, 0.000000f, 0.955423f}, {0.442863f, 0.238856f, 0.864188f}, 
{0.162460f, 0.262866f, 0.951056f}, {-0.681718f, 0.147621f, 0.716567f}, 
{-0.809017f, 0.309017f, 0.500000f},{-0.587785f, 0.425325f, 0.688191f}, 
{-0.850651f, 0.525731f, 0.000000f},{-0.864188f, 0.442863f, 0.238856f}, 
{-0.716567f, 0.681718f, 0.147621f},{-0.688191f, 0.587785f, 0.425325f}, 
{-0.500000f, 0.809017f, 0.309017f}, {-0.238856f, 0.864188f, 0.442863f}, 
{-0.425325f, 0.688191f, 0.587785f}, {-0.716567f, 0.681718f, -0.147621f}, 
{-0.500000f, 0.809017f, -0.309017f}, {-0.525731f, 0.850651f, 0.000000f}, 
{0.000000f, 0.850651f, -0.525731f}, {-0.238856f, 0.864188f, -0.442863f}, 
{0.000000f, 0.955423f, -0.295242f}, {-0.262866f, 0.951056f, -0.162460f}, 
{0.000000f, 1.000000f, 0.000000f}, {0.000000f, 0.955423f, 0.295242f}, 
{-0.262866f, 0.951056f, 0.162460f}, {0.238856f, 0.864188f, 0.442863f}, 
{0.262866f, 0.951056f, 0.162460f}, {0.500000f, 0.809017f, 0.309017f}, 
{0.238856f, 0.864188f, -0.442863f},{0.262866f, 0.951056f, -0.162460f}, 
{0.500000f, 0.809017f, -0.309017f},{0.850651f, 0.525731f, 0.000000f}, 
{0.716567f, 0.681718f, 0.147621f}, {0.716567f, 0.681718f, -0.147621f}, 
{0.525731f, 0.850651f, 0.000000f}, {0.425325f, 0.688191f, 0.587785f}, 
{0.864188f, 0.442863f, 0.238856f}, {0.688191f, 0.587785f, 0.425325f}, 
{0.809017f, 0.309017f, 0.500000f}, {0.681718f, 0.147621f, 0.716567f}, 
{0.587785f, 0.425325f, 0.688191f}, {0.955423f, 0.295242f, 0.000000f}, 
{1.000000f, 0.000000f, 0.000000f}, {0.951056f, 0.162460f, 0.262866f}, 
{0.850651f, -0.525731f, 0.000000f},{0.955423f, -0.295242f, 0.000000f}, 
{0.864188f, -0.442863f, 0.238856f}, {0.951056f, -0.162460f, 0.262866f}, 
{0.809017f, -0.309017f, 0.500000f}, {0.681718f, -0.147621f, 0.716567f}, 
{0.850651f, 0.000000f, 0.525731f}, {0.864188f, 0.442863f, -0.238856f}, 
{0.809017f, 0.309017f, -0.500000f}, {0.951056f, 0.162460f, -0.262866f}, 
{0.525731f, 0.000000f, -0.850651f}, {0.681718f, 0.147621f, -0.716567f}, 
{0.681718f, -0.147621f, -0.716567f},{0.850651f, 0.000000f, -0.525731f}, 
{0.809017f, -0.309017f, -0.500000f}, {0.864188f, -0.442863f, -0.238856f}, 
{0.951056f, -0.162460f, -0.262866f}, {0.147621f, 0.716567f, -0.681718f}, 
{0.309017f, 0.500000f, -0.809017f}, {0.425325f, 0.688191f, -0.587785f}, 
{0.442863f, 0.238856f, -0.864188f}, {0.587785f, 0.425325f, -0.688191f}, 
{0.688191f, 0.587785f, -0.425325f}, {-0.147621f, 0.716567f, -0.681718f}, 
{-0.309017f, 0.500000f, -0.809017f}, {0.000000f, 0.525731f, -0.850651f}, 
{-0.525731f, 0.000000f, -0.850651f}, {-0.442863f, 0.238856f, -0.864188f}, 
{-0.295242f, 0.000000f, -0.955423f}, {-0.162460f, 0.262866f, -0.951056f}, 
{0.000000f, 0.000000f, -1.000000f}, {0.295242f, 0.000000f, -0.955423f}, 
{0.162460f, 0.262866f, -0.951056f}, {-0.442863f, -0.238856f, -0.864188f}, 
{-0.309017f, -0.500000f, -0.809017f}, {-0.162460f, -0.262866f, -0.951056f}, 
{0.000000f, -0.850651f, -0.525731f}, {-0.147621f, -0.716567f, -0.681718f}, 
{0.147621f, -0.716567f, -0.681718f}, {0.000000f, -0.525731f, -0.850651f}, 
{0.309017f, -0.500000f, -0.809017f}, {0.442863f, -0.238856f, -0.864188f}, 
{0.162460f, -0.262866f, -0.951056f}, {0.238856f, -0.864188f, -0.442863f}, 
{0.500000f, -0.809017f, -0.309017f}, {0.425325f, -0.688191f, -0.587785f}, 
{0.716567f, -0.681718f, -0.147621f}, {0.688191f, -0.587785f, -0.425325f}, 
{0.587785f, -0.425325f, -0.688191f}, {0.000000f, -0.955423f, -0.295242f}, 
{0.000000f, -1.000000f, 0.000000f}, {0.262866f, -0.951056f, -0.162460f}, 
{0.000000f, -0.850651f, 0.525731f}, {0.000000f, -0.955423f, 0.295242f}, 
{0.238856f, -0.864188f, 0.442863f}, {0.262866f, -0.951056f, 0.162460f}, 
{0.500000f, -0.809017f, 0.309017f}, {0.716567f, -0.681718f, 0.147621f}, 
{0.525731f, -0.850651f, 0.000000f}, {-0.238856f, -0.864188f, -0.442863f}, 
{-0.500000f, -0.809017f, -0.309017f}, {-0.262866f, -0.951056f, -0.162460f}, 
{-0.850651f, -0.525731f, 0.000000f}, {-0.716567f, -0.681718f, -0.147621f}, 
{-0.716567f, -0.681718f, 0.147621f}, {-0.525731f, -0.850651f, 0.000000f}, 
{-0.500000f, -0.809017f, 0.309017f}, {-0.238856f, -0.864188f, 0.442863f}, 
{-0.262866f, -0.951056f, 0.162460f}, {-0.864188f, -0.442863f, 0.238856f}, 
{-0.809017f, -0.309017f, 0.500000f}, {-0.688191f, -0.587785f, 0.425325f}, 
{-0.681718f, -0.147621f, 0.716567f}, {-0.442863f, -0.238856f, 0.864188f}, 
{-0.587785f, -0.425325f, 0.688191f}, {-0.309017f, -0.500000f, 0.809017f}, 
{-0.147621f, -0.716567f, 0.681718f}, {-0.425325f, -0.688191f, 0.587785f}, 
{-0.162460f, -0.262866f, 0.951056f}, {0.442863f, -0.238856f, 0.864188f}, 
{0.162460f, -0.262866f, 0.951056f}, {0.309017f, -0.500000f, 0.809017f}, 
{0.147621f, -0.716567f, 0.681718f}, {0.000000f, -0.525731f, 0.850651f}, 
{0.425325f, -0.688191f, 0.587785f}, {0.587785f, -0.425325f, 0.688191f}, 
{0.688191f, -0.587785f, 0.425325f}, {-0.955423f, 0.295242f, 0.000000f}, 
{-0.951056f, 0.162460f, 0.262866f}, {-1.000000f, 0.000000f, 0.000000f}, 
{-0.850651f, 0.000000f, 0.525731f}, {-0.955423f, -0.295242f, 0.000000f}, 
{-0.951056f, -0.162460f, 0.262866f}, {-0.864188f, 0.442863f, -0.238856f}, 
{-0.951056f, 0.162460f, -0.262866f}, {-0.809017f, 0.309017f, -0.500000f}, 
{-0.864188f, -0.442863f, -0.238856f}, {-0.951056f, -0.162460f, -0.262866f}, 
{-0.809017f, -0.309017f, -0.500000f}, {-0.681718f, 0.147621f, -0.716567f}, 
{-0.681718f, -0.147621f, -0.716567f}, {-0.850651f, 0.000000f, -0.525731f}, 
{-0.688191f, 0.587785f, -0.425325f}, {-0.587785f, 0.425325f, -0.688191f}, 
{-0.425325f, 0.688191f, -0.587785f}, {-0.425325f, -0.688191f, -0.587785f}, 
{-0.587785f, -0.425325f, -0.688191f}, {-0.688191f, -0.587785f, -0.425325f}
};

//==============================================================

int		Q_rand( int *seed ) {
	*seed = (69069 * *seed + 1);
	return *seed;
}

float	Q_random( int *seed ) {
	return ( Q_rand( seed ) & 0xffff ) / (float)0x10000;
}

float	Q_crandom( int *seed ) {
	return 2.0 * ( Q_random( seed ) - 0.5 );
}

#ifdef __LCC__

int VectorCompare( const vec3_t v1, const vec3_t v2 ) {
	if (v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2]) {
		return 0;
	}			
	return 1;
}

vec_t VectorLength( const vec3_t v ) {
	return (vec_t)sqrt (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

vec_t VectorLengthSquared( const vec3_t v ) {
	return (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

vec_t Distance( const vec3_t p1, const vec3_t p2 ) {
	vec3_t	v;

	VectorSubtract (p2, p1, v);
	return VectorLength( v );
}

vec_t DistanceSquared( const vec3_t p1, const vec3_t p2 ) {
	vec3_t	v;

	VectorSubtract (p2, p1, v);
	return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}

// fast vector normalize routine that does not check to make sure
// that length != 0, nor does it return length, uses rsqrt approximation
void VectorNormalizeFast( vec3_t v )
{
	float ilength;

	ilength = Q_rsqrt( DotProduct( v, v ) );

	v[0] *= ilength;
	v[1] *= ilength;
	v[2] *= ilength;
}

void VectorInverse( vec3_t v ){
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

void CrossProduct( const vec3_t v1, const vec3_t v2, vec3_t cross ) {
	cross[0] = v1[1]*v2[2] - v1[2]*v2[1];
	cross[1] = v1[2]*v2[0] - v1[0]*v2[2];
	cross[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
#endif

//=======================================================

signed char ClampChar( int i ) {
	if ( i < -128 ) {
		return -128;
	}
	if ( i > 127 ) {
		return 127;
	}
	return i;
}

signed short ClampShort( int i ) {
	if ( i < -32768 ) {
		return -32768;
	}
	if ( i > 0x7fff ) {
		return 0x7fff;
	}
	return i;
}


// this isn't a real cheap function to call!
int DirToByte( vec3_t dir ) {
	int		i, best;
	float	d, bestd;

	if ( !dir ) {
		return 0;
	}

	bestd = 0;
	best = 0;
	for (i=0 ; i<NUMVERTEXNORMALS ; i++)
	{
		d = DotProduct (dir, bytedirs[i]);
		if (d > bestd)
		{
			bestd = d;
			best = i;
		}
	}

	return best;
}

void ByteToDir( int b, vec3_t dir ) {
	if ( b < 0 || b >= NUMVERTEXNORMALS ) {
		VectorCopy( vec3_origin, dir );
		return;
	}
	VectorCopy (bytedirs[b], dir);
}


unsigned ColorBytes3 (float r, float g, float b) {
	unsigned	i;

	( (byte *)&i )[0] = r * 255;
	( (byte *)&i )[1] = g * 255;
	( (byte *)&i )[2] = b * 255;

	return i;
}

unsigned ColorBytes4 (float r, float g, float b, float a) {
	unsigned	i;

	( (byte *)&i )[0] = r * 255;
	( (byte *)&i )[1] = g * 255;
	( (byte *)&i )[2] = b * 255;
	( (byte *)&i )[3] = a * 255;

	return i;
}

float NormalizeColor( const vec3_t in, vec3_t out ) {
	float	max;
	
	max = in[0];
	if ( in[1] > max ) {
		max = in[1];
	}
	if ( in[2] > max ) {
		max = in[2];
	}

	if ( !max ) {
		VectorClear( out );
	} else {
		out[0] = in[0] / max;
		out[1] = in[1] / max;
		out[2] = in[2] / max;
	}
	return max;
}


/*
=====================
PlaneFromPoints

Returns false if the triangle is degenrate.
The normal will point out of the clock for clockwise ordered points
=====================
*/
qboolean PlaneFromPoints( vec4_t plane, const vec3_t a, const vec3_t b, const vec3_t c ) {
	vec3_t	d1, d2;

	VectorSubtract( b, a, d1 );
	VectorSubtract( c, a, d2 );
	CrossProduct( d2, d1, plane );
	if ( VectorNormalize( plane ) == 0 ) {
		return qfalse;
	}

	plane[3] = DotProduct( a, plane );
	return qtrue;
}

/*
===============
RotatePointAroundVector

This is not implemented very well...
===============
*/
void RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point,
							 float degrees ) {
	float	m[3][3];
	float	im[3][3];
	float	zrot[3][3];
	float	tmpmat[3][3];
	float	rot[3][3];
	int	i;
	vec3_t vr, vup, vf;
	float	rad;

	vf[0] = dir[0];
	vf[1] = dir[1];
	vf[2] = dir[2];

	PerpendicularVector( vr, dir );
	CrossProduct( vr, vf, vup );

	m[0][0] = vr[0];
	m[1][0] = vr[1];
	m[2][0] = vr[2];

	m[0][1] = vup[0];
	m[1][1] = vup[1];
	m[2][1] = vup[2];

	m[0][2] = vf[0];
	m[1][2] = vf[1];
	m[2][2] = vf[2];

	memcpy( im, m, sizeof( im ) );

	im[0][1] = m[1][0];
	im[0][2] = m[2][0];
	im[1][0] = m[0][1];
	im[1][2] = m[2][1];
	im[2][0] = m[0][2];
	im[2][1] = m[1][2];

	memset( zrot, 0, sizeof( zrot ) );
	zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

	rad = DEG2RAD( degrees );
	zrot[0][0] = cos( rad );
	zrot[0][1] = sin( rad );
	zrot[1][0] = -sin( rad );
	zrot[1][1] = cos( rad );

	MatrixMultiply( m, zrot, tmpmat );
	MatrixMultiply( tmpmat, im, rot );

	for ( i = 0; i < 3; i++ ) {
		dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
	}
}

/*
===============
RotateAroundDirection
===============
*/
void RotateAroundDirection( vec3_t axis[3], float yaw ) {

	// create an arbitrary axis[1] 
	PerpendicularVector( axis[1], axis[0] );

	// rotate it around axis[0] by yaw
	if ( yaw ) {
		vec3_t	temp;

		VectorCopy( axis[1], temp );
		RotatePointAroundVector( axis[1], axis[0], temp, yaw );
	}

	// cross to get axis[2]
	CrossProduct( axis[0], axis[1], axis[2] );
}



void vectoangles( const vec3_t value1, vec3_t angles ) {
	float	forward;
	float	yaw, pitch;
	
	if ( value1[1] == 0 && value1[0] == 0 ) {
		yaw = 0;
		if ( value1[2] > 0 ) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if ( value1[0] ) {
// STONELANCE
//			yaw = ( atan2 ( value1[1], value1[0] ) * 180 / M_PI );
			yaw = ( atan2 ( value1[1], value1[0] ) * M_180_PI );
// END
		}
		else if ( value1[1] > 0 ) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if ( yaw < 0 ) {
			yaw += 360;
		}

		forward = sqrt ( value1[0]*value1[0] + value1[1]*value1[1] );
// STONELANCE
//		pitch = ( atan2(value1[2], forward) * 180 / M_PI );
		pitch = ( atan2(value1[2], forward) * M_180_PI );
// END
		if ( pitch < 0 ) {
			pitch += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}


/*
=================
AnglesToAxis
=================
*/
void AnglesToAxis( const vec3_t angles, vec3_t axis[3] ) {
	vec3_t	right;

	// angle vectors returns "right" instead of "y axis"
	AngleVectors( angles, axis[0], right, axis[2] );
	VectorSubtract( vec3_origin, right, axis[1] );
}

void AxisClear( vec3_t axis[3] ) {
	axis[0][0] = 1;
	axis[0][1] = 0;
	axis[0][2] = 0;
	axis[1][0] = 0;
	axis[1][1] = 1;
	axis[1][2] = 0;
	axis[2][0] = 0;
	axis[2][1] = 0;
	axis[2][2] = 1;
}

void AxisCopy( vec3_t in[3], vec3_t out[3] ) {
	VectorCopy( in[0], out[0] );
	VectorCopy( in[1], out[1] );
	VectorCopy( in[2], out[2] );
}

void ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal )
{
	float d;
	vec3_t n;
	float inv_denom;

	inv_denom =  DotProduct( normal, normal );
#ifndef Q3_VM
	assert( Q_fabs(inv_denom) != 0.0f ); // bk010122 - zero vectors get here
#endif
	inv_denom = 1.0f / inv_denom;

	d = DotProduct( normal, p ) * inv_denom;

	n[0] = normal[0] * inv_denom;
	n[1] = normal[1] * inv_denom;
	n[2] = normal[2] * inv_denom;

	dst[0] = p[0] - d * n[0];
	dst[1] = p[1] - d * n[1];
	dst[2] = p[2] - d * n[2];
}

/*
================
MakeNormalVectors

Given a normalized forward vector, create two
other perpendicular vectors
================
*/
void MakeNormalVectors( const vec3_t forward, vec3_t right, vec3_t up) {
	float		d;

	// this rotate and negate guarantees a vector
	// not colinear with the original
	right[1] = -forward[0];
	right[2] = forward[1];
	right[0] = forward[2];

	d = DotProduct (right, forward);
	VectorMA (right, -d, forward, right);
	VectorNormalize (right);
	CrossProduct (right, forward, up);
}


void VectorRotate( vec3_t in, vec3_t matrix[3], vec3_t out )
{
	out[0] = DotProduct( in, matrix[0] );
	out[1] = DotProduct( in, matrix[1] );
	out[2] = DotProduct( in, matrix[2] );
}

//============================================================================

#if !idppc
/*
** float q_rsqrt( float number )
*/
float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;						// evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

#ifndef Q3_VM
#ifdef __linux__
	assert( !isnan(y) ); // bk010122 - FPE?
#endif
#endif
	return y;
}

float Q_fabs( float f ) {
	int tmp = * ( int * ) &f;
	tmp &= 0x7FFFFFFF;
	return * ( float * ) &tmp;
}
#endif

//============================================================

/*
===============
LerpAngle

===============
*/
float LerpAngle (float from, float to, float frac) {
	float	a;

	if ( to - from > 180 ) {
		to -= 360;
	}
	if ( to - from < -180 ) {
		to += 360;
	}
	a = from + frac * (to - from);

	return a;
}


/*
=================
AngleSubtract

Always returns a value from -180 to 180
=================
*/
float	AngleSubtract( float a1, float a2 ) {
	float	a;

	a = a1 - a2;
	while ( a > 180 ) {
		a -= 360;
	}
	while ( a < -180 ) {
		a += 360;
	}
	return a;
}


void AnglesSubtract( vec3_t v1, vec3_t v2, vec3_t v3 ) {
	v3[0] = AngleSubtract( v1[0], v2[0] );
	v3[1] = AngleSubtract( v1[1], v2[1] );
	v3[2] = AngleSubtract( v1[2], v2[2] );
}


float	AngleMod(float a) {
	a = (360.0/65536) * ((int)(a*(65536/360.0)) & 65535);
	return a;
}


/*
=================
AngleNormalize360

returns angle normalized to the range [0 <= angle < 360]
=================
*/
float AngleNormalize360 ( float angle ) {
	return (360.0 / 65536) * ((int)(angle * (65536 / 360.0)) & 65535);
}


/*
=================
AngleNormalize180

returns angle normalized to the range [-180 < angle <= 180]
=================
*/
float AngleNormalize180 ( float angle ) {
	angle = AngleNormalize360( angle );
	if ( angle > 180.0 ) {
		angle -= 360.0;
	}
	return angle;
}


/*
=================
AngleDelta

returns the normalized delta from angle1 to angle2
=================
*/
float AngleDelta ( float angle1, float angle2 ) {
	return AngleNormalize180( angle1 - angle2 );
}


//============================================================


/*
=================
SetPlaneSignbits
=================
*/
void SetPlaneSignbits (cplane_t *out) {
	int	bits, j;

	// for fast box on planeside test
	bits = 0;
	for (j=0 ; j<3 ; j++) {
		if (out->normal[j] < 0) {
			bits |= 1<<j;
		}
	}
	out->signbits = bits;
}


/*
==================
BoxOnPlaneSide

Returns 1, 2, or 1 + 2

// this is the slow, general version
int BoxOnPlaneSide2 (vec3_t emins, vec3_t emaxs, struct cplane_s *p)
{
	int		i;
	float	dist1, dist2;
	int		sides;
	vec3_t	corners[2];

	for (i=0 ; i<3 ; i++)
	{
		if (p->normal[i] < 0)
		{
			corners[0][i] = emins[i];
			corners[1][i] = emaxs[i];
		}
		else
		{
			corners[1][i] = emins[i];
			corners[0][i] = emaxs[i];
		}
	}
	dist1 = DotProduct (p->normal, corners[0]) - p->dist;
	dist2 = DotProduct (p->normal, corners[1]) - p->dist;
	sides = 0;
	if (dist1 >= 0)
		sides = 1;
	if (dist2 < 0)
		sides |= 2;

	return sides;
}

==================
*/
#if !( (defined __linux__ || __FreeBSD__) && (defined __i386__) && (!defined C_ONLY)) // rb010123
#if defined __LCC__ || defined C_ONLY || !id386

int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, struct cplane_s *p)
{
	float	dist1, dist2;
	int		sides;

// fast axial cases
	if (p->type < 3)
	{
		if (p->dist <= emins[p->type])
			return 1;
		if (p->dist >= emaxs[p->type])
			return 2;
		return 3;
	}

// general case
	switch (p->signbits)
	{
	case 0:
		dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		dist2 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		break;
	case 1:
		dist1 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		break;
	case 2:
		dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		dist2 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		break;
	case 3:
		dist1 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		break;
	case 4:
		dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		dist2 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		break;
	case 5:
		dist1 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emins[2];
		dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emaxs[2];
		break;
	case 6:
		dist1 = p->normal[0]*emaxs[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		dist2 = p->normal[0]*emins[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		break;
	case 7:
		dist1 = p->normal[0]*emins[0] + p->normal[1]*emins[1] + p->normal[2]*emins[2];
		dist2 = p->normal[0]*emaxs[0] + p->normal[1]*emaxs[1] + p->normal[2]*emaxs[2];
		break;
	default:
		dist1 = dist2 = 0;		// shut up compiler
		break;
	}

	sides = 0;
	if (dist1 >= p->dist)
		sides = 1;
	if (dist2 < p->dist)
		sides |= 2;

	return sides;
}
#else
#pragma warning( disable: 4035 )

__declspec( naked ) int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, struct cplane_s *p)
{
	static int bops_initialized;
	static int Ljmptab[8];

	__asm {

		push ebx
			
		cmp bops_initialized, 1
		je  initialized
		mov bops_initialized, 1
		
		mov Ljmptab[0*4], offset Lcase0
		mov Ljmptab[1*4], offset Lcase1
		mov Ljmptab[2*4], offset Lcase2
		mov Ljmptab[3*4], offset Lcase3
		mov Ljmptab[4*4], offset Lcase4
		mov Ljmptab[5*4], offset Lcase5
		mov Ljmptab[6*4], offset Lcase6
		mov Ljmptab[7*4], offset Lcase7
			
initialized:

		mov edx,dword ptr[4+12+esp]
		mov ecx,dword ptr[4+4+esp]
		xor eax,eax
		mov ebx,dword ptr[4+8+esp]
		mov al,byte ptr[17+edx]
		cmp al,8
		jge Lerror
		fld dword ptr[0+edx]
		fld st(0)
		jmp dword ptr[Ljmptab+eax*4]
Lcase0:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase1:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase2:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase3:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase4:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase5:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase6:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase7:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
LSetSides:
		faddp st(2),st(0)
		fcomp dword ptr[12+edx]
		xor ecx,ecx
		fnstsw ax
		fcomp dword ptr[12+edx]
		and ah,1
		xor ah,1
		add cl,ah
		fnstsw ax
		and ah,1
		add ah,ah
		add cl,ah
		pop ebx
		mov eax,ecx
		ret
Lerror:
		int 3
	}
}
#pragma warning( default: 4035 )

#endif
#endif

/*
=================
RadiusFromBounds
=================
*/
float RadiusFromBounds( const vec3_t mins, const vec3_t maxs ) {
	int		i;
	vec3_t	corner;
	float	a, b;

	for (i=0 ; i<3 ; i++) {
		a = fabs( mins[i] );
		b = fabs( maxs[i] );
		corner[i] = a > b ? a : b;
	}

	return VectorLength (corner);
}


void ClearBounds( vec3_t mins, vec3_t maxs ) {
	mins[0] = mins[1] = mins[2] = 99999;
	maxs[0] = maxs[1] = maxs[2] = -99999;
}

void AddPointToBounds( const vec3_t v, vec3_t mins, vec3_t maxs ) {
	if ( v[0] < mins[0] ) {
		mins[0] = v[0];
	}
	if ( v[0] > maxs[0]) {
		maxs[0] = v[0];
	}

	if ( v[1] < mins[1] ) {
		mins[1] = v[1];
	}
	if ( v[1] > maxs[1]) {
		maxs[1] = v[1];
	}

	if ( v[2] < mins[2] ) {
		mins[2] = v[2];
	}
	if ( v[2] > maxs[2]) {
		maxs[2] = v[2];
	}
}


vec_t VectorNormalize( vec3_t v ) {
	float	length, ilength;

	length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	length = sqrt (length);

	if ( length ) {
		ilength = 1/length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}
		
	return length;
}


vec_t VectorNormalize2( const vec3_t v, vec3_t out) {
	float	length, ilength;

	length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	length = sqrt (length);

	if (length)
	{
#ifndef Q3_VM // bk0101022 - FPE related
//	  assert( ((Q_fabs(v[0])!=0.0f) || (Q_fabs(v[1])!=0.0f) || (Q_fabs(v[2])!=0.0f)) );
#endif

		ilength = 1/length;
		out[0] = v[0]*ilength;
		out[1] = v[1]*ilength;
		out[2] = v[2]*ilength;
	} else {
#ifndef Q3_VM // bk0101022 - FPE related
//	  assert( ((Q_fabs(v[0])==0.0f) && (Q_fabs(v[1])==0.0f) && (Q_fabs(v[2])==0.0f)) );
#endif
		VectorClear( out );
	}
		
	return length;

}

void _VectorMA( const vec3_t veca, float scale, const vec3_t vecb, vec3_t vecc) {
	vecc[0] = veca[0] + scale*vecb[0];
	vecc[1] = veca[1] + scale*vecb[1];
	vecc[2] = veca[2] + scale*vecb[2];
}


vec_t _DotProduct( const vec3_t v1, const vec3_t v2 ) {
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

void _VectorSubtract( const vec3_t veca, const vec3_t vecb, vec3_t out ) {
	out[0] = veca[0]-vecb[0];
	out[1] = veca[1]-vecb[1];
	out[2] = veca[2]-vecb[2];
}

void _VectorAdd( const vec3_t veca, const vec3_t vecb, vec3_t out ) {
	out[0] = veca[0]+vecb[0];
	out[1] = veca[1]+vecb[1];
	out[2] = veca[2]+vecb[2];
}

void _VectorCopy( const vec3_t in, vec3_t out ) {
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}

void _VectorScale( const vec3_t in, vec_t scale, vec3_t out ) {
	out[0] = in[0]*scale;
	out[1] = in[1]*scale;
	out[2] = in[2]*scale;
}

void Vector4Scale( const vec4_t in, vec_t scale, vec4_t out ) {
	out[0] = in[0]*scale;
	out[1] = in[1]*scale;
	out[2] = in[2]*scale;
	out[3] = in[3]*scale;
}


int Q_log2( int val ) {
	int answer;

	answer = 0;
	while ( ( val>>=1 ) != 0 ) {
		answer++;
	}
	return answer;
}



/*
=================
PlaneTypeForNormal
=================
*/
/*
int	PlaneTypeForNormal (vec3_t normal) {
	if ( normal[0] == 1.0 )
		return PLANE_X;
	if ( normal[1] == 1.0 )
		return PLANE_Y;
	if ( normal[2] == 1.0 )
		return PLANE_Z;
	
	return PLANE_NON_AXIAL;
}
*/


// STONELANCE
/*
================================================================================
VectorNAN
================================================================================
*/
qboolean VectorNAN( const vec3_t vec ){
	if (IS_NAN(vec[0]) || IS_NAN(vec[1]) || IS_NAN(vec[2])){
		return qtrue;
	}

	return qfalse;
}


/*
================
MatrixMultiply
================
*/
void MatrixMultiply( float in1[3][3], float in2[3][3], float out[3][3]) {
	if( in1 == out || in2 == out )
	{
		float temp[3][3];

		temp[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
					in1[0][2] * in2[2][0];
		temp[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
					in1[0][2] * in2[2][1];
		temp[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
					in1[0][2] * in2[2][2];
		temp[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
					in1[1][2] * in2[2][0];
		temp[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
					in1[1][2] * in2[2][1];
		temp[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
					in1[1][2] * in2[2][2];
		temp[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
					in1[2][2] * in2[2][0];
		temp[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
					in1[2][2] * in2[2][1];
		temp[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
					in1[2][2] * in2[2][2];

		out[0][0] = temp[0][0];
		out[0][1] = temp[0][1];
		out[0][2] = temp[0][2];
		out[1][0] = temp[1][0];
		out[1][1] = temp[1][1];
		out[1][2] = temp[1][2];
		out[2][0] = temp[2][0];
		out[2][1] = temp[2][1];
		out[2][2] = temp[2][2];
	}
	else
	{
		out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
					in1[0][2] * in2[2][0];
		out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
					in1[0][2] * in2[2][1];
		out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
					in1[0][2] * in2[2][2];
		out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
					in1[1][2] * in2[2][0];
		out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
					in1[1][2] * in2[2][1];
		out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
					in1[1][2] * in2[2][2];
		out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
					in1[2][2] * in2[2][0];
		out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
					in1[2][2] * in2[2][1];
		out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
					in1[2][2] * in2[2][2];
	}
}


/*
================
MatrixTranspose

  Cant do MatrixTranspose(m, m);
================
*/
void MatrixTranspose( float in[3][3], float out[3][3] ) {
	out[0][0] = in[0][0];
	out[0][1] = in[1][0];
	out[0][2] = in[2][0];
	out[1][0] = in[0][1];
	out[1][1] = in[1][1];
	out[1][2] = in[2][1];
	out[2][0] = in[0][2];
	out[2][1] = in[1][2];
	out[2][2] = in[2][2];
}

/*
================
MatrixAdd
================
*/
void MatrixAdd( float in1[3][3], float in2[3][3], float out[3][3] ) {
	out[0][0] = in1[0][0] + in2[0][0];
	out[0][1] = in1[0][1] + in2[0][1];
	out[0][2] = in1[0][2] + in2[0][2];
	out[1][0] = in1[1][0] + in2[1][0];
	out[1][1] = in1[1][1] + in2[1][1];
	out[1][2] = in1[1][2] + in2[1][2];
	out[2][0] = in1[2][0] + in2[2][0];
	out[2][1] = in1[2][1] + in2[2][1];
	out[2][2] = in1[2][2] + in2[2][2];
}


/*
================
MatrixScale
================
*/
void MatrixScale( float in[3][3], float s, float out[3][3] ) {
	out[0][0] = in[0][0] * s;
	out[0][1] = in[0][1] * s;
	out[0][2] = in[0][2] * s;
	out[1][0] = in[1][0] * s;
	out[1][1] = in[1][1] * s;
	out[1][2] = in[1][2] * s;
	out[2][0] = in[2][0] * s;
	out[2][1] = in[2][1] * s;
	out[2][2] = in[2][2] * s;
}


/*
================
AnglesToOrientation
  
Converts car angles to an orientation matrix
================
*/
void AnglesToOrientation( const vec3_t angles, float t[3][3] ) {
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * M_PI_180;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * M_PI_180;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * M_PI_180;
	sr = sin(angle);
	cr = cos(angle);

	t[0][0] = (-1*sr*sp*cy+-1*cr*-sy);	t[1][0] = (-1*sr*sp*sy+-1*cr*cy);	t[2][0] = -1*sr*cp;
	t[0][1] = cp*cy;					t[1][1] = cp*sy;					t[2][1] = -sp;
	t[0][2] = (cr*sp*cy+-sr*-sy);		t[1][2] = (cr*sp*sy+-sr*cy);		t[2][2] = cr*cp;
}


/*
================
OrientationToVectors
  
Converts orientation matrix to angle vectors
================
*/
void OrientationToVectors( float t[3][3], vec3_t forward, vec3_t right, vec3_t up ) {
	right[0] = t[0][0];		right[1] = t[1][0];		right[2] = t[2][0];
	forward[0] = t[0][1];	forward[1] = t[1][1];	forward[2] = t[2][1];
	up[0] = t[0][2];		up[1] = t[1][2];		up[2] = t[2][2];

//	VectorNormalize(right);
//	VectorNormalize(forward);
//	VectorNormalize(up);
}


/*
================
AnglesToDeltaAngles
  
Converts orientation matrix to car angles
================
*/
void AnglesToDeltaAngles( vec3_t angles, const vec3_t w, vec3_t deltaAngles ) {
/*
	float	cr, cy;
	float	sr, sy;
	float	aaa;

	sy = sin( angles[YAW] * M_180_PI );
	cy = cos( angles[YAW] * M_180_PI );
	
	sr = sin( angles[ROLL] * M_180_PI );
	cr = cos( angles[ROLL] * M_180_PI );


// [ dpsi dtheta dphi ]T = M-1(theta,phi) * Omega
// 
// M-1(theta,phi) = [       cos(phi)/cos(theta)           sin(phi)/cos(theta)        0 ]
//                  [           -sin(phi)                      cos(phi)              0 ]
//                  [ cos(phi)*sin(theta)/cos(theta) sin(phi)*sin(theta)/cos(theta)  1 ]

	aaa = ( cr + sr ) / cy;
	delta_angles[0] = ( aaa ) * w[0];
	delta_angles[1] = ( cr - sr ) * w[1];
	delta_angles[2] = ( aaa * sy + 1 ) * w[2];
*/

	float	c1, c2, c3, s1, s2, s3;
	float	p, sp;

	c1 = cos( angles[0] * M_PI_180 );
	c2 = cos( angles[1] * M_PI_180 );
	c3 = cos( angles[2] * M_PI_180 );
	s1 = sin( angles[0] * M_PI_180 );
	s2 = sin( angles[1] * M_PI_180 );
	s3 = sin( angles[2] * M_PI_180 );

	p = 2.0f * acos( c1*c2*c3 - s1*s2*s3 );
	sp = p / sin( p / 2.0f ) ;

	deltaAngles[0] = (c1*s2*c3 + s1*c2*c3) * sp * M_180_PI;
	deltaAngles[1] = (s1*s2*c3 + c1*s2*s3) * sp * M_180_PI;
	deltaAngles[2] = (c1*c2*s3 + s1*s2*c3) * sp * M_180_PI;
}


/*
================
OrientationToDeltaAngles
  
Converts orientation matrix to car angles
================
*/
void OrientationToDeltaAngles( float t[3][3], const vec3_t w, vec3_t delta_angles ) {
//	vec3_t	forward, right, up;
//	float	cp, as;


//	OrientationToVectors(t, forward, right, up);
/*
	forward[0] = -w[2] * t[1][1] + w[1] * t[1][2];
	forward[1] =  w[2] * t[1][0] + w[0] * t[1][2];
	forward[2] = -w[1] * t[1][0] + w[0] * t[1][1];
	up[2] = -w[1] * t[2][0] + w[0] * t[2][1];
	right[2] = -w[1] * t[0][0] + w[0] * t[0][1];

	delta_angles[PITCH] = asin( -forward[2] ) * M_180_PI;
	if (up[2] < 0.0f)
		delta_angles[PITCH] = 180 - delta_angles[PITCH];

	cp = cos( delta_angles[PITCH] * M_PI_180 );
	if (cp){
		// fix small floating point errors that would cause it to
		// have asin() of a number > 1.00
		as = forward[1] / cp > 1.00f ? 1.00f : forward[1] / cp;
		as = as < -1.00f ? -1.00f : as;

		delta_angles[YAW] = asin(as) * M_180_PI;
		if (forward[0] < 0.0f)
			delta_angles[YAW] = 180 - delta_angles[YAW];
		if (up[2] < 0.0f)
			delta_angles[YAW] = 180 - delta_angles[YAW];

		as = -right[2] / cp > 1.00f ? 1.00f : -right[2] / cp;
		as = as < -1.00f ? -1.00f : as;
		delta_angles[ROLL] = asin(as) * M_180_PI;
	}
	else {
		delta_angles[YAW]=0;
		delta_angles[ROLL]=0;
	}

	// we still want yaw to be facing the front of the car so spin yaw
	// 180 and adjust pitch and roll to keep car in the same position
	if (fabs(delta_angles[PITCH]) > 90){
		delta_angles[YAW] += 180;
		delta_angles[PITCH] = 180 - delta_angles[PITCH];
		delta_angles[ROLL] += 180;
	}
*/
}


/*
================
OrientationToAngles
  
Converts orientation matrix to car angles
================
*/
void OrientationToAngles( float t[3][3], vec3_t angles ) {
	vec3_t	forward, right, up;
	float	cp, as;

	OrientationToVectors(t, forward, right, up);

	angles[PITCH] = asin(-forward[2]) * M_180_PI;
	if (up[2] < 0.0f)
		angles[PITCH] = 180 - angles[PITCH];

	cp = cos(angles[PITCH] * M_PI_180 );
	if (cp){
		// fix small floating point errors that would cause it to
		// have asin() of a number > 1.00
		as = forward[1] / cp > 1.00f ? 1.00f : forward[1] / cp;
		as = as < -1.00f ? -1.00f : as;

		angles[YAW] = asin(as) * M_180_PI;
		if (forward[0] < 0.0f)
			angles[YAW] = 180 - angles[YAW];
		if (up[2] < 0.0f)
			angles[YAW] = 180 - angles[YAW];

		as = -right[2] / cp > 1.00f ? 1.00f : -right[2] / cp;
		as = as < -1.00f ? -1.00f : as;
		angles[ROLL] = asin(as) * M_180_PI;
	}
	else {
		angles[YAW]=0;
		angles[ROLL]=0;
	}

	// we still want yaw to be facing the front of the car so spin yaw
	// 180 and adjust pitch and roll to keep car in the same position
	if (fabs(angles[PITCH]) > 90){
		angles[YAW] += 180;
		angles[PITCH] = 180 - angles[PITCH];
		angles[ROLL] += 180;
	}
}


/*
================
OrthonormalizeOrientation
  
Normalizes orientation matrix
================
*/
void OrthonormalizeOrientation( float t[3][3] ){
	vec3_t	x, y, z;

	VectorSet(x, t[0][0], t[1][0], t[2][0]);
	VectorSet(y, t[0][1], t[1][1], t[2][1]);

	// FIXME: check for 0 length?
	VectorNormalize(x);

	CrossProduct(x, y, z);
	VectorNormalize(z);

	CrossProduct(z, x, y);
	VectorNormalize(y);

	t[0][0] = x[0]; t[0][1] = y[0]; t[0][2] = z[0];
	t[1][0] = x[1]; t[1][1] = y[1]; t[1][2] = z[1];
	t[2][0] = x[2]; t[2][1] = y[2]; t[2][2] = z[2];
}

/*
================
QuaternionLengthSquared
  
Returns the DotProduct(q, q)

================
*/
float QuaternionLengthSquared( const vec4_t q ){
	return q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
}


/*
================
QuaternionLength
  
Returns the sqrt( DotProduct(q, q) )

================
*/
float QuaternionLength( const vec4_t q ){
	return sqrt( q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3] );
}


/*
================
QuaternionFastNormalize
  
  Normalizes quat avoiding a sqrt
  returns squared length of quaternion;

  Quaternion function code from racer
================
*/
float QuaternionFastNormalize( vec4_t quat )
{
	// Check if quaternion needs normalizing
	float		len = QuaternionLengthSquared( quat );
	if ( len == 0.0f )
	{
		// TODO: Make w 0 instead of this?
		quat[3] = 1.0f;
		quat[0] = quat[1] = quat[2] = 0.0f;
	}
	else if( len < 0.9999f || len > 1.0001f )
	{
		// Push quat back to 1.0 (avoid a sqrt())
		float	n = ( len + 1.0f ) / ( 2.0f * len );
		quat[0] *= n;
		quat[1] *= n;
		quat[2] *= n;
		quat[3] *= n;
	}

	return len;
}


/*
================
QuaternionNormalize
  
  Normalizes a quat using the normal sqrt method
  returns length of quaternion;

  Quaternion function code from racer
================
*/
float QuaternionNormalize( vec4_t quat )
{
	float	tmp, len;

	len = QuaternionLength( quat );
	if ( len == 0.0f )
	{
		// TODO: Make w 0 instead of this?
		quat[3] = 1.0f;
		quat[0] = quat[1] = quat[2] = 0.0f;
		return len;
	}

	tmp = 1.0f / len;
	quat[3] *= tmp;
	quat[0] *= tmp;
	quat[1] *= tmp;
	quat[2] *= tmp;

	return len;
}

/*
================
QuaternionMultiply
  
Multiplies two quaternions together

  Quaternion function code from skwid
================
*/
void QuaternionMultiply(const vec4_t in1, const vec4_t in2, vec4_t out){
/*
	// from racer
	w = in1[3] * in2[3] - in1[0] * in2[0] - in1[1] * in2[1] - in1[2] * in2[2];
	x = in1[3] * in2[0] + in1[0] * in2[3] + in1[1] * in2[2] - in1[2] * in2[1];
	y = in1[3] * in2[1] - in1[0] * in2[2] + in1[1] * in2[3] + in1[2] * in2[0];
	z = in1[3] * in2[2] + in1[0] * in2[1] - in1[1] * in2[0] + in1[2] * in2[3];
*/
/*	this crap doesnt work
	float	A, B, C, D, E, F, G, H;

	A = (q1[3] + q1[0]) * (q2[3] + q2[0]);
	B = (q1[2] - q1[1]) * (q2[1] - q2[2]);
	C = (q1[3] - q1[0]) * (q2[1] + q2[2]); 
	D = (q1[1] + q1[2]) * (q2[3] - q2[0]);
	E = (q1[0] + q1[2]) * (q2[0] + q2[1]);
	F = (q1[0] - q1[2]) * (q2[0] - q2[1]);
	G = (q1[3] + q1[1]) * (q2[3] - q2[2]);
	H = (q1[3] - q1[1]) * (q2[3] + q2[2]);

	res[0] = A - (E + F + G + H)/2; 
	res[1] = C + (E - F + G - H)/2; 
	res[2] = D + (E - F - G + H)/2;
	res[3] = B + (-E - F + G + H) /2;
*/

	vec3_t temp;  // in case of mulp(a, b, a)
	temp[0] = (in1[1] * in2[2] - in1[2] * in2[1]) + in2[3] * in1[0] + in1[3] * in2[0];
	temp[1] = (in1[2] * in2[0] - in1[0] * in2[2]) + in2[3] * in1[1] + in1[3] * in2[1];
	temp[2] = (in1[0] * in2[1] - in1[1] * in2[0]) + in2[3] * in1[2] + in1[3] * in2[2];

	out[3] = in1[3] * in2[3] - (in1[0] * in2[0] + in1[1] * in2[1] + in1[2] * in2[2]);
	out[0] = temp[0];
	out[1] = temp[1];
	out[2] = temp[2];
}


/*
================
QuaternionRotate
  
  Rotates quat by the amount specified in w*time

  w in body coords

  Note: out cannot be the same as quat

================
*/
void QuaternionRotate( const vec4_t quat, const vec3_t w, const float time, vec4_t out){
	vec3_t	tempVec;

	// divide by 2.0f is part of quaternion derivative calculation
	VectorScale( w, time / 2.0f, tempVec );

	out[0] = quat[0] + ( quat[3]*tempVec[0] - quat[2]*tempVec[1] + quat[1]*tempVec[2] );
	out[1] = quat[1] + ( quat[2]*tempVec[0] + quat[3]*tempVec[1] - quat[0]*tempVec[2] );
	out[2] = quat[2] + (-quat[1]*tempVec[0] + quat[0]*tempVec[1] + quat[3]*tempVec[2] );
	out[3] = quat[3] + (-quat[0]*tempVec[0] - quat[1]*tempVec[1] - quat[2]*tempVec[2] );
}


/*
================
QuaternionSLERP
  
  4D Spherically lerps from one quarternion to another.
  'from' and 'to' need to be normalized

  t = fraction of move from start quarternion to end quarternion [0 .. 1]
================
*/
void QuaternionSLERP(const vec4_t from, const vec4_t to, float t, vec4_t res){
/*
	// from racer
	float	theta, costheta, w1, w2, sintheta;

	costheta = from[0] * to[0] + from[1] * to[1] + from[2] * to[2] + from[3] * to[3];
	
	// OPTIMIZE: If costheta was squared then i could use the sin(t)^2 = 1-cos(t)^s
	//			 Is sintheta ever negative? If not then sin(t)^2 > 0 == sin(t) > 0
	theta = acos( costheta );
	sintheta = sin( theta );
	if( sintheta > 0.0f )
	{
		w1 = sin( (1.0f - t) * theta ) / sintheta;
		w2 = sin( t * theta ) / sintheta;
	}
	else
	{
		// They're the same quaternion, so who cares?
		w1 = 1.0f;
		w2 = 0.0f;
	}

	res[0] = w1 * from[0] + w2 * to[0];
	res[1] = w1 * from[1] + w2 * to[1];
	res[2] = w1 * from[2] + w2 * to[2];
	res[3] = w1 * from[3] + w2 * to[3];
*/

/*
//	Quaternion function from 'Rotating Objects Using Quaternions' by Nick Bobick
	vec4_t	to1;
	double	omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = from[0] * to[0] + from[1] * to[1] + from[2] * to[2] + from[3] * to[3];

	// adjust signs (if necessary)
	if ( cosom < 0.0 ){
		cosom = -cosom;
		to1[0] = -to[0];
		to1[1] = -to[1];
		to1[2] = -to[2];
		to1[3] = -to[3];
	} else  {
		to1[0] = to[0];
		to1[1] = to[1];
		to1[2] = to[2];
		to1[3] = to[3];
	}

	// calculate coefficients
	if ( (1.0 - cosom) > 0.1f ) {
		// standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	} else {        
		// "from" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation
		scale0 = 1.0 - t;
		scale1 = t;
	}

	// calculate final values
	res[0] = scale0 * from[0] + scale1 * to1[0];
	res[1] = scale0 * from[1] + scale1 * to1[1];
	res[2] = scale0 * from[2] + scale1 * to1[2];
	res[3] = scale0 * from[3] + scale1 * to1[3];
*/
}



/*
================
AnglesToQuaternion
  
  Converts euler angles to a quaternion
================
*/
void AnglesToQuaternion( const vec3_t angles, vec4_t quat){
	float	t[3][3];

	// OPTIMIZE: See if this can be simplified
	AnglesToOrientation( angles, t );
	OrientationToQuaternion( t, quat );

	// based on Quaternion function from 'Rotating Objects Using Quaternions' by Nick Bobick
/*
	float	cr, cp, cy, sr, sp, sy, spcy, spsy;
	float	tr, s;
	int		i, j, k;
	vec3_t	t[3];

	// calculate trig identities
	cr = cos(angles[ROLL] * M_PI / 180.0f);
	cp = cos(angles[PITCH] * M_PI / 180.0f);
	cy = cos(angles[YAW] * M_PI / 180.0f);

	sr = sin(angles[ROLL] * M_PI / 180.0f);
	sp = sin(angles[PITCH] * M_PI / 180.0f);
	sy = sin(angles[YAW] * M_PI / 180.0f);

	spcy = sp * cy;
	spsy = sp * sy;

	t[0][0] = (-sr*spcy + cr*sy);	t[1][0] = (-sr*spsy - cr*cy);	t[2][0] = -sr*cp;
	t[0][1] = cp*cy;				t[1][1] = cp*sy;				t[2][1] = -sp;
	t[0][2] = (cr*spcy + sr*sy);	t[1][2] = (cr*spsy - sr*cy);	t[2][2] = cr*cp;
*/
/*
	tr = t[0][0] + t[1][1] + t[2][2];

	// check the diagonal
	if (tr > 0.0) {
		s = sqrt (tr + 1.0);
		quat[3] = s / 2.0;
		s = 0.5 / s;
		quat[0] = (t[1][2] - t[2][1]) * s;
		quat[1] = (t[2][0] - t[0][2]) * s;
		quat[2] = (t[0][1] - t[1][0]) * s;
	} else {		
		// diagonal is negative
		i = 0;
		if (t[1][1] > t[0][0]) i = 1;
		if (t[2][2] > t[i][i]) i = 2;

		j = (i+1) % 3;
		k = (j+1) % 3;

		s = sqrt ((t[i][i] - (t[j][j] + t[k][k])) + 1.0);
      
		quat[i] = s * 0.5;
            
		if (s != 0.0) s = 0.5 / s;

		quat[3] = (t[j][k] - t[k][j]) * s;
		quat[j] = (t[i][j] + t[j][i]) * s;
		quat[k] = (t[i][k] + t[k][i]) * s;
	}
*/
/*
	// Quaternion function code from skwid
	vec4_t qy = { 0, sin(angles[YAW] * M_PI / 180.0f), 0, cos(angles[YAW] * M_PI / 180.0f) };
	vec4_t qp = { sin(angles[PITCH] * M_PI / 180.0f), 0, 0, cos(angles[PITCH] * M_PI / 180.0f) };
	vec4_t qr = { 0, 0, sin(angles[ROLL] * M_PI / 180.0f), cos(angles[ROLL] * M_PI / 180.0f) };

	Com_Printf("qy %f %f %f %f\n", qy[0], qy[1], qy[2], qy[3]);
	Com_Printf("qp %f %f %f %f\n", qp[0], qp[1], qp[2], qp[3]);
	Com_Printf("qr %f %f %f %f\n", qr[0], qr[1], qr[2], qr[3]);

	QuaternionMultiply( qy, qp, quat );

	Com_Printf("quat %f %f %f %f\n", quat[0], quat[1], quat[2], quat[3]);

	QuaternionMultiply( quat, qr, quat );
*/
}


/*
================
QuaternionToAngles
  
  Converts a quaternion to angles

================
*/
void QuaternionToAngles( const vec4_t quat, vec3_t angles ){
	float	t[3][3];

	// OPTIMIZE: See if this can be simplified, some parts of t dont need to be calculated
	QuaternionToOrientation( quat, t );
	OrientationToAngles( t, angles );

/*
	float	wx, wy, wz, xx, yy, yz, xy, xz, zz;
	float	x2, y2, z2;
	vec3_t	f, r, u;
	float	cp, as;

	// calculate coefficients
	x2 = quat[0] + quat[0];
	y2 = quat[1] + quat[1];
	z2 = quat[2] + quat[2];
	xx = quat[0] * x2;   xy = quat[0] * y2;   xz = quat[0] * z2;
	yy = quat[1] * y2;   yz = quat[1] * z2;   zz = quat[2] * z2;
	wx = quat[3] * x2;   wy = quat[3] * y2;   wz = quat[3] * z2;

	right[0] = 1.0 - (yy + zz);	right[1] = xy - wz;				right[2] = xz + wy;
	forward[0] = xy + wz;		forward[1] = 1.0 - (xx + zz);	forward[2] = yz - wx;
	up[0] = xz - wy;		 	up[1] = yz + wx;				up[2] = 1.0 - (xx + yy);
*/
/*
// Quaternion function code based on skwids
	float	wx, wy, wz, xx, yy, yz, xy, xz, zz;
	float	x2, y2, z2;
	vec3_t	f, r, u;
	float	cp, as;

	float	s = 2 / QuaternionNormal( quat );

	xx = quat[0] * quat[0];   xy = quat[0] * quat[1];   xz = quat[0] * quat[2];
	yy = quat[1] * quat[1];   yz = quat[1] * quat[2];   zz = quat[2] * quat[2];
	wx = quat[3] * quat[0];   wy = quat[3] * quat[1];   wz = quat[3] * quat[2];

//	r[0] = 1 - s * (yy + zz);	r[1] =     s * (xy - wz);
	r[2] =     s * (xz + wy);
	f[0] =     s * (xy + wz);	f[1] = 1 - s * (xx + zz);	f[2] =     s * (yz - wx);
//	u[0] =     s * (xz - wy);	u[1] =     s * (yz + wx);
	u[2] = 1 - s * (xx + yy);

	angles[PITCH] = asin(-f[2]) * 180.0f / M_PI;
	if (u[2] < 0.0f)
		angles[PITCH] = 180 - angles[PITCH];

	cp = cos(angles[PITCH] / 180.0f * M_PI);
	if (cp){
		// fix small floating point errors that would cause it to
		// have asin() of a number > 1.00
		as = f[1] / cp > 1.00f ? 1.00f : f[1] / cp;
		as = as < -1.00f ? -1.00f : as;

		angles[YAW] = asin(as) * 180.0f / M_PI;
		if (f[0] < 0.0f)
			angles[YAW] = 180 - angles[YAW];
		if (u[2] < 0.0f)
			angles[YAW] = 180 - angles[YAW];

		as = -r[2] / cp > 1.00f ? 1.00f : -r[2] / cp;
		as = as < -1.00f ? -1.00f : as;
		angles[ROLL] = asin(as) * 180 / M_PI;
	}
	else {
		angles[YAW]=0;
		angles[ROLL]=0;
	}

	// we still want yaw to be facing the front of the car so spin yaw
	// 180 and adjust pitch and roll to keep car in the same position
	if (fabs(angles[PITCH]) > 90){
		angles[YAW] += 180;
		angles[PITCH] = 180 - angles[PITCH];
		angles[ROLL] += 180;
	}
*/
}


/*
================
OrientationToQuaternion
  
  Converts orientation matrix to a quaternion
================
*/
void OrientationToQuaternion( float t[3][3], vec4_t quat ){

// from racer
	int		i;
	float	qw2, qx2, qy2, qz2, tmp;

	// Quaternion components squared
	qw2 = 0.25 * (t[0][0] + t[1][1] + t[2][2] + 1.0);
	qx2 = qw2 - 0.5 * (t[1][1] + t[2][2]);
	qy2 = qw2 - 0.5 * (t[2][2] + t[0][0]);
	qz2 = qw2 - 0.5 * (t[0][0] + t[1][1]);

	// Decide maximum magnitude component
	i = ( qw2 > qx2 ) ?
		( ( qw2 > qy2 ) ? (( qw2 > qz2 ) ? 0 : 3) : (( qy2 > qz2 ) ? 2 : 3)) :
		( ( qx2 > qy2 ) ? (( qx2 > qz2 ) ? 1 : 3) : (( qy2 > qz2 ) ? 2 : 3));

	// Compute signed quat components using numerically stable method
	switch( i )
	{
	case 0:
		quat[3] = sqrt(qw2); tmp = 0.25f / quat[3];
		quat[0] = (t[1][2] - t[2][1]) * tmp;
		quat[1] = (t[2][0] - t[0][2]) * tmp;
		quat[2] = (t[0][1] - t[1][0]) * tmp;
		break;

	case 1:
		quat[0] = sqrt(qx2); tmp = 0.25f / quat[0];
		quat[3] = (t[1][2] - t[2][1]) * tmp;
		quat[1] = (t[1][0] + t[0][1]) * tmp;
		quat[2] = (t[0][2] + t[2][0]) * tmp;
		break;

	case 2:
		quat[1] = sqrt(qy2); tmp = 0.25f / quat[1];
		quat[3] = (t[2][0] - t[0][2]) * tmp;
		quat[0] = (t[1][0] + t[0][1]) * tmp;
		quat[2] = (t[2][1] + t[1][2]) * tmp;
		break;

	case 3:
		quat[2] = sqrt(qz2); tmp = 0.25f / quat[2];
		quat[3] = (t[0][1] - t[1][0]) * tmp;
		quat[0] = (t[2][0] + t[0][2]) * tmp;
		quat[1] = (t[2][1] + t[1][2]) * tmp;
		break;
	}

	// Always keep all components positive
	// (note that scalar*quat is equivalent to quat, so q==-q)
	if ( i && quat[3] < 0.0f )
	{
		quat[3] = -quat[3];
		quat[0] = -quat[0];
		quat[1] = -quat[1];
		quat[2] = -quat[2];
	}

	// Normalize it to be safe
	QuaternionFastNormalize( quat );
//	tmp = 1.0f / sqrt( quat[3]*quat[3] + quat[0]*quat[0] + quat[1]*quat[1] + quat[2]*quat[2] );
//	quat[3] *= tmp;
//	quat[0] *= tmp;
//	quat[1] *= tmp;
//	quat[2] *= tmp;


// Quaternion function from 'Rotating Objects Using Quaternions' by Nick Bobick
/*
	float	tr, s;
	int		i, j, k;

	int nxt[3] = {1, 2, 0};

	tr = t[0][0] + t[1][1] + t[2][2];

	// check the diagonal
	if (tr > 0.0) {
		s = sqrt (tr + 1.0);
		quat[3] = s / 2.0;
		s = 0.5 / s;
		quat[0] = (t[1][2] - t[2][1]) * s;
		quat[1] = (t[2][0] - t[0][2]) * s;
		quat[2] = (t[0][1] - t[1][0]) * s;
	} else {		
		// diagonal is negative
		i = 0;
		if (t[1][1] > t[0][0]) i = 1;
		if (t[2][2] > t[i][i]) i = 2;

		j = nxt[i];
		k = nxt[j];

		s = sqrt ((t[i][i] - (t[j][j] + t[k][k])) + 1.0);
      
		quat[i] = s * 0.5;
            
		if (s != 0.0) s = 0.5 / s;

		quat[3] = (t[j][k] - t[k][j]) * s;
		quat[j] = (t[i][j] + t[j][i]) * s;
		quat[k] = (t[i][k] + t[k][i]) * s;
	}
*/

/*
	// thanks to skwid for the equations
	quat[3] = 0.5f * sqrt( t[0][0] + t[1][1] + t[2][2] + 1.0f );
	quat[0] = (t[1][2] - t[2][1]) / (4 * quat[3]);
	quat[1] = (t[2][0] - t[0][2]) / (4 * quat[3]);
	quat[2] = (t[0][1] - t[1][0]) / (4 * quat[3]);
*/

}


/*
================
QuaternionToOrientation
  
  Converts normalized quaternion to a orientation matrix
================
*/
void QuaternionToOrientation( const vec4_t quat, float t[3][3] ){
	float	xy, wz, xz, wy, yz, wx, x2, y2, z2;

// based on code from racer
	x2 = quat[0] * quat[0];
	y2 = quat[1] * quat[1];
	z2 = quat[2] * quat[2];

	xy = quat[0] * quat[1];
	wz = quat[3] * quat[2];
	xz = quat[0] * quat[2];
	wy = quat[3] * quat[1];
	yz = quat[1] * quat[2];
	wx = quat[3] * quat[0];
	t[0][0] = 1.0f - 2.0f * (y2 + z2);
	t[1][0] = 2.0f * (xy - wz);
	t[2][0] = 2.0f * (xz + wy);

	t[0][1] = 2.0f * (xy + wz);
	t[1][1] = 1.0f - 2.0f * (z2 + x2);
	t[2][1] = 2.0f * (yz - wx);

	t[0][2] = 2.0f * (xz - wy);
	t[1][2] = 2.0f * (yz + wx);
	t[2][2] = 1.0f - 2.0f * (x2 + y2);

/*
Quaternion function from 'Rotating Objects Using Quaternions' by Nick Bobick

	float	x2, y2, z2;

	// calculate coefficients
	x2 = quat[0] + quat[0];
	y2 = quat[1] + quat[1];
	z2 = quat[2] + quat[2];
	xx = quat[0] * x2;   xy = quat[0] * y2;   xz = quat[0] * z2;
	yy = quat[1] * y2;   yz = quat[1] * z2;   zz = quat[2] * z2;
	wx = quat[3] * x2;   wy = quat[3] * y2;   wz = quat[3] * z2;

	t[0][0] = 1.0 - (yy + zz);	t[1][0] = xy - wz;			t[2][0] = xz + wy;
 	t[0][1] = xy + wz;			t[1][1] = 1.0 - (xx + zz);	t[2][1] = yz - wx;
	t[0][2] = xz - wy;			t[1][2] = yz + wx;			t[2][2] = 1.0 - (xx + yy);
*/
/*
// Quaternion function code based on skwids
	float	s = 2 / QuaternionNormal( quat );

	xx = quat[0] * quat[0];   xy = quat[0] * quat[1];   xz = quat[0] * quat[2];
	yy = quat[1] * quat[1];   yz = quat[1] * quat[2];   zz = quat[2] * quat[2];
	wx = quat[3] * quat[0];   wy = quat[3] * quat[1];   wz = quat[3] * quat[2];

	t[0][0] = 1 - s * (yy + zz);	t[1][0] =     s * (xy - wz);	t[2][0] =     s * (xz + wy);
	t[0][1] =     s * (xy + wz);	t[1][1] = 1 - s * (xx + zz);	t[2][1] =     s * (yz - wx);
	t[0][2] =     s * (xz - wy);	t[1][2] =     s * (yz + wx);	t[2][2] = 1 - s * (xx + yy);
*/
}


/*
================
QuaternionL2ToOrientation
  
  Converts non-normalize quaternion to a orientation matrix
  'l2' is the squared length of the quaternion, used to get 't' orthogonal
================
*/
void QuaternionL2ToOrientation( const vec4_t quat, const float l2, float t[3][3] ){
	float	xy, wz, xz, wy, yz, wx, x2, y2, z2, s;

// based on code from racer
	x2 = quat[0] * quat[0];
	y2 = quat[1] * quat[1];
	z2 = quat[2] * quat[2];

	xy = quat[0] * quat[1];
	wz = quat[3] * quat[2];
	xz = quat[0] * quat[2];
	wy = quat[3] * quat[1];
	yz = quat[1] * quat[2];
	wx = quat[3] * quat[0];

	s = 2.0f / l2;
	t[0][0] = 1.0f - s * (y2 + z2);
	t[1][0] = s * (xy - wz);
	t[2][0] = s * (xz + wy);

	t[0][1] = s * (xy + wz);
	t[1][1] = 1.0f - s * (z2 + x2);
	t[2][1] = s * (yz - wx);

	t[0][2] = s * (xz - wy);
	t[1][2] = s * (yz + wx);
	t[2][2] = 1.0f - s * (x2 + y2);
}


/*
================
QuaternionToVectors
  
Converts a quaternion to angle vectors
================
*/
void QuaternionToVectors( const vec4_t quat, vec3_t forward, vec3_t right, vec3_t up ) {
	float	t[3][3];

	QuaternionToOrientation( quat, t );
	OrientationToVectors( t, forward, right, up );
/*
	float	wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	// calculate coefficients
	x2 = quat[0] + quat[0];
	y2 = quat[1] + quat[1];
	z2 = quat[2] + quat[2];
	xx = quat[0] * x2;   xy = quat[0] * y2;   xz = quat[0] * z2;
	yy = quat[1] * y2;   yz = quat[1] * z2;   zz = quat[2] * z2;
	wx = quat[3] * x2;   wy = quat[3] * y2;   wz = quat[3] * z2;

	right[0] = 1.0 - (yy + zz);	right[1] = xy - wz;				right[2] = xz + wy;
 	forward[0] = xy + wz;		forward[1] = 1.0 - (xx + zz);	forward[2] = yz - wx;
	up[0] = xz - wy;			up[1] = yz + wx;				up[2] = 1.0 - (xx + yy);

	VectorNormalize(right);
	VectorNormalize(forward);
	VectorNormalize(up);
*/
}
// END


void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up) {
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

// STONELANCE
//	angle = angles[YAW] * (M_PI*2 / 360);
	angle = angles[YAW] * M_PI_180;
// END
	sy = sin(angle);
	cy = cos(angle);
// STONELANCE
//	angle = angles[PITCH] * (M_PI*2 / 360);
	angle = angles[PITCH] * M_PI_180;
// END
	sp = sin(angle);
	cp = cos(angle);
// STONELANCE
//	angle = angles[ROLL] * (M_PI*2 / 360);
	angle = angles[ROLL] * M_PI_180;
// END
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (-1*sr*sp*sy+-1*cr*cy);
		right[2] = -1*sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy+-sr*-sy);
		up[1] = (cr*sp*sy+-sr*cy);
		up[2] = cr*cp;
	}
}

/*
** assumes "src" is normalized
*/
void PerpendicularVector( vec3_t dst, const vec3_t src )
{
	int	pos;
	int i;
	float minelem = 1.0F;
	vec3_t tempvec;

	/*
	** find the smallest magnitude axially aligned vector
	*/
	for ( pos = 0, i = 0; i < 3; i++ )
	{
		if ( fabs( src[i] ) < minelem )
		{
			pos = i;
			minelem = fabs( src[i] );
		}
	}
	tempvec[0] = tempvec[1] = tempvec[2] = 0.0F;
	tempvec[pos] = 1.0F;

	/*
	** project the point onto the plane defined by src
	*/
	ProjectPointOnPlane( dst, tempvec, src );

	/*
	** normalize the result
	*/
	VectorNormalize( dst );
}


