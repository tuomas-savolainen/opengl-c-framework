/* Include guards */
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "bytearray.h"
#include "datastructures.h"


//TODO: Add support for other forms than triangles (i.e. quads) and for 1d and 3d textures
struct object3D
{
	/* data */
	/* v3f array for vertices */
	Arrayf* triangleGeometry;

	Arrayf* triangleNormals;

	Arrayf* triangleTextures;

	Arrayf* geometricVertices;

	Arrayf* normalVertices;

	Arrayf* textureVertices;

	byteArray* faces;

};

typedef struct object3D object3D;


// TODO: Handle negative indexes
void constructTriangles(object3D* target)
{
	/* One face consists of three face elements and generates one triangle */

	/* Check that there is enough face elements */
	if(sizeByteArray(target->faces) % 3 == 0)
	{
		int i = 0;



		for(i = 0; i  < sizeByteArray(target->faces); i = i + 3)
		{
			/* Push x, y and z */
			/*Load vertex corresponding to index written in member i in faces (*(int*)getByteArray(target->faces, i),
			Negate one since arrays begin from zero in c
			multiply the index by three since the arrays has values like = [x0, y0, z0, x1, y1, z1, ...]
			get x, y and z values, yeah I know it looks horrible
			 */

			pushArrayf(target->triangleGeometry,getArrayf(target->geometricVertices, (*(int*)getByteArray(target->faces, i) -1) *3));
			pushArrayf(target->triangleGeometry,getArrayf(target->geometricVertices, (*(int*)getByteArray(target->faces, i) -1) *3+1));
			pushArrayf(target->triangleGeometry,getArrayf(target->geometricVertices, (*(int*)getByteArray(target->faces, i) -1) *3+2));

			/* Push u, v, w */
			pushArrayf(target->triangleTextures,getArrayf(target->textureVertices, (*(int*)getByteArray(target->faces, i+1) -1) *3));
			pushArrayf(target->triangleTextures,getArrayf(target->textureVertices, (*(int*)getByteArray(target->faces, i+1) -1) *3+1));
			pushArrayf(target->triangleTextures,getArrayf(target->textureVertices, (*(int*)getByteArray(target->faces, i+1) -1) *3+2));

			/* Push i, j, k */
			pushArrayf(target->triangleNormals,getArrayf(target->normalVertices, (*(int*)getByteArray(target->faces, i+2) -1) *3));
			pushArrayf(target->triangleNormals,getArrayf(target->normalVertices, (*(int*)getByteArray(target->faces, i+2) -1) *3+1));
			pushArrayf(target->triangleNormals,getArrayf(target->normalVertices, (*(int*)getByteArray(target->faces, i+2) -1) *3+2));




		}
		/* Scale the model to unit length */
		
		float x = 1.0;
		float y = 1.0;
		float z = 1.0;
		float max = 1.0;
		for(i = 0; i < sizeArrayf(target->triangleGeometry); i = i + 3)
		{
			x = getArrayf(target->triangleGeometry, i);
			y = getArrayf(target->triangleGeometry, i+1);
			z = getArrayf(target->triangleGeometry, i+2);
			if(sqrt(x * x + y * y + z * z) > max)
			{
				max = sqrt(x * x + y * y + z * z);
			}
		}
		printf("max = %f\n", max);
		for(i = 0; i < sizeArrayf(target->triangleGeometry); i = i + 3)
		{

			x = getArrayf(target->triangleGeometry, i);
			y = getArrayf(target->triangleGeometry, i+1);
			z = getArrayf(target->triangleGeometry, i+2);
			if((x/max > 1) || (y/max > 1) || (z/max > 1))
				printf("%s\n", "over");

			insertArrayf(target->triangleGeometry, (x/max), i);
			insertArrayf(target->triangleGeometry, (y/max), i+1);
			insertArrayf(target->triangleGeometry, (z/max), i+2);
		}
		



	}
	else
	{
		printf("not divisible by 3\n");
	}
}

//TODO: Test this function
/*TextureString Format
vt u v w
*/
void parseTextureString(object3D* dest, const char* line)
{
	int limit = strlen(line);
	int i = 0;

	char buffer[32];
	int count = 0;

	float result[3] = {0.0};

	size_t valueIndex = 0;

	if(isspace(line[2]))
	{
		for(i = 3; (i < limit)&&(valueIndex < 4); i++)
		{
			if((isdigit(line[i]))||(line[i] == '-')||(line[i] == '.'))
			{
				buffer[count] = line[i];
				count++;
			}
			else
			{
				buffer[count] = '\0';
				result[valueIndex] = atof(buffer);
				count = 0;
				valueIndex++;
			}
		}
	}
	else
	{
		for(i = 2; (i < limit)&&(valueIndex < 4); i++)
		{
			if((isdigit(line[i]))||(line[i] == '-')||(line[i] == '.'))
			{
				buffer[count] = line[i];
				count++;
			}
			else
			{
				buffer[count] = '\0';
				result[valueIndex] = atof(buffer);
				count = 0;
				valueIndex++;
			}
		}
	}
	pushArrayf(dest->textureVertices, result[0]);
	pushArrayf(dest->textureVertices, result[1]);
	pushArrayf(dest->textureVertices, result[2]);
	//printf(" Parsing Texture: %s\n", line);
}

/*NormalString Format
vn i j k
*/
void parseNormalString(object3D* dest, const char* line)
{
	
	int limit = strlen(line);
	int i = 0;

	char buffer[32];
	int count = 0;

	float result[3] = {0.0};

	size_t valueIndex = 0;

	if(isspace(line[2]))
	{
		for(i = 3; (i < limit)&&(valueIndex < 4); i++)
		{
			if((isdigit(line[i]))||(line[i] == '-')||(line[i] == '.'))
			{
				buffer[count] = line[i];
				count++;
			}
			else
			{
				buffer[count] = '\0';
				result[valueIndex] = atof(buffer);
				count = 0;
				valueIndex++;
			}
		}
	}
	else
	{
		for(i = 2; (i < limit)&&(valueIndex < 4); i++)
		{
			if((isdigit(line[i]))||(line[i] == '-')||(line[i] == '.'))
			{
				buffer[count] = line[i];
				count++;
			}
			else
			{
				buffer[count] = '\0';
				result[valueIndex] = atof(buffer);
				count = 0;
				valueIndex++;
			}
		}
	}
	pushArrayf(dest->normalVertices, result[0]);
	pushArrayf(dest->normalVertices, result[1]);
	pushArrayf(dest->normalVertices, result[2]);

}

/*VertexString Format:
v x y z w
*/
void parseVertexString(object3D* dest, const char* line)
{
	//printf(" Parsing Vertex: %s\n", line);
	int limit = strlen(line);
	int i = 0;

	char buffer[32];
	int count = 0;

	float result[4] = {0.0};
	result[3] = 1.0;

	size_t valueIndex = 0;


	if(isspace(line[2]))
	{
		for(i = 3; (i < limit)&&(valueIndex < 4); i++)
		{
			if((isdigit(line[i]))||(line[i] == '-')||(line[i] == '.'))
			{
				buffer[count] = line[i];
				count++;
			}
			else
			{
				buffer[count] = '\0';
				result[valueIndex] = atof(buffer);
				count = 0;
				valueIndex++;
			}
		}
	}
	else
	{
		for(i = 2; (i < limit)&&(valueIndex < 4); i++)
		{
			if((isdigit(line[i]))||(line[i] == '-')||(line[i] == '.'))
			{
				buffer[count] = line[i];
				count++;
			}
			else
			{
				buffer[count] = '\0';
				result[valueIndex] = atof(buffer);
				count = 0;
				valueIndex++;
			}
		}
	}


	pushArrayf(dest->geometricVertices, result[0]);
	pushArrayf(dest->geometricVertices, result[1]);
	pushArrayf(dest->geometricVertices, result[2]);
	/* Add result[3] if you want to use quads */





}

/* FaceString Format:
geometric vertex/texture vertex/vertex normal
texture vertex and vertex normal are optional, so legit formats are:
Only geometric:
f 1
f 1/
f 1//
geometric + texture:
f 1/2
f 1/2/
geometric + texture + normal:
f 1/2/3
geometrix + normal:
f 1//3
(maybe I forgot some case?)
So it is only guaranteed that the geometric vertex exists

*/

/* Lets start with reading only geometric vertices */
void parseFaceString(object3D* dest, const char* line)
{
	//printf(" Parsing Face: %s\n", line);
	int limit = strlen(line);
	int i = 0;

	char buffer[32];
	int count = 0;

	int result[12] = {0};
	size_t faceIndex = 0;
	size_t valueIndex = 0;


	for(i = 2; (i < limit)&&(faceIndex < 4); i++)
	{
		/*accept only "valid" numbers */
		if((isdigit(line[i]))||(line[i] == '-'))
		{
			buffer[count] = line[i];
			count++;
		}
		else if(isspace(line[i]))
		{
			//next face
			buffer[count] = '\0';
			result[faceIndex*3+valueIndex] = atoi(buffer);
			count = 0;
			faceIndex++;
			valueIndex = 0;
		}
		else if(line[i] == '/')
		{
			//next value
			buffer[count] = '\0';
			result[faceIndex*3+valueIndex] = atoi(buffer);
			count = 0;
			valueIndex++;

		}
	}


	//geometric vertice + texture vertice + normal vertice
	pushByteArray(dest->faces, &(result[0]));
	pushByteArray(dest->faces, &(result[1]));
	pushByteArray(dest->faces, &(result[2]));
	

	pushByteArray(dest->faces, &(result[3]));
	pushByteArray(dest->faces, &(result[4]));
	pushByteArray(dest->faces, &(result[5]));
	

	pushByteArray(dest->faces, &(result[6]));
	pushByteArray(dest->faces, &(result[7]));
	pushByteArray(dest->faces, &(result[8]));





	/* Add pusharray calls to use more data */

}

/* The EOF has been handled before so no need to check it here */
void handleString(object3D* dest, const char* line)
{
	static int count = 0;

	if(strlen(line) > 2)
	{
		if(line[0] == '#')
		{
			//printf("%d %s\n", count, "^_^");
		}
		else if((line[0] == 'v')&&(line[1] == 't')&&(isspace(line[2])))
		{
			parseTextureString(dest, line);
			
		}
		else if((line[0] == 'v')&&(line[1] == 'n')&&(isspace(line[2])))
		{
			parseNormalString(dest, line);
		}
		else if((line[0] == 'v')&&(isspace(line[1])))
		{
			parseVertexString(dest, line);
		}
		else if((line[0] == 'f')&&(isspace(line[1])))
		{
			parseFaceString(dest, line);
		}
		else
		{
			//printf("%d %s\n", count, line);
		}
	}
	count++;
}


/* reads the line and skips any whitespace */
/* returns 0 when the EOF is found*/
void readLines(object3D* dest, FILE* textFile)
{

	int index = 0;

	char buffer[256];

	char letter;

	while((letter = getc(textFile)) != EOF)
	{
		if(letter == '\n')
		{
			buffer[index] = ' ';
			index++;
			buffer[index] = '\0';
			handleString(dest, buffer);
			index = 0;
		}
		else
		{
			buffer[index] = letter;
			index++;
		}
		
	}

}

object3D* initObject3D(void)
{
	object3D* result = NULL;
	result = malloc(sizeof(object3D));
	result->triangleGeometry = initArrayf();
	result->triangleNormals = initArrayf();
	result->triangleTextures = initArrayf();
	result->geometricVertices = initArrayf();
	result->normalVertices = initArrayf();
	result->textureVertices = initArrayf();
	result->faces = initByteArray(sizeof(int));

	return result;
}

void destroyObject3D(object3D* target)
{
	destroyArrayf(target->triangleGeometry);
	destroyArrayf(target->triangleNormals);
	destroyArrayf(target->geometricVertices);
	destroyArrayf(target->triangleTextures);
	destroyArrayf(target->normalVertices);
	destroyArrayf(target->textureVertices);
	destroyByteArray(target->faces);

	free(target);
}


void loadObject3D(object3D* dest, const char *fileName)
{
	/* load the data to dest */


	/*reading text file*/
	printf("%s %s\n", "reading text file: ", fileName);

	FILE * textFile = NULL;
	textFile = fopen(fileName, "r");

	if(textFile != NULL)
	{
		readLines(dest, textFile);
		constructTriangles(dest);

		printf("%s info:\n", fileName);
		printf("geometric vertices = %d\n", sizeArrayf(dest->triangleGeometry)/3);
		printf("normal vertices = %d\n", sizeArrayf(dest->triangleNormals)/3);
		printf("done reading\n");


		
	}
	else
	{
		printf("%s\n", "failed to open file for reading");
	}
	fclose(textFile);


}

/* End of include guard OBJLOADER_H*/
#endif