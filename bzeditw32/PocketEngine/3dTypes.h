// 3dtypes.x


#ifdef POCKETENGINE_EXPORTS
#define THREEDUTILS_API __declspec(dllexport)
#else
#define THREEDUTILS_API __declspec(dllimport)
#endif

#ifndef _3DTYPES_H_
#define _3DTYPES_H_

#define UINT	unsigned int

	// 3d variables
	typedef struct
	{
		float	x,y,z;
	}trVertex3D;

	typedef struct
	{
		float	s,t;
	}trVertex2D;

	typedef struct
	{
		float	r,g,b;
	}trColor;

	typedef struct
	{
		long			aVerts[3];
		long			aNorms[3];
		long			aTCs[3];
		trVertex3D		rFaceNormal;
	}trTriangle;

	#include "math.h"

	/*--------------------------------- trVertex3D operators -----------------------------------------*/

	/*--------------------------------- operator - -----------------------------------------*/

	inline trVertex3D operator - (const trVertex3D &p0)
	{
		trVertex3D p1;

		p1.x = - p0.x;
		p1.y = - p0.y;
		p1.z = - p0.z;

		return p1;
	}

	/*--------------------------------- operator + -----------------------------------------*/

	inline trVertex3D operator + (const trVertex3D &p0, const trVertex3D &p1)
	{
		trVertex3D p2;

		p2.x = p0.x + p1.x;
		p2.y = p0.y + p1.y;
		p2.z = p0.z + p1.z;

		return p2;
	}

	/*--------------------------------- operator - -----------------------------------------*/

	inline trVertex3D operator - (const trVertex3D &p0, const trVertex3D &p1)
	{
		trVertex3D p2;

		p2.x = p0.x - p1.x;
		p2.y = p0.y - p1.y;
		p2.z = p0.z - p1.z;

		return p2;
	}

	/*--------------------------------- operator * -----------------------------------------*/

	inline trVertex3D operator * (const trVertex3D &p0, float scalar)
	{
		trVertex3D p2;

		p2.x = p0.x * scalar;
		p2.y = p0.y * scalar;
		p2.z = p0.z * scalar;

		return p2;
	}

	/*--------------------------------- operator / -----------------------------------------*/

	inline trVertex3D operator / (const trVertex3D &p0, float scalar)
	{
		trVertex3D p2;

		scalar = 1.0f / scalar;
		p2.x = p0.x * scalar;
		p2.y = p0.y * scalar;
		p2.z = p0.z * scalar;

		return p2;
	}

	/*--------------------------------- operator * -----------------------------------------*/

	inline trVertex3D operator * (float scalar, const trVertex3D &p0)
	{
		trVertex3D p2;

		p2.x = p0.x * scalar;
		p2.y = p0.y * scalar;
		p2.z = p0.z * scalar;

		return p2;
	}

	/*--------------------------------- operator / -----------------------------------------*/

	inline trVertex3D operator / (float scalar, const trVertex3D &p0)
	{
		trVertex3D p2;

		scalar = 1.0f / scalar;
		p2.x = p0.x * scalar;
		p2.y = p0.y * scalar;
		p2.z = p0.z * scalar;

		return p2;
	}

	/*--------------------------------- operator += -----------------------------------------*/

	inline void operator += (trVertex3D &p0, const trVertex3D &p1)
	{
		p0.x += p1.x;
		p0.y += p1.y;
		p0.z += p1.z;
	}

	/*--------------------------------- operator -= -----------------------------------------*/

	inline void operator -= (trVertex3D &p0, const trVertex3D &p1)
	{
		p0.x -= p1.x;
		p0.y -= p1.y;
		p0.z -= p1.z;
	}

	/*--------------------------------- operator *= -----------------------------------------*/

	inline void operator *= (trVertex3D &p0, const trVertex3D &p1)
	{
		p0.x *= p1.x;
		p0.y *= p1.y;
		p0.z *= p1.z;
	}

	/*--------------------------------- operator /= -----------------------------------------*/

	inline void operator /= (trVertex3D &p0, const trVertex3D &p1)
	{
		p0.x /= p1.x;
		p0.y /= p1.y;
		p0.z /= p1.z;
	}

	/*--------------------------------- operator *= -----------------------------------------*/

	inline void operator *= (trVertex3D &p0, float scalar)
	{
		p0.x *= scalar;
		p0.y *= scalar;
		p0.z *= scalar;
	}

	/*--------------------------------- operator /= -----------------------------------------*/

	inline void operator /= (trVertex3D &p0, float scalar)
	{
		scalar = 1.0f / scalar;
		p0.x *= scalar;
		p0.y *= scalar;
		p0.z *= scalar;
	}

	/*------------------------------------- dot ---------------------------------------------*/

	inline float dot(const trVertex3D &lhs, const trVertex3D &rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	/*------------------------------------- dot_2d ---------------------------------------------*/

	inline float dot_2d(const trVertex3D &lhs, const trVertex3D &rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	/*------------------------------------- cross ---------------------------------------------*/

	inline trVertex3D& cross(const trVertex3D &lhs, const trVertex3D &rhs, trVertex3D &result)
	{
		float temp_x, temp_y;		// this allows result to be same as lhs or rhs

		temp_x = lhs.z * rhs.y - lhs.y * rhs.z;
		temp_y = lhs.z * rhs.x - lhs.x * rhs.z;
		result.z = lhs.x * rhs.y - lhs.y * rhs.x;
		result.x = temp_x;
		result.y = temp_y;
		return result;
	}

	/*------------------------------------- cross_2d ---------------------------------------------*/

	inline float cross_2d(const trVertex3D &lhs, const trVertex3D &rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}

	/*------------------------------------- get_mag ---------------------------------------------*/

	inline float get_mag(const trVertex3D &vec)
	{
		return (float)sqrt(dot(vec, vec));
	}

	/*------------------------------------- get_mag_2d ---------------------------------------------*/

	inline float get_mag_2d(const trVertex3D &vec)
	{
		return (float)sqrt(dot_2d(vec, vec));
	}

	/*------------------------------------- set_mag ---------------------------------------------*/

	inline bool set_mag(trVertex3D &vec, float mag)
	{
		float old_mag = get_mag(vec);
		if (old_mag < 1.0e-15)
		{
			vec.x = mag;
			vec.z = vec.y = 0;
			return false;
		}
		vec *= mag / old_mag;
		return true;
	}

	/*------------------------------------- set_mag_2d ---------------------------------------------*/

	inline bool set_mag_2d(trVertex3D &vec, float mag)
	{
		float old_mag = get_mag_2d(vec);
		if (old_mag < 1.0e-15)
		{
			vec.x = mag;
			vec.z = vec.y = 0;
			return false;
		}
		mag /= old_mag;
		vec.x *= mag;
		vec.y *= mag;
		return true;
	}
	
	#define XFORM_TRANS		1
	#define XFORM_SCALE		2
	#define XFORM_ROT		4

	#pragma pack (push, matrix_def, 1)			// trMatrix is 1 byte aligned

	typedef struct
	{
		short flags;
		float el[3][4];
	}
	trMatrix;

	#pragma pack (pop, matrix_def)				// restore struct alignment

	typedef enum
	{
		eXY, 
		eXZ, 
		eYZ,
		eXR,
		eXD,
		eNonPrimary
	}
	tePlanes;

	THREEDUTILS_API trMatrix *ClassifyMatrix(trMatrix*);
	THREEDUTILS_API trMatrix *NormalizeMatrix(trMatrix*);
	THREEDUTILS_API trMatrix *GetIdentityMatrix(trMatrix*);
	THREEDUTILS_API bool MatrixPlanar(const trMatrix*, bool*);
	THREEDUTILS_API tePlanes MatrixPlane(const trMatrix*);
	THREEDUTILS_API bool MatrixScaleNonUniform(const trMatrix*);
	THREEDUTILS_API bool MatrixScaleNonUniform2d(const trMatrix*);
	THREEDUTILS_API trVertex3D *MatrixScale(const trMatrix*, trVertex3D*);
	THREEDUTILS_API bool EqualMatrix(const trMatrix*, const trMatrix*);
	THREEDUTILS_API bool SamePlane(const trMatrix*, const trMatrix*, bool*);
	THREEDUTILS_API trMatrix *TranslateMatrix(trMatrix*, const trVertex3D*);
	THREEDUTILS_API trMatrix *TranslationMatrix(trMatrix*, const trVertex3D*);
	THREEDUTILS_API trMatrix *ScaleMatrix(trMatrix*, const trVertex3D*);
	THREEDUTILS_API trMatrix *ScalingMatrix(trMatrix*, const trVertex3D*);
	THREEDUTILS_API trMatrix *RotationMatrix(trMatrix*, const trVertex3D*, float, float);
	THREEDUTILS_API trMatrix *RotationMatrix(trMatrix*, float, float);
	THREEDUTILS_API trMatrix *Rotate2dMatrix(trMatrix*, float, const trVertex3D*);
	THREEDUTILS_API trMatrix *MirrorMatrix(trMatrix*, const trVertex3D*, const trVertex3D*);
	THREEDUTILS_API trMatrix *ForceDepthMatrix(trMatrix*, double);
	THREEDUTILS_API trMatrix *ConcatMatrices(trMatrix*, const trMatrix*);
	THREEDUTILS_API trMatrix *InvertMatrix(trMatrix*);
	THREEDUTILS_API trVertex3D *TransformPos(trVertex3D*, const trMatrix*);
	THREEDUTILS_API trVertex3D *TransformPos2d(trVertex3D*, const trMatrix*);
	THREEDUTILS_API trVertex3D *TransformNorm(trVertex3D*, const trMatrix*);
	THREEDUTILS_API trVertex3D *TransformNorm2d(trVertex3D*, const trMatrix*);
	THREEDUTILS_API trMatrix *MatrixFromAxes(trMatrix*, const trVertex3D*, const trVertex3D*, const trVertex3D*);
	THREEDUTILS_API trMatrix *DifferenceMatrix(trMatrix*, const trVertex3D*, const trVertex3D*, const trVertex3D*, const trVertex3D*, const trVertex3D*, const trVertex3D*);

#endif//_3DTYPES_H_