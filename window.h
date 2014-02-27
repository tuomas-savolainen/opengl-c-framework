/* Include guards */
#ifndef WINDOW_H
#define WINDOW_H


#define PI 3.141592654



struct spherePosition
{
	/* distance from the origin */
	v3f position;
	/* direction vector*/
	v3f direction;
	/* helper variables */
	GLfloat radius;
	GLfloat theta;
	GLfloat fii;

};
typedef struct spherePosition sPosition;



static sPosition* globalCurrentTarget = NULL;	

static void setCurrentTarget(sPosition* target)
{
	globalCurrentTarget = target;
}

static sPosition* getCurrentTarget(void)
{
	return globalCurrentTarget;
}


static void resetSpherePosition(sPosition* target)
{
	target->position = vInit(0.0, 0.0, 1.0);
	target->direction = vInit(1.0, 1.0, 1.0);
	target->radius = 1.0;
	target->theta = 0.0;
	target->fii = 0.0;
}



//Converts screencoordinates to cartesian coordinates (origo at the center of the screen)
//Change this function to adjust according to window resolution
static void toCartesianCoordinates(GLFWwindow* window,  double* x, double* y)
{
	int width;
	int height;
	glfwGetFramebufferSize(window, &width, &height);
	double halfWidth = width/2.0;
	double halfHeight = height/2.0;

	*x = (*x - halfWidth)/halfWidth;
	*y = -(*y - halfHeight)/halfHeight;
}


static void error_callback(int error, const char* description)
{
   fputs(description, stderr);
}


static void moveTargetForward(sPosition* target)
{
	target->position.z += cos(target->theta);
	target->position.x -= sin(target->theta);

}

static void moveTargetBacward(sPosition* target)
{
	target->position.z -= cos(target->theta);
	target->position.x += sin(target->theta);

}


static void moveTargetLeft(sPosition* target)
{

	target->position.z -= cos(target->theta+PI/2);
	target->position.x += sin(target->theta+PI/2);

}


static void moveTargetRight(sPosition* target)
{
	target->position.z += cos(target->theta+PI/2);
	target->position.x -= sin(target->theta+PI/2);


}

static void zoomTargetIn(sPosition* target)
{
	if(target->radius > 1.0)
	{
		target->radius = target->radius - 0.8;
	}

}

static void zoomTargetOut(sPosition* target)
{
	if(target->radius < 1000.0)
	{
		target->radius = target->radius + 1.0;
	}
		
}


static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

   if (action == GLFW_PRESS)
   {
   	//change this later on so that you can select targets for example with ctrl+mouse
	sPosition* target =  getCurrentTarget();

      	switch(key)
      	{
         	case GLFW_KEY_ESCAPE:
            	glfwSetWindowShouldClose(window, GL_TRUE);
            	break;
            case GLFW_KEY_W:
            	moveTargetForward(target);
            	break;
            case GLFW_KEY_A:
				moveTargetLeft(target);
				break;

            case GLFW_KEY_D:
				moveTargetRight(target);
				break;
			
			case GLFW_KEY_S:
				moveTargetBacward(target);
				break;
			case GLFW_KEY_R:
				resetSpherePosition(target);
				break;
         	default:
            	break;
      }
   }

}

static void wheelCallback(GLFWwindow* window, double deltaX, double deltaY)
{
	sPosition* target =  getCurrentTarget();
	if (deltaY > 0)
	{
		zoomTargetIn(target);
	}
	else if(deltaY < 0)
	{
		zoomTargetOut(target);
	}
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	static double lastX = 0.0;
	static double lastY = 0.0; 
	if((button == GLFW_MOUSE_BUTTON_MIDDLE)&&(action == GLFW_PRESS))
	{
		//record starting position
		glfwGetCursorPos(window, &lastX, &lastY);
		toCartesianCoordinates(window, &lastX, &lastY);
	}
	else if((button == GLFW_MOUSE_BUTTON_MIDDLE)&&(action == GLFW_RELEASE))
	{
		sPosition* target =  getCurrentTarget();
		double updatedX =0.0;
		double updatedY =0.0;
		glfwGetCursorPos(window, &updatedX, &updatedY);
		toCartesianCoordinates(window, &updatedX, &updatedY);


		target->theta += (updatedX-lastX);
		target->fii += (updatedY-lastY);
		//keep values of theta and fii between [-PI, PI]
		if(target->theta > 2*PI)
		{
			target->theta -= 2*PI;
		}
		else if(target->theta < -2*PI)
		{
			target->theta += 2*PI;
		}

		if(target->fii > 2*PI)
		{
			target->fii -= 2*PI;
		}
		else if(target->fii < -2*PI)
		{
			target->fii += 2*PI;
		}



	}
}


static void clearAndResize(GLFWwindow* window)
{
   float ratio;
   int width, height;

   glfwGetFramebufferSize(window, &width, &height);
   ratio = width / (float) height;
   //resize
   glViewport(0, 0, width, height);
   //Clear
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* PROJECTION MATRIX == CAMERA */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ratio, ratio, -1.0f, 1.0f, 1.0f, 400.0f);


}

/* initial = what to transform, target = final value, speed  */
void smoothTransition(sPosition* initial, sPosition* target, float speed)
{
	if(speed > 0)
	{
		if(initial->position.x != target->position.x)
		{
			initial->position.x += (target->position.x - initial->position.x)/speed;
		}
		if(initial->position.y != target->position.y)
		{
			initial->position.y += (target->position.y - initial->position.y)/speed;
		}
		if(initial->position.z != target->position.z)
		{
			initial->position.z += (target->position.z - initial->position.z)/speed;
		}
		if(initial->radius != target->radius)
		{
			initial->radius += (target->radius - initial->radius)/speed;
		}
		if(initial->theta != target->theta)
		{
			initial->theta += (target->theta - initial->theta)/speed;
		}
		if(initial->fii != target->fii)
		{
			initial->fii += (target->fii - initial->fii)/speed;
		}

	}

}

GLFWwindow* initWindow(void)
{
	GLFWwindow* window = NULL;

	glfwSetErrorCallback(error_callback);


	if(!glfwInit())
	{
		fputs("failed to create context, (glfwInit failed)", stderr);
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1024, 768, "OPENGL 3.0 and glsl 1.3 template", NULL, NULL);

   	if(!window)
   	{
   		fputs("failed to create window, (glfwCreateWindow failed)", stderr);
    	glfwTerminate();
    	exit(EXIT_FAILURE);
   	}

   	//function makes the context of the specified window current on the calling thread
   	glfwMakeContextCurrent(window);

   	//function sets the key callback of the specific window, which is called when a key is pressed, repeated or released
   	glfwSetKeyCallback(window, keyCallback);

   	//function sets the mouseclick callback
   	glfwSetMouseButtonCallback(window, mouseButtonCallback);

   	//function sets the mousewheel callback
   	glfwSetScrollCallback(window, wheelCallback);


   	//print the opengl version
   	printf("%s\n", glGetString(GL_VERSION));

   	return window;

}



void polarView(float x, float y, float z, float radius, float theta, float fii)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -radius);	//fourth
	glRotatef(-fii*180/3.141592, 1.0, 0.0, 0.0);	//third
	glRotatef(theta*180/3.141592, 0.0, 1.0, 0.0); //second
	glTranslatef(x, y, z);	//first

}



/* End of include guard WINDOW_H*/
#endif
