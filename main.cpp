#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#include <SDL_opengl.h>

//New Stuff
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

//Constants
const int WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 480;

//Background colors
const float BG_RED = 0.1922f, 
			BG_BLUE = 0.549f, 
			BG_GREEN = 0.9059f;
const float BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0, 
			VIEWPORT_Y = 0, 
			VIEWPORT_WIDTH = WINDOW_WIDTH, 
			VIEWPORT_HEIGHT = WINDOW_HEIGHT;

//Shader filepath
const char V_SHADER_PATH[] = "shaders/vertex/glsl", 
			F_SHADER_PATH[] = "shaders/fragment.glsl";

//Old
SDL_Window* display_window;
bool game_is_running = true;
//New
ShaderProgram program;
glm::mat4 view_matrix;	//Define the position(location and direction) of the camera
glm::mat4 model_matrix;	//Define every single transformation we apply to the rendered object
glm::mat4 projection_matrix;	//Define the characteristics of your camera(clip panes, filed of view, projection method, etc.)



void initialise()
{
	/* Old stuff */
	SDL_Init(SDL_INIT_VIDEO);
	display_window = SDL_CreateWindow("Hello, Triangle!",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(display_window);
	SDL_GL_MakeCurrent(display_window, context);

#ifdef _WINDOWS
	glewInit();
#endif

	/* New stuff */
	glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_HEIGHT, VIEWPORT_HEIGHT);   // Initialise our camera

	program.load(V_SHADER_PATH, F_SHADER_PATH);     // Load up our shaders

	// Initialise our view, model, and projection matrices
	view_matrix = glm::mat4(1.0f);
	model_matrix = glm::mat4(1.0f);
	projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);  // Orthographic means perpendicular—meaning that our camera will be looking perpendicularly down to our triangle

	program.setViewMatrix(view_matrix);
	program.setProjectionMatrix(projection_matrix);
	// Notice we haven't set our model matrix yet!

	program.SetColor(TRIANGLE_RED, TRIANGLE_BLUE, TRIANGLE_GREEN, TRIANGLE_OPACITY);
	glUseProgram(program.programID);

	// Old stuff
	glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
}

void process_input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			//if the user quits or closes the window, our game is over
			game_is_running = false;
		}
	}
}

void update()
{
	// This scale vector will make the x- and y-coordinates of the triangle
	// grow by a factor of 1% of it of its original size every frame.
	float scale_factor = 1.01;
	glm::vec3 scale_vector = glm::vec3(scale_factor, scale_factor, 1.0f);

	// We replace the previous value of the model matrix with the scaled
	// value of model matrix. This would mean that  glm::scale() returns
	// a matrix, which it does!
	model_matrix = glm::scale(model_matrix, scale_vector);
}

void render() {
	// Step 1
	glClear(GL_COLOR_BUFFER_BIT);

	// Step 2
	program.SetModelMatrix(model_matrix);

	// Step 3
	float vertices[] =
	{
		0.5f, -0.5f,
		0.0f, 0.5f,
		-0.5f, -0.5f
	};

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(program.positionAttribute);

	// Step 4
	SDL_GL_SwapWindow(display_window);
}

void shutdown() {
	SDL_QUIT();
}


int mmain(int arg, char* argv[]) {
	//STEP 1: Get everything set up and ready
	initialise();

	//STEP 2: Game loop
	while (game_is_running){
		//STEP 3: Check for any input from the user
		process_input();
		//STEP 4: Update our scene, maybe using the input from user
		update();
		//STEP 5: Get those updates onto our current screen
		render();

	}
	//STEP 6: Shut our engine down
	shutdown();
	return 0;
}