/* Include guards */
#ifndef TEXTURES_H
#define TEXTURES_H

#include <ctype.h>
#include "datastructures.h"

//TODO: ADD textures to opengl and glsl


/* Usage example:
	ArrayB* texture = NULL;
	texture = initArrayB();

	loadTexture(texture, "mountains.ppm");

	destroyArrayB(texture);

*/



/* reads the line and skips any whitespace */
/* returns -1 if no number was found */
int readNumber( FILE* bFile)
{
	char input;

	char buffer[10];
	int count = 0;

	int result = 0;

	input = fgetc(bFile);

	while(input != EOF)
	{
		if(input == '#')
		{
			while((input != EOF)&&(input != '\n'))
			{
				input = fgetc(bFile);
			}
			return -1;
		}
		else
		{
			if(!isspace(input))
			{
				buffer[count] = input;
				count++;
			}
			else
			{
				buffer[count] = '\0';
				result = atoi(buffer);
				return result;
			}
		}
		input = fgetc(bFile);
	}

	return -1;

}

/* loads texture from ppm file:
dest: pointer to (unsigned char) array were the data should be saved
filename: name of the file to load
*/
void loadTexture(ArrayB* dest, char *fileName)
{
	int width = 0;
	int height = 0;
	int max = 0;
	//int data = 0;
	char input;

	/*reading binary file*/

	FILE * bFile = NULL;
	bFile = fopen(fileName, "rb");

	if (bFile == NULL)
	{
		fputs("failed to open file for reading", stderr);
		exit(EXIT_FAILURE);
	}

	/* Check that file format is P6 */
	printf("reading file: %s\n", fileName);
	if((fgetc(bFile) == 'P')&&(fgetc(bFile) == '6'))
	{
		printf("correct file format (P6, PPM)\n");

	}
	else
	{
		printf("wrong file format, exit...\n" );
		fclose(bFile);	//close the file
		exit(EXIT_FAILURE);
	}
	/* skip the newline */
	fgetc(bFile);

	/* skip all the comments */

	while((width = readNumber(bFile)) == -1);
	printf("%d\n", width);

	while((height = readNumber(bFile)) == -1);
	printf("%d\n", height);

	while((max = readNumber(bFile)) == -1);
	printf("%d\n", max);
	printf("file info readed succesfully\n");





	int i = 0;
	int x = 0;
	for(i = 0; i < width*height*3; i++)
	{
		input = fgetc(bFile);
		pushArrayB(dest, (unsigned char) input);
		x++;
	}


	printf("Loaded %d bytes \n", x);
	printf("done reading\n");
	fclose(bFile);	//close the file
}




/* End of include guard TEXTURES_H*/
#endif