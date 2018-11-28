#include <cstdio>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#include "Buffer.h"
#include "Sprite.h"
#include "PlayerSprite.h"
#include "GameConfig.h"
#include "Game.h"
#include "Alien.h"
#include "Formatter.h"
#include "Player.h"

bool gameIsRunning = false;
int movementDirection = 0;
bool firePressed = false;

// Shader Functions
GLuint generateShaders();
void attachShader(GLuint shaderProgram, const GLchar* shader, int type);
void validateShader(GLuint shader, const char *file);
bool validateProgram(GLuint program);

// Callbacks
void errorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char* argv[])
{
	const int bufferWidth = 224;
	const int bufferHeight = 256;

	#pragma region WindowSetup

	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(2 * bufferWidth, 2 * bufferHeight, APPLICATION_NAME, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW.\n");
		glfwTerminate();
		return -1;
	}

	// Create buffer and fill it with color
	Buffer* buffer = new Buffer(bufferWidth, bufferHeight);

	// Create texture
	GLuint bufferTexture;
	glGenTextures(1, &bufferTexture);
	glBindTexture(GL_TEXTURE_2D, bufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer->width, buffer->height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// Create vao for generating fullscreen triangle
	GLuint fullscreenTriangleVAO;
	glGenVertexArrays(1, &fullscreenTriangleVAO);

	GLuint shaderProgram = generateShaders();
	
	glLinkProgram(shaderProgram);

	if (!validateProgram(shaderProgram)) {
		fprintf(stderr, "Error while validating shader.\n");
		glfwTerminate();
		glDeleteVertexArrays(1, &fullscreenTriangleVAO);
		delete[] buffer->data;
		return -1;
	}

	glUseProgram(shaderProgram);

	GLint location = glGetUniformLocation(shaderProgram, "buffer");
	glUniform1i(location, 0);

	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(fullscreenTriangleVAO);
	#pragma endregion

	Game* game = new Game(buffer);

	game->setup();

	glfwSwapInterval(1);

	int playerMovementDirection = 1;
	gameIsRunning = true;
	while (!glfwWindowShouldClose(window) && gameIsRunning)
	{
		game->paintBackground(BLACK_UINT32);

		game->updateAliens();
		game->drawScore();
		game->drawBullets();
		game->drawPlayer();

		// Fill texture with buffer data and draw it on the screen
		glTexSubImage2D(
			GL_TEXTURE_2D, 0, 0, 0,
			buffer->width, buffer->height,
			GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
			buffer->data
		);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glfwSwapBuffers(window);
		
		game->enemyFire();
		game->updateDeathCounters();
		game->updateBullets();

		int playerMovementDirection = 2 * movementDirection;
		game->updatePlayer(playerMovementDirection);

		if (firePressed) {
			game->createPlayerBullet();
			firePressed = false;
		}

		//Poll for events
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteVertexArrays(1, &fullscreenTriangleVAO);

	// Clear all data
	delete[] buffer->data;
	delete game;

	return 0;
}

/*-----------------------------------------
	SHADER FUNCTIONS
-----------------------------------------*/

GLuint generateShaders() {
	char vertexShaderString[1024 * 256];
	char fragmentShaderString[1024 * 256];

	parse_file_into_str(VERTEX_SHADER_PATH, vertexShaderString, 1024 * 256);
	parse_file_into_str(FRAGMENT_SHADER_PATH, fragmentShaderString, 1024 * 256);


	const GLchar *vertexShader = (GLchar *) vertexShaderString;
	const GLchar *fragmentShader = (GLchar *)fragmentShaderString;

	GLuint shaderProgram = glCreateProgram();

	attachShader(shaderProgram, vertexShader, GL_VERTEX_SHADER);
	attachShader(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);

	return shaderProgram;
}

void attachShader(GLuint shaderProgram, const GLchar* shader, int type) {
	GLuint shader_vp = glCreateShader(type);

	glShaderSource(shader_vp, 1, &shader, 0);
	glCompileShader(shader_vp);
	validateShader(shader_vp, shader);
	glAttachShader(shaderProgram, shader_vp);

	glDeleteShader(shader_vp);
}

void validateShader(GLuint shader, const char *file = 0) {
	static const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	GLsizei length = 0;

	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

	if (length > 0) {
		printf("Shader %d(%s) compile error: %s\n", shader, (file ? file : ""), buffer);
	}
}

bool validateProgram(GLuint program) {
	static const GLsizei BUFFER_SIZE = 512;
	GLchar buffer[BUFFER_SIZE];
	GLsizei length = 0;

	glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

	if (length > 0) {
		printf("Program %d link error: %s\n", program, buffer);
		return false;
	}

	return true;
}

/*-----------------------------------------
	CALLBACKS
-----------------------------------------*/
void errorCallback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) gameIsRunning = false;
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS) movementDirection += 1;
		else if (action == GLFW_RELEASE) movementDirection -= 1;
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS) movementDirection -= 1;
		else if (action == GLFW_RELEASE) movementDirection += 1;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_RELEASE) firePressed = true;
		break;
	default:
		break;
	}
}