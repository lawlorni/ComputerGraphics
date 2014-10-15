
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
//#include "maths_funcs.h"

int g_gl_width = 640;
int g_gl_height = 480;
float offx;

int main () {
	GLFWwindow* window = NULL;
	const GLubyte* renderer;
	const GLubyte* version;
	GLuint vao;
	GLuint vbo;
	GLuint colours_vbo;

	/* geometry to use. these are 3 xyz points (9 floats total) to make a triangle
	*/
	GLfloat points[] = {
		 0.0f,	0.5f,	0.0f,
		 0.5f, -0.5f,	0.0f,
		-0.5f, -0.5f,	0.0f,
		0.00f,	0.8f,	0.0f,
		 -0.5f, -0.2f,	0.0f,
		0.5f, -0.2f,	0.0f

	};

		GLfloat colours[] = {
		1.0f, 1.0f,  0.0f,  //yellow rgb(255,255,0)
		1.0f, 1.0f,  0.0f,
		1.0f, 1.0f,  0.0f,
		1.0f, 0.0f,  0.0f, //red rgb(255,0,0)
		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,
	};
	/* these are the strings of code for the shaders
	the vertex shader positions each vertex point */
	const char* vertex_shader =
	"#version 400\n"
	"layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_colour;"
    "out vec3 color;"
	"uniform mat4 matrix;"
	"void main () {"
	"  color = vertex_colour;"
	"	gl_Position=matrix * vec4(vertex_position, 1.0);"
	"}";
	/* the fragment shader colours each fragment (pixel-sized area of the
	triangle) */
	const char* fragment_shader =
	"#version 400\n"
	"in vec3 color;"
	"out vec4 frag_colour;"
	"void main () {"
	"	frag_colour = vec4 (color, 1.0);"
	"}";
	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vs, fs;
	/* GL shader programme object [combined, to link] */
	GLuint shader_programme;

	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return 1;
	} 

	/* change to 3.2 if on Apple OS X */
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow (
		640, 480, "Hello Triangle", NULL, NULL
	);
	if (!window) {
		fprintf (stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent (window);
	/* start GLEW extension handler */
	glewExperimental = GL_TRUE;
	glewInit ();

	/* get version info */
	renderer = glGetString (GL_RENDERER); /* get renderer string */
	version = glGetString (GL_VERSION); /* version as a string */
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

	/* tell GL to only draw onto a pixel if the shape is closer to the viewer */
	glEnable (GL_DEPTH_TEST); /* enable depth-testing */
	glDepthFunc (GL_LESS);/*depth-testing interprets a smaller value as "closer"*/

	/* a vertex buffer object (VBO) is created here. this stores an array of data
	on the graphics adapter's memory. in our case - the vertex points */
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (GLfloat), points, GL_STATIC_DRAW);
	

	glGenBuffers (1, &colours_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (colours), colours, GL_STATIC_DRAW);
	/* the vertex array object (VAO) is a little descriptor that defines which
	data from vertex buffer objects should be used as input variables to vertex
	shaders. in our case - use our only VBO, and say 'every three floats is a 
	variable' */
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray (1);  //enable 
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);



	
	/* here we copy the shader strings into GL shaders, and compile them. we then
	create an executable shader 'program' and attach both of the compiled shaders.
	we link this, which matches the outputs of the vertex shader to the inputs of
	the fragment shader, etc. and it is then ready to use */
	vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader, NULL);
	glCompileShader (vs);

	fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader, NULL);
	glCompileShader (fs);
	shader_programme = glCreateProgram ();
	
	glAttachShader (shader_programme, fs);
	glAttachShader (shader_programme, vs);
	glLinkProgram (shader_programme);



double theta=180;

GLfloat matrix[] = {
0.5f,0.0f,0.0f,0.00f,
0.0f,0.5f,0.0f,0.00f,
0.0f,0.0f,0.5f,0.00f,
0.25f,0.5f,0.75f,1.0f,
}; 

int matrix_location = glGetUniformLocation (shader_programme, "matrix");
	

	
	while (!glfwWindowShouldClose (window)) {
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		glUseProgram (shader_programme);
		glBindVertexArray (vao);

		glDrawArrays (GL_TRIANGLES, 0, 3);
		glDrawArrays (GL_TRIANGLES, 3, 6);
		
		// update other events like input handling 
		glfwPollEvents ();
		if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (window, 1);
		}

		
		 if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_U)) { //return to normal
		 matrix[0] += 0.015f;
		matrix[5] += 0.005f;
		 }

		 if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_J)) { //return to normal
		 matrix[0] -= 0.015f;
		matrix[5] -= 0.005f;
		 }

		 //uniformed scaling make smaller
		 if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_D)) { //return to normal
		 matrix[0] -= 0.005f;
		 matrix[5] -= 0.005f;
		  }
		
		 //uniformed scaling bigger
		 if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_E)) { //return to normal
		 matrix[0] += 0.005f;
		 matrix[5] += 0.005f;
		 matrix[10] += 0.005f;
		 }

		 //non-uniform scaling
		 if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_X)) { //return to normal
		 
		 matrix[5] = cos(theta);
		 matrix[6] = sin(theta);
		 matrix[9] = -sin(theta);
		 matrix[10] =cos(theta);
		 theta+=0.025;
		}

		//rotation around Y axis
		if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_Y)) { //return to normal
		 matrix[0] = cos(theta);
		 matrix[2] = sin(theta);
		 matrix[8] = -sin(theta);
		 matrix[10] =cos(theta);
		 theta+=0.025;
		}

		//rotation around Z axis
		if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_Z)) { //return to normal
		 matrix[0] = cos(theta);
		 matrix[1] = -sin(theta);
		 matrix[4] = sin(theta);
		 matrix[5] = cos(theta);
		 theta+=0.025;
		}

		//combined transformations
		 if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_C)) {
		 matrix[0] += 0.005f;
		 matrix[5] += 0.005f;
		 matrix[10] +=0.005f;
		 matrix[12] += 0.025f;
		}

		if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_V)) {
		 matrix[0] -= 0.005f;
		 matrix[5] -= 0.005f;
		 matrix[10] -=0.005f;
		 matrix[12] -= 0.025f;
		}

		// X left
		if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_LEFT)) {
				matrix[12] -= 0.025f;
			}

			//X Right
		if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_RIGHT)) {
				matrix[12] += 0.025f;
			}

			//Y Down
		if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_DOWN)) {
				matrix[13] -= 0.025f;
			}


		//Y UP
		if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_UP)) {
				matrix[13] += 0.025f;
			}


		glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix); //control triangle seperately
        
		// draw points 0-3 from the currently bound VAO with current in-use shader
		

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (window);
	}
	
	/* close GL context and any other GLFW resources */
	glfwTerminate();
	return 0;
}

