/* Include guards */

/*This is more opengl compatible version of the original */
#ifndef VECTORMATH_H
#define VECTORMATH_H




/* Defines three dimensional vector and basic vector operations on it*/

struct vector3f
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

typedef struct vector3f v3f;

v3f vInit(GLfloat x, GLfloat y, GLfloat z)
{
	v3f result;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}

/* vector space properties: commutativity, associativity  of addition, additive identity and existence of additive inverse (1-4) */
v3f vCopy(v3f vector0)
{
	v3f result;
	result.x = vector0.x;
	result.y = vector0.y;
	result.z = vector0.z;

	return result;
}


v3f vAdd(v3f vector0, v3f vector1)
{
	v3f result;
	result.x = vector0.x + vector1.x;
	result.y = vector0.y + vector1.y;
	result.z = vector0.z + vector1.z;
	return result;
}

/* Associativity of scalar multiplication (5)*/
/*Distributivity of scalar multiplication and scalar sums, scalar multiplication identity (6-8)*/
v3f vMultiply(v3f vector0, GLfloat scalar)
{
	v3f result;
	result.x = vector0.x * scalar; 
	result.y = vector0.y * scalar; 
	result.z = vector0.z * scalar;
	return result;
}

/* other nice functions */

GLfloat vDotProduct(v3f vector0, v3f vector1)
{
	return vector0.x * vector1.x + vector0.y * vector1.y + vector0.z * vector1.z;
}

GLfloat vLength(v3f vector0)
{
	return sqrt(vDotProduct(vector0, vector0));
}

void vNormalize(v3f* vector0)
{
	GLfloat length = vLength(*vector0);
	vector0->x = vector0->x / length;
	vector0->y = vector0->y / length; 
	vector0->z = vector0->z / length;  
}
/* cross product:
 | i  j  k  |
 | ax ay az |
 | bx by bz |

NOTE: this is not normalized
*/



v3f vCrossProduct(v3f a, v3f b)
{
	v3f result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = -( a.x*b.z - a.z*b.x );
	result.z = a.x*b.z - a.z*b.x;
	return result; 
}


/* End of include guard VECTORMATH_H*/
#endif