#include <cstdio>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#include "Buffer.h"
#include "Sprite.h"
#include "PlayerSprite.h"
#include "BulletSprite.h"
#include "GameConfig.h"
#include "Game.h"
#include "Alien.h"

#define APPLICATION_NAME "Space Invaders"
#define FRAGMENT_SHADER_PATH "fragment_shader.fs"
#define VERTEX_SHADER_PATH "vertex_shader.vs"

bool gameIsRunning = false;
int movementDirection = 0;
bool firePressed = false;
size_t score = 0;
size_t credits = 0;

// Structs

enum AlienType : uint8_t
{
	ALIEN_DEAD = 0,
	ALIEN_TYPE_A = 1,
	ALIEN_TYPE_B = 2,
	ALIEN_TYPE_C = 3
};

// Shader Functions
GLuint generateShaders();
void attachShader(GLuint shaderProgram, const GLchar* shader, int type);
void validateShader(GLuint shader, const char *file);
bool validateProgram(GLuint program);

// Callbacks
void errorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Sprite functions
bool spriteOverlapCheck(const Sprite& sp_a, size_t x_a, size_t y_a, const Sprite& sp_b, size_t x_b, size_t y_b);

// Formatting functions
uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b);

int main(int argc, char* argv[])
{
	const size_t bufferWidth = 224;
	const size_t bufferHeight = 256;

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
	buffer->colorClear(0);

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


	//OpenGL setup
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(fullscreenTriangleVAO);

	// Prepare game
	Sprite* alienSprites[6];

	alienSprites[0] = new Sprite(8, 8);
	alienSprites[0]->setData(new uint8_t[64]
	{
		0,0,0,1,1,0,0,0, // ...@@...
		0,0,1,1,1,1,0,0, // ..@@@@..
		0,1,1,1,1,1,1,0, // .@@@@@@.
		1,1,0,1,1,0,1,1, // @@.@@.@@
		1,1,1,1,1,1,1,1, // @@@@@@@@
		0,1,0,1,1,0,1,0, // .@.@@.@.
		1,0,0,0,0,0,0,1, // @......@
		0,1,0,0,0,0,1,0  // .@....@.
	});

	alienSprites[1] = new Sprite(8, 8);
	alienSprites[1]->setData(new uint8_t[64]
	{
		0,0,0,1,1,0,0,0, // ...@@...
		0,0,1,1,1,1,0,0, // ..@@@@..
		0,1,1,1,1,1,1,0, // .@@@@@@.
		1,1,0,1,1,0,1,1, // @@.@@.@@
		1,1,1,1,1,1,1,1, // @@@@@@@@
		0,0,1,0,0,1,0,0, // ..@..@..
		0,1,0,1,1,0,1,0, // .@.@@.@.
		1,0,1,0,0,1,0,1  // @.@..@.@
	});

	alienSprites[2] = new Sprite(11, 8);
	alienSprites[2]->setData(new uint8_t[88]
	{
		0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
		0,0,0,1,0,0,0,1,0,0,0, // ...@...@...
		0,0,1,1,1,1,1,1,1,0,0, // ..@@@@@@@..
		0,1,1,0,1,1,1,0,1,1,0, // .@@.@@@.@@.
		1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
		1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
		1,0,1,0,0,0,0,0,1,0,1, // @.@.....@.@
		0,0,0,1,1,0,1,1,0,0,0  // ...@@.@@...
	});

	alienSprites[3] = new Sprite(11, 8);
	alienSprites[3]->setData(new uint8_t[88]
	{
		0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
		1,0,0,1,0,0,0,1,0,0,1, // @..@...@..@
		1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
		1,1,1,0,1,1,1,0,1,1,1, // @@@.@@@.@@@
		1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
		0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
		0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
		0,1,0,0,0,0,0,0,0,1,0  // .@.......@.
	});

	alienSprites[4] = new Sprite(12, 8);
	alienSprites[4]->setData(new uint8_t[96]
	{
		0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
		0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
		1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
		1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
		1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
		0,0,0,1,1,0,0,1,1,0,0,0, // ...@@..@@...
		0,0,1,1,0,1,1,0,1,1,0,0, // ..@@.@@.@@..
		1,1,0,0,0,0,0,0,0,0,1,1  // @@........@@
	});

	alienSprites[5] = new Sprite(12, 8);
	alienSprites[5]->setData(new uint8_t[96]
	{
		0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
		0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
		1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
		1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
		1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
		0,0,1,1,1,0,0,1,1,1,0,0, // ..@@@..@@@..
		0,1,1,0,0,1,1,0,0,1,1,0, // .@@..@@..@@.
		0,0,1,1,0,0,0,0,1,1,0,0  // ..@@....@@..
	});

	Sprite* alienDeathSprite = new Sprite(13, 7);
	alienDeathSprite->setData(new uint8_t[91]
	{
		0,1,0,0,1,0,0,0,1,0,0,1,0, // .@..@...@..@.
		0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
		0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
		1,1,0,0,0,0,0,0,0,0,0,1,1, // @@.........@@
		0,0,0,1,0,0,0,0,0,1,0,0,0, // ...@.....@...
		0,0,1,0,0,1,0,1,0,0,1,0,0, // ..@..@.@..@..
		0,1,0,0,1,0,0,0,1,0,0,1,0  // .@..@...@..@.
	});

	Sprite* playerSprite = new PlayerSprite(11, 7);

	Sprite* textSpritesheet = new Sprite(5,7);
	textSpritesheet->setData(new uint8_t[65 * 35]
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
		0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,0,1,0,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,0,1,0,1,0,
		0,0,1,0,0,0,1,1,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,1,0,0,
		1,1,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,
		0,1,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,1,
		0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
		0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,1,0,0,
		0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
		0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,

		0,1,1,1,0,1,0,0,0,1,1,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,1,1,0,
		0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,
		0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,
		1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
		0,0,0,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,
		1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
		0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
		1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,
		0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
		0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,

		0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
		0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
		0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
		0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
		0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,0,1,0,0,0,1,0,1,1,1,0,

		0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,
		1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,
		0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,0,
		1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,
		1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,
		1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
		0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
		1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
		0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,
		0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
		1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,
		1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,
		1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
		1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,
		0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
		1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
		0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,1,1,1,
		1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,
		0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,
		1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,
		1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
		1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,
		1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,1,
		1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,
		1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,
		1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,

		0,0,0,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,
		0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,
		1,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,
		0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
		0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	});

	Sprite numberSpritesheet = *textSpritesheet;
	numberSpritesheet.data += 16 * 35;


	Sprite* bulletSprite = new BulletSprite();

	Game* game = new Game(bufferWidth, bufferHeight);

	game->createAlienMatrix();

	uint32_t backgroundColor = rgbToUint32(0, 0, 0);

	glfwSwapInterval(1);

	int playerMovementDirection = 1;
	gameIsRunning = true;
	while (!glfwWindowShouldClose(window) && gameIsRunning)
	{

		// TODO: Move everything to Game Class
		// Alien sons just have the sprites different. Is inheritance the best way to do this? (maybe it is)
		buffer->colorClear(backgroundColor);

		// Draw credits
		char creditsText[16];
		sprintf(creditsText, "CREDIT %02lu", credits);
		buffer->drawText(*textSpritesheet, creditsText, 164, 7, rgbToUint32(255, 255, 255));

		// Draw score
		buffer->drawText(*textSpritesheet, "SCORE", 4, game->height - textSpritesheet->height - 7, rgbToUint32(255, 255, 255));
		buffer->drawNumber(numberSpritesheet, score, 4 + 2 * numberSpritesheet.width, game->height - 2 * numberSpritesheet.height - 12, rgbToUint32(255, 255, 255));
	

		// For each alien
		for (size_t ai = 0; ai < game->numAliens; ++ai) {
			Alien* alien = game->aliens[ai];
			// If deathCounter = 0, then do nothing and go to next alien (continue)
			if (!alien->deathCounter) continue;
			// if it got killed, draw death sprite
			if (alien->dead) {
				buffer->drawSprite(alienDeathSprite, alien->x, alien->y, rgbToUint32(255, 255, 255));
			}
			// else, change sprite (animation)
			else {
				Sprite* sprite = alien->getNextSprite();
				buffer->drawSprite(sprite, alien->x, alien->y, rgbToUint32(255, 255, 255));
				alien->animation.time++;
				if (alien->animation.time == alien->animation.numFrames * alien->animation.frameDuration) {
					alien->animation.time = 0;
				}
			}
		}

		// draw bullets on screen
		for (size_t bi = 0; bi < game->numBullets; ++bi) {
			const Bullet& bullet = game->bullets[bi];
			const Sprite& sprite = *bulletSprite;
			//buffer->drawSprite(sprite, bullet.x, bullet.y, rgbToUint32(255, 255, 255));
		}

		// Draw player
		buffer->drawSprite(playerSprite, game->player.x, game->player.y, rgbToUint32(255, 255, 255));

		// fill texture with data from the buffer;
		glTexSubImage2D(
			GL_TEXTURE_2D, 0, 0, 0,
			buffer->width, buffer->height,
			GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
			buffer->data
		);

		// Draw our fullscreen triangle
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(window);

		// ALIENS
		// Decrease deathcounter
		for (size_t ai = 0; ai < game->numAliens; ++ai) {
			Alien* alien = game->aliens[ai];
			if (false && alien->deathCounter) {
				alien->decreaseDeathCounter();
			}
		}

		// BULLETS
		// Add next X and Y of its movement
		for (size_t bi = 0; bi < game->numBullets;) {
			game->bullets[bi].y += game->bullets[bi].dir;
			// Border check
			if (game->bullets[bi].y >= game->height || game->bullets[bi].y < bulletSprite->height) {
				game->bullets[bi] = game->bullets[game->numBullets - 1];
				--game->numBullets;
				continue;
			}

			// Check if hit an alien
			for (size_t ai = 0; ai < game->numAliens; ++ai) {
				const Alien* alien = game->aliens[ai];
				if (false) continue;
				const SpriteAnimation& animation = alien->animation;
				size_t currentFrame = animation.time / animation.frameDuration;
				const Sprite& alienSprite = *animation.frames[currentFrame];
				bool overlap = spriteOverlapCheck(
					*bulletSprite, game->bullets[bi].x, game->bullets[bi].y,
					alienSprite, alien->x, alien->y
				);
				// If it hits, increase score and change alien type to dead
				if (overlap) {
					score += 10 * (4);
					// Adjust alien sprite X so the animation is centralized
					game->aliens[ai]->x -= (alienDeathSprite->width - alienSprite.width) / 2;
					game->bullets[bi] = game->bullets[game->numBullets - 1];
					--game->numBullets;
					continue;
				}
			}
			++bi;
		}

		// PLAYER
		int playerMovementDirection = 2 * movementDirection;

		// If it's not pressing both keys
		if (playerMovementDirection != 0) {
			if (game->player.x + playerSprite->width + playerMovementDirection >= game->width) {
				game->player.x = game->width - playerSprite->width;
			}
			else if ((int)game->player.x + playerMovementDirection <= 0) {
				game->player.x = 0;
			}
			else {
				game->player.x += playerMovementDirection;
			}
		}

		// Process events
		if (firePressed && game->numBullets < GAME_MAX_BULLETS) {
			game->bullets[game->numBullets].x = game->player.x + playerSprite->width / 2;
			game->bullets[game->numBullets].y = game->player.y + playerSprite->height;
			game->bullets[game->numBullets].dir = 2;
			++game->numBullets;
		}
		firePressed = false;

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteVertexArrays(1, &fullscreenTriangleVAO);

	// Clear alien sprites
	for (size_t i = 0; i < 6; ++i) {
		delete[] alienSprites[i]->data;
	}
	delete[] alienDeathSprite->data;

	// Clear all data
	delete[] textSpritesheet->data;
	delete[] buffer->data;
	delete[] game->aliens;

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


/*-----------------------------------------
	SPRITE FUNCTIONS
-----------------------------------------*/
bool spriteOverlapCheck(const Sprite& sp_a, size_t x_a, size_t y_a, const Sprite& sp_b, size_t x_b, size_t y_b) {
	if (x_a < x_b + sp_b.width && x_a + sp_a.width > x_b &&
		y_a < y_b + sp_b.height && y_a + sp_a.height > y_b)
	{
		return true;
	}

	return false;
}

/*-----------------------------------------
	FORMATTING FUNCTIONS
-----------------------------------------*/
uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b)
{
	return (r << 24) | (g << 16) | (b << 8) | 255;
}