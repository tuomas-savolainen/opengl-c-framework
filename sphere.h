/* Include guards */
#ifndef SPHERE_H
#define SPHERE_H


#include "resources.h"

struct sphere3D
{
	Arrayf* oldVertices;
	Arrayf* currentVertices;
};

typedef struct sphere3D sphere3D;




void createNewTriangles(sphere3D* target, v3f input0, v3f input1, v3f input2, unsigned int index)
{
	/*
	Original triangle (input)
		2
	   / \
	  /   \
	 0-----1
	New triangle ABC (center)
		2
	   / \
	  A---C
	 / \ / \
	0---B---1

	*/


	

	/* A, B and C*/
	v3f vectorA;
	v3f vectorB;
	v3f vectorC;

	vectorA = vAdd(input0, input1);
	vectorB = vAdd(input1, input2);
	vectorC = vAdd(input2, input0);

	vNormalize(&vectorA);
	vNormalize(&vectorB);
	vNormalize(&vectorC);
	
	/*Now that vectors have length one the new triangles can be created */

		/* New triangles 0,1,2 and 3

		2
	   / \
	  A---C
and
	  A
	 / \ 
	0---B
and
	  A---C
	   \ /
	    B
and
	 C
	/ \
   B---1

	*/


   insertArrayf(target->currentVertices, input0.x, index + 0);
   insertArrayf(target->currentVertices, input0.y, index + 1);
   insertArrayf(target->currentVertices, input0.z, index + 2);

   insertArrayf(target->currentVertices, vectorA.x, index + 3);
   insertArrayf(target->currentVertices, vectorA.y, index + 4);
   insertArrayf(target->currentVertices, vectorA.z, index + 5);

   insertArrayf(target->currentVertices, vectorC.x, index + 6);
   insertArrayf(target->currentVertices, vectorC.y, index + 7);
   insertArrayf(target->currentVertices, vectorC.z, index + 8);




   insertArrayf(target->currentVertices, input1.x, index + 9);
   insertArrayf(target->currentVertices, input1.y, index + 10);
   insertArrayf(target->currentVertices, input1.z, index + 11);

   insertArrayf(target->currentVertices, vectorB.x, index + 12);
   insertArrayf(target->currentVertices, vectorB.y, index + 13);
   insertArrayf(target->currentVertices, vectorB.z, index + 14);

   insertArrayf(target->currentVertices, vectorA.x, index + 15);
   insertArrayf(target->currentVertices, vectorA.y, index + 16);
   insertArrayf(target->currentVertices, vectorA.z, index + 17);




   insertArrayf(target->currentVertices, input2.x, index + 18);
   insertArrayf(target->currentVertices, input2.y, index + 19);
   insertArrayf(target->currentVertices, input2.z, index + 20);

   insertArrayf(target->currentVertices, vectorC.x, index + 21);
   insertArrayf(target->currentVertices, vectorC.y, index + 22);
   insertArrayf(target->currentVertices, vectorC.z, index + 23);


   insertArrayf(target->currentVertices, vectorB.x, index + 24);
   insertArrayf(target->currentVertices, vectorB.y, index + 25);
   insertArrayf(target->currentVertices, vectorB.z, index + 26);




   insertArrayf(target->currentVertices, vectorA.x, index + 27);
   insertArrayf(target->currentVertices, vectorA.y, index + 28);
   insertArrayf(target->currentVertices, vectorA.z, index + 29);

   insertArrayf(target->currentVertices, vectorB.x, index + 30);
   insertArrayf(target->currentVertices, vectorB.y, index + 31);
   insertArrayf(target->currentVertices, vectorB.z, index + 32);


   insertArrayf(target->currentVertices, vectorC.x, index + 33);
   insertArrayf(target->currentVertices, vectorC.y, index + 34);
   insertArrayf(target->currentVertices, vectorC.z, index + 35);

}

sphere3D* initSphere3D(void)
{
	GLfloat octahedronArray [] =
	{
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,

		0.0, 0.0, 1.0,
		-1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,

		-1.0, 0.0, 0.0,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0,

		0.0, 0.0, -1.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,


		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, -1.0, 0.0,

		0.0, 0.0, 1.0,
		-1.0, 0.0, 0.0,
		0.0, -1.0, 0.0,

		-1.0, 0.0, 0.0,
		0.0, 0.0, -1.0,
		0.0, -1.0, 0.0,

		0.0, 0.0, -1.0,
		1.0, 0.0, 0.0,
		0.0, -1.0, 0.0,

	};
	
	/* Copy octahedron to the ball and old ball*/
	sphere3D* result = NULL;
	result = malloc(sizeof(sphere3D));

	result->oldVertices = NULL;
	result->oldVertices = initArrayf();
	copyToArrayf(result->oldVertices, &octahedronArray[0], 0, 72);

	result->currentVertices = NULL;
	result->currentVertices = initArrayf();
	copyToArrayf(result->currentVertices, &octahedronArray[0], 0, 72);

	return result;

}

void tessellateSphere3D(sphere3D* target)
{
	int i = 0;
	/*Use old balls vectors to contruct new balls vectors */

	/*copy old vectors to old ball */
	copyToArrayf(target->oldVertices, target->currentVertices->data, 0, sizeArrayf(target->currentVertices));

	for(i = 0; i < sizeArrayf(target->oldVertices) /9 ; i++)
	{
		v3f input0 = vInit(getArrayf(target->oldVertices, i*9), getArrayf(target->oldVertices, i*9+1), getArrayf(target->oldVertices, i*9+2));
		v3f input1 = vInit(getArrayf(target->oldVertices, i*9+3), getArrayf(target->oldVertices, i*9+4), getArrayf(target->oldVertices, i*9+5));
		v3f input2 = vInit(getArrayf(target->oldVertices, i*9+6), getArrayf(target->oldVertices, i*9+7), getArrayf(target->oldVertices, i*9+8));

		createNewTriangles(target, input0, input1, input2, i*36);
	}


}


void destroySphere3D(sphere3D* target)
{
	
	destroyArrayf(target->oldVertices);
	destroyArrayf(target->currentVertices);
	free(target);
}






/* End of include guard SPHERE_H*/
#endif