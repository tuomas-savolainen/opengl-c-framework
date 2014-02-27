

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>


// standard c-libraries
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

//own libraries
#include "vectormath2.h"
#include "objloader.h"
#include "textures.h"
#include "sphere.h"
#include "window.h"
#include "shaders.h"





void drawGrid(void)
{

	GLfloat geometricArray [] =
	{

		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f

	};

	GLfloat normalArray [] =
	{

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f

	};
	

	

	/* draw and duplicate grid inside push and pop here */
	glVertexPointer(3, GL_FLOAT, 0, geometricArray);

	glNormalPointer(GL_FLOAT, 0, normalArray);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);


	glColor3ub(128, 128, 128);




	int stepZ = 0;
	int stepX = 0;

	for(stepZ = 0; stepZ < 20; stepZ++)
	{
		for(stepX = 0; stepX < 20; stepX++)
		{
			glPushMatrix();

			glTranslatef(-10.0f+ stepX , -1.0f, -1.0f - stepZ);
			
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);	//rotation is in degrees

			glDrawArrays(GL_LINES, 0, 8);

			glPopMatrix();
		}

	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);



}

void drawCustom(Arrayf* vertices, Arrayf* normals)
{
	
	glVertexPointer(3, GL_FLOAT, 0, vertices->data);

	glNormalPointer(GL_FLOAT, 0, normals->data);


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glColor3ub(128, 128,128);


	glPushMatrix();
		
		glTranslatef(0.0 , 0.0f, -10.0f);
		//rotation is in degrees
		//glRotatef((float) glfwGetTime()*30.0f, 0.0f, 1.0f, 0.0f);	
		//printf("%d\n", sizeArrayf(vertices)/3);
		glScalef(10.0f, 10.0f, 10.0f);
		glDrawArrays(GL_TRIANGLES, 0, sizeArrayf(vertices)/3);

	glPopMatrix();

	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}

void drawSphere3D(sphere3D* target, float programTime, float rotationAngle)
{
	/*Set the vertex and normal data*/
	glVertexPointer(3, GL_FLOAT, 0, target->currentVertices->data);


	/*since this is unit ball the normal vectors are the same as normal vectors */
	glNormalPointer(GL_FLOAT, 0, target->currentVertices->data);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);



	/*Set the default drawing color to Cobalt blue */
	glColor3ub(0, 71, 171);

	/*Save the viewmatrix (must be defined before the drawSphere function call) */
	glPushMatrix();
 


	/* move the model farther and up (i.e. move it along negative z-axis and up in the direction of y-axis) */
		//glTranslatef(0.0, 2.0, -15.0*programTime);
		glTranslatef(15.0 * cos(programTime/2.0+rotationAngle), 15.0*sin(programTime/2.0+rotationAngle) , -10.0);

		/* rotate model according to time degrees around y-axis */
		//glRotatef((float) glfwGetTime() *10.0, 0.0, 1.0, 0.0);

		/* scale the model */
		glScalef(0.5f, 0.5f, 0.5f);

		/* Remember to use vertex count here! */
		glDrawArrays(GL_TRIANGLES, 0, sizeArrayf(target->currentVertices)/3);

	/*Restore original viewmatrix */
	glPopMatrix();


	/* Disable vertex and normal data */
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}





int main(int argc, char** argv)
{

	//try creating window
	GLFWwindow* window = NULL;
	window = initWindow();


   	//try creating shaders
   	int programObject = initShader();

   	// create simple camera animation to smooth the transition to new view


   	static sPosition currentTarget;
   	resetSpherePosition(&currentTarget);
   	setCurrentTarget(&currentTarget);


   	static sPosition smoothTarget;
   	resetSpherePosition(&smoothTarget);




   	printf("%s\n", "loading comanche.obj");

   	object3D* comanche = initObject3D();


	loadObject3D(comanche, "comanche.obj");


	// Create other objects
	sphere3D* blueLight = initSphere3D();
	tessellateSphere3D(blueLight);
	tessellateSphere3D(blueLight);
	tessellateSphere3D(blueLight);

	sphere3D* greenLight = initSphere3D();
	tessellateSphere3D(greenLight);
	tessellateSphere3D(greenLight);
	tessellateSphere3D(greenLight);

	// Load textures (load only file to memory, doesn't do any necessary opengl calls)
	/*
	ArrayB* chairImage = NULL;
   	chairImage = initArrayB();
   	loadTexture(chairImage, "chairTexture.ppm");
	*/


   	float programTime = 0.0;
   	GLint myUniformLocation = glGetUniformLocation(programObject, "programTime");


   	//Loop that should last for one frame
	while(!glfwWindowShouldClose(window))
   	{
   		glEnable(GL_DEPTH_TEST); 
   		clearAndResize(window);

   		programTime = (float) glfwGetTime();
		glUniform1f(myUniformLocation, programTime);
   		// Move camera in spherical coorninates
		
		smoothTransition(&smoothTarget, getCurrentTarget(), 20.0);
		polarView(smoothTarget.position.x, smoothTarget.position.y, smoothTarget.position.z, smoothTarget.radius, smoothTarget.theta, smoothTarget.fii);


		// Draw things
   		drawGrid();

   		drawCustom(comanche->triangleGeometry, comanche->triangleNormals);

   		drawSphere3D(blueLight, programTime, 0.0);
   		drawSphere3D(greenLight, programTime, PI);


   		//Show the drawings
      	glfwSwapBuffers(window);
      	//Check for user input
      	glfwPollEvents();

   	}

   	// Destroy the window and end the glfw
   	glfwDestroyWindow(window);
   	// Destroy the 3D objects
   	destroyObject3D(comanche);

   	destroySphere3D(blueLight);
   	destroySphere3D(greenLight);

   	// Destroy the textures

   	

      


	return 0; 
}