// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
//#include <glm/glm.hpp>
//using namespace glm;

#include <common/shader.hpp>

#include "games.h"

Game game;

float select_x = -0.3;
float select_y = -0.6;
int curr_x = 0;
int curr_y = 0;

float default_color[] = {0.0, 0.0, 0.0};

float color_array[8][3] = {
	{0.0, 0.4, 0.4},
	{0.0, 0.8, 0.0},
	{0.8, 0.0, 0.0},
	{0.0, 0.0, 0.8},
	{0.8, 0.8, 0.0},
	{0.0, 0.8, 0.8},
	{0.8, 0.0, 0.8},
	{0.2, 0.2, 0.2},
};

int rund = 1;
int press_space = 0;
int match = 0;
int first_match[2] = {0,0};
int second_match[2] = {0,0};
bool hide = false;
int time_show_card = 0;
bool trans = false;
float time_trans;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		system("cls");
		printf("Rund: %d\n", rund);
		printf("Match: %d\n", match);
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (match < 8){
			if (hide){
				if (time_show_card > 0)
					time_show_card--;
				else {
					trans = true;
					time_trans = 0.25;
					hide = false;
				}
			}
			if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !hide && !trans)
			{
				if(game.getCard(curr_x, curr_y)->get_visibility()){
					press_space++;
					if (press_space == 1) {
						first_match[0] = curr_x;
						first_match[1] = curr_y;
						game.getCard(curr_x, curr_y)->set_page(2);
					}
					else if (press_space == 2) {
						if (first_match[0] != curr_x || first_match[1] != curr_y){
							rund++;
							press_space = 0;
							game.getCard(curr_x, curr_y)->set_page(2);
							if (game.getCard(first_match[0], first_match[1])->get_id() == game.getCard(curr_x, curr_y)->get_id()) {
								game.getCard(first_match[0], first_match[1])->set_visibility(false);
								game.getCard(curr_x, curr_y)->set_visibility(false);
								match++;
							}
							else {
								second_match[0] = curr_x;
								second_match[1] = curr_y;
								hide = true;
								time_show_card = 2;
							}
						} else
							press_space--;
					}
				}
			}
				if (key == GLFW_KEY_UP && action == GLFW_PRESS)
			{
				if (select_y < 0.1){
					select_y += 0.3;
					curr_y++;
				}
			}
			if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
			{
				if (select_y > -0.6){
					select_y -= 0.3;
					curr_y--;
				}
			}
			if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			{
				if (select_x > -0.21){
					select_x -= 0.3;
					curr_x--;
				}
			}
			if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			{
				if (select_x < 0.5){
					select_x += 0.3;
					curr_x++;
				}
			}
		} else
			printf("GAME OVER\n\n Esc to quit.");
	}


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	game.init_game();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 640, 480, "Memory games", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.6f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

	GLuint tempId = glGetUniformLocation(programID, "temp");
	GLuint posXId = glGetUniformLocation(programID, "posX");
	GLuint posYId = glGetUniformLocation(programID, "posY");
	GLuint rotationId = glGetUniformLocation(programID, "rotationX");
	GLuint redId = glGetUniformLocation(programID, "red");
	GLuint greenId = glGetUniformLocation(programID, "green");
	GLuint blueId = glGetUniformLocation(programID, "blue");

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 0.0f,  0.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		-1.0f,  0.0f, 0.0f,
		-1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		-0.49f, 0.49f, 0.0f,
		 0.0f,  0.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		 -0.5f, 0.0f, 0.0f,
		 -1.0f, 0.5f, 0.0f,
		 -0.5f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,
		 -0.75f, 1.0f, 0.0f,
		 -0.25f, 1.0f, 0.0f,
		 0.0f,  0.75f, 0.0f,
		 0.0f,  0.25f, 0.0f,
		 -0.25f, 0.0f, 0.0f,
		 -0.75f, 0.0f, 0.0f,
		 -1.0f, 0.25f, 0.0f,
		 -1.0f, 0.75f, 0.0f,
		 -1.0f, 0.5f, 0.0f,
		 -0.5f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,
		 -0.25f, 0.0f, 0.0f,
		 -0.75f, 0.0f, 0.0f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do{

		//Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glUniform1f(rotationId, 1.0);
		glUniform1f(redId, 1.0);
		glUniform1f(greenId, 0.0);
		glUniform1f(blueId, 0.0);
		
		glUniform1f(tempId, 1.4);
		glUniform1f(posXId, 0.72);
		glUniform1f(posYId, -0.72);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		
		glUniform1f(redId, 0.0);
		glUniform1f(greenId, 0.6);
		glUniform1f(blueId, 0.4);
		
		glUniform1f(tempId, 1.25);
		glUniform1f(posXId, 0.65);
		glUniform1f(posYId, -0.65);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);;




		glUniform1f(redId, 1.0);
		glUniform1f(greenId, 0.0);
		glUniform1f(blueId, 0.0);
		
		glUniform1f(tempId, 0.28);
		glUniform1f(posXId, select_x + 0.015);
		glUniform1f(posYId, select_y - 0.015);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glUniform1f(redId, 0.0);
		glUniform1f(greenId, 0.6);
		glUniform1f(blueId, 0.4);
		
		glUniform1f(tempId, 0.25);
		glUniform1f(posXId, select_x);
		glUniform1f(posYId, select_y);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


		int tmp_id = 0;
		float tmp_i = -0.3;
		float tmp_j = -0.9;
		bool revers;

		for (int i = 0; i < 4; i++ ) {
			for (int j = 0; j < 4; j++ )
			{
				tmp_j += 0.3;
				//if (!game.getCard(i, j)->get_visibility())
					//continue;
				tmp_id = game.getCard(i, j)->get_id();
				glUniform1f(posXId, tmp_i);
				glUniform1f(posYId, tmp_j);
				revers = game.getCard(i, j)->get_page() == 1;
				if (revers) {
					glUniform1f(redId, default_color[0]);
					glUniform1f(greenId, default_color[1]);
					glUniform1f(blueId, default_color[2]);
				}
				else
				{
					glUniform1f(redId, color_array[tmp_id][0]);
					glUniform1f(greenId, color_array[tmp_id][1]);
					glUniform1f(blueId, color_array[tmp_id][2]);
				}

				if (trans && ( (first_match[0] == i && first_match[1] == j) || (second_match[0] == i && second_match[1] == j) )) {
					time_trans -= 0.005;
					if (time_trans < 0){
						game.getCard(first_match[0], first_match[1])->set_page(1);
						game.getCard(second_match[0], second_match[1])->set_page(1);
						glUniform1f(rotationId, (-1) * time_trans);
					}
					else glUniform1f(rotationId, time_trans);
					if (time_trans < -0.25)
						trans = false;
				}
				else {
					glUniform1f(rotationId, 1.0);
				}
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				if (!revers) {
					glUniform1f(redId, 0.9);
					glUniform1f(greenId, 0.9);
					glUniform1f(blueId, 0.9);
					if (tmp_id == 0)
						glDrawArrays(GL_TRIANGLES, 0, 3);
					else if (tmp_id == 1)
						glDrawArrays(GL_TRIANGLES, 1, 3);
					else if (tmp_id == 2)
						glDrawArrays(GL_LINES, 3, 2);
					else if (tmp_id == 3){
						glDrawArrays(GL_LINES, 1, 2);
						glDrawArrays(GL_LINES, 3, 2);
					}
					else if (tmp_id == 4){
						glDrawArrays(GL_TRIANGLES, 5, 7);
					}
					else if (tmp_id == 5){
						glDrawArrays(GL_TRIANGLE_FAN, 11, 4);
					}
					else if (tmp_id == 6){
						glDrawArrays(GL_TRIANGLE_FAN, 15, 8);	
					}
					else if (tmp_id == 7){
						glDrawArrays(GL_TRIANGLE_FAN, 23, 5);
					}
				}
				glUniform1f(tempId, 0.25);
				if (!game.getCard(i, j)->get_visibility()){
					glUniform1f(redId, 0.0);
					glUniform1f(greenId, 0.55);
					glUniform1f(blueId, 0.35);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);}
			}
			tmp_i += 0.3;
			tmp_j = -0.9;
		}
		// Draw the triangle !
		// glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 3 indices starting at 0 -> 1 triangle



		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwWindowShouldClose(window) == 0);
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

