/* Include guards */
#ifndef SHADERS_H
#define SHADERS_H

// Returns program object so that you can pass uniform (read only) values to shaders
int initShader()
{
	//Read shaders from files basic.vert and basic.frag
	printf("%s\n", "reading vertex shader from basic.vert");
	byteArray* vertexText = NULL;
	vertexText = initByteArray(sizeof(char));
	FILE * vertFile = NULL;
	vertFile = fopen("basic.vert", "r");	//open file for reading
	if(vertFile != NULL)
	{
		char letter;
		while((letter = getc(vertFile)) != EOF)
		{
			pushByteArray(vertexText, &letter);
		}
		char ending = '\0';
		pushByteArray(vertexText,&ending);
	}
	else
	{
		printf("%s\n", "failed to open file for reading");
	}
	fclose(vertFile);


	printf("%s\n", "reading fragment shader from basic.frag");
	byteArray* fragmentText = NULL;
	fragmentText = initByteArray(sizeof(char));
	FILE * fragFile = NULL;
	fragFile = fopen("basic.frag", "r");	//open file for reading
	if(fragFile != NULL)
	{
		char letter;
		while((letter = getc(fragFile)) != EOF)
		{
			pushByteArray(fragmentText, &letter);
		}
		char ending = '\0';
		pushByteArray(fragmentText,&ending);
	}
	else
	{
		printf("%s\n", "failed to open file for reading");
	}
	fclose(fragFile);


	const GLchar* vertexShaderSource = (char*)vertexText->data;


	const GLchar* fragmentShaderSource = (char*)fragmentText->data;


	//Create program ,vertex and fragment shader objects
	GLuint programObject = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader sources
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile the shaders
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	// Print compile info
	GLchar vertexMessage[256];
	glGetShaderInfoLog(vertexShader, 256, NULL, vertexMessage);
	printf("%s\n", vertexMessage);

	GLchar fragmentMessage[256];
	glGetShaderInfoLog(fragmentShader, 256, NULL, fragmentMessage);
	printf("%s\n", fragmentMessage);

	


	//Attach the shaders to the GPU program object
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	//Create an executable to run on the GPU
	glLinkProgram(programObject);
	//install vertex shader as part of the pipeline (Note for 3.1 you must specify also fragment shader)
	
	GLchar linkMessage[256];
	glGetProgramInfoLog(programObject, 256, NULL, linkMessage);
	printf("%s\n", linkMessage);

	glUseProgram(programObject);

	destroyByteArray(vertexText);


	return programObject;

}


/* End of include guard SHADERS_H*/
#endif