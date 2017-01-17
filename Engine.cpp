#include "Engine.h"
#include "Tile.h"
#include <vector>
#include "InputDecoder.h"

Engine* Engine::instance;


Engine* Engine::getInstance()
{
	if(instance == nullptr)
	{
		instance = new Engine();
	}
	return instance;
}

Engine::Engine()
{
	winWidth = 1400;
	winHeight = 920;
	initialize();
}


void Engine::initialize()
{
	initGLFW();
	initGLEW();
	createShaders();
	loadTextures();
	setupVertexes();

	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	world = new World(0, 0);
	InputDecoder inputDecoder("input_a1.txt");
	world->spawnWorldObject(inputDecoder.decodeWorldObject());
}

void Engine::initGLFW()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}
void Engine::initGLEW()
{
	window = glfwCreateWindow(winWidth, winHeight, "Fred's rendering engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	glViewport(0, 0, winWidth, winHeight);

	lastMouseX = winWidth / 2;
	lastMouseY = winHeight / 2;

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Engine::createShaders()
{
	textureShader = new Shader("shader.vs", "shader.fs");
}

void Engine::loadTextures()
{

	// ====================
	// Texture 1
	// ====================
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("metal.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
									 // ===================
									 // Texture 2
									 // ===================
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Engine::setupVertexes()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Engine::run()
{
	std::vector<std::vector<Tile*>*>* tiles = new std::vector<std::vector<Tile*>*>();
	for(int x=0; x < 25; x++)
	{
		std::vector<Tile*>* vec1 = new std::vector<Tile*>();
		for (int y = 0; y < 25; y++)
		{
			vec1->push_back(new Tile(x, y));
		}

		tiles->push_back(vec1);
	}
	
	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//ontick
		onTick();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		textureShader->use();



		glm::mat4 view;
		view = camera->getViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(camera->zoom, GLfloat(winWidth) / GLfloat(winHeight), 0.1f, 1000.0f);

		
		GLuint modelLoc = glGetUniformLocation(textureShader->program, "model");
		
		GLuint viewLoc = glGetUniformLocation(textureShader->program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		GLuint projectionLoc = glGetUniformLocation(textureShader->program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		GLuint colorLoc = glGetUniformLocation(textureShader->program, "glowColor");

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(textureShader->program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(textureShader->program, "ourTexture2"), 1);

		for(int i=0; i < world->getWorldObjects()->size(); i++)
		{
			WorldObject* object = world->getWorldObjects()->at(i);
			object->render();
		}

		// Draw container
		glBindVertexArray(VAO);

		

		for(GLuint x = 0; x < tiles->size(); x++)
		{
			std::vector<Tile*>* tileVec = tiles->at(x);

			for(GLuint y = 0; y < tileVec->size(); y++)
			{
				GLfloat dx, dy;
				dx = x - camera->position.x;
				dy = y - camera->position.z;

				GLfloat distance = abs(sqrt(dx*dx + dy*dy));

				if (distance < 5)
				{
					glUniform1i(glGetUniformLocation(textureShader->program, "ourTexture1"), 0);
				}
				else
				{
					glUniform1i(glGetUniformLocation(textureShader->program, "ourTexture1"), 1);
				}

				Tile* tile = tileVec->at(y);

				for(GLint z=0; z < distance && z < 6; z++)
				{
					
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(x, z-5 , y));

					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				

				

			}
			
		}

		
		
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void Engine::onTick()
{
	calculateDeltaTime();



	if (glfwGetKey(window, GLFW_KEY_W))
		camera->processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S))
		camera->processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A))
		camera->processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D))
		camera->processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE))
		camera->processKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		camera->processKeyboard(DOWN, deltaTime);



}

void Engine::calculateDeltaTime()
{
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}


void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	getInstance()->keyEvent(window, key, scancode, action, mode);
}
void Engine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	getInstance()->mouseEvent(window, xpos, ypos);
}
void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	getInstance()->scrollEvent(window, xoffset, yoffset);
}

void Engine::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		wireframe = !wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, wireframe?GL_LINE:GL_FILL);
	}


	GLfloat cameraSpeed = 0.05f;


}

void Engine::mouseEvent(GLFWwindow* window, double xpos, double ypos)
{
	
	if (firstMouse) 
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastMouseX;
	GLfloat yoffset = ypos - lastMouseY;

	lastMouseX = xpos;
	lastMouseY = ypos;

	camera->processMouseMovement(xoffset, yoffset);
}

void Engine::scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->processMouseScrolls(yoffset);
}
