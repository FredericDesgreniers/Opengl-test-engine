#include "Engine.h"


Engine* Engine::instance;

void error_callback(int error, const char* description)
{
	puts(description);
}
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
	winWidth = 800;
	winHeight = 800;
	initialize();
}


void Engine::initialize()
{
	initGLFW();
	initGLEW();
	createShaders();
	loadTextures();
	//setupVertexes();

	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	world = new World(100,100);
	world->generateRoads();

	int row = 0;
	int column = 0;
	for(int i=1; i <=9; i++)
	{
		InputDecoder inputDecoder("input_a"+std::to_string(i)+".txt");
		world->spawnWorldObject(inputDecoder.decodeWorldObject(row*5, 0, column*5));
		std::cout << i << ": " << row << "," << column << std::endl;
		row++;
		if (row >= 3) {
			column++;
			row = 0;
		}
	}



}

void Engine::initGLFW()
{
	// Init GLFW
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		std::cout << "INIT ERROR" << std::endl;
	}
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void Engine::initGLEW()
{
	window = glfwCreateWindow(winWidth, winHeight, "Fred's rendering engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	lastMouseX = winWidth / 2;
	lastMouseY = winHeight / 2;

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Engine::createShaders()
{
	//textureShader = new Shader("shader.vs", "shader.fs");
}

void Engine::loadTextures()
{
	/*

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
	unsigned char* image = SOIL_load_image("grass.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image == NULL) {
		std::cout << "An error occurred while loading image." << SOIL_last_result() << std::endl;

	}
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
	image = SOIL_load_image("metal.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
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


	glBindVertexArray(0); // Unbind VAO


}

void Engine::run()
{
	Shader shader("a1Shader.vs","a1Shader.fs");
	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//ontick
		onTick();

		// Render
		// Clear the colorbuffer
		glClearColor(255.0f,255.0f,255.0f,255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		//textureShader->use();



		glm::mat4 view;
		view = camera->getViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(camera->zoom, GLfloat(winWidth) / GLfloat(winHeight), 0.1f, 1000.0f);


		// Bind Textures using texture units
		int j = 0;
		
		shader.use();
		for(int i=0; i < world->getWorldObjects()->size(); i++)
		{
			WorldObject* object = world->getWorldObjects()->at(i);
			if (object == nullptr)
				continue;
			if (object->getVaoObject() == nullptr)
				continue;

			GLuint modelLoc = glGetUniformLocation(shader.program, "model");

			GLuint viewLoc = glGetUniformLocation(shader.program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			GLuint projectionLoc = glGetUniformLocation(shader.program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



			GLuint addColor = glGetUniformLocation(shader.program, "addColor");
			glUniform3f(addColor, selectedObject == i ? 0.2f : 0.0f,0.0f,0.0f);

			glBindVertexArray(object->getVaoObject()->getVAO());

			glm::vec3 rotation = object->rotation;

			glm::mat4 model;
			
			model = glm::translate(model, glm::vec3(object->getX(), object->getY(), object->getZ()));
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			object->render();

			j++;
			glBindVertexArray(0);
		}

		/*
		// Draw container
		glBindVertexArray(VAO);

		Tile*** tiles = world->getTiles();

		for(GLuint x = 0; x < world->getWidth(); x++)
		{
			for (GLuint y = 0; y < world->getHeight(); y++)
			{
				Tile* tile = tiles[x][y];

				glm::mat4 model;
				model = glm::translate(model, glm::vec3(x, -5, y));

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);

			}
		}
*		 */
		
		
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);

	}

	// Properly de-allocate all resources once they've outlived their purpose
	for(int i =0; i < world->getWorldObjects()->size(); i++)
	{
		if (world->getWorldObjects()->at(i)->getVaoObject() == nullptr)
			continue;
		GLuint v = world->getWorldObjects()->at(i)->getVaoObject()->getVAO();
		GLuint v1 = world->getWorldObjects()->at(i)->getVaoObject()->getVBO();
		GLuint e = world->getWorldObjects()->at(i)->getVaoObject()->getEBO();
		glDeleteVertexArrays(1, &v);
		glDeleteBuffers(1, &v1);
		glDeleteBuffers(1, &e);
	
		delete world->getWorldObjects()->at(i);
		
	}
	
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
	if (glfwGetKey(window, GLFW_KEY_UP)) 
		world->getWorldObjects()->at(selectedObject)->rotation.x -= deltaTime*100;
	if (glfwGetKey(window, GLFW_KEY_DOWN))
		world->getWorldObjects()->at(selectedObject)->rotation.x += deltaTime * 100;
	if (glfwGetKey(window, GLFW_KEY_LEFT))
		world->getWorldObjects()->at(selectedObject)->rotation.z += deltaTime * 100;
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
		world->getWorldObjects()->at(selectedObject)->rotation.z -= deltaTime * 100;
	



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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		glPointSize(10.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		camera->lookAtOrigin = !camera->lookAtOrigin;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if(key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		selectedObject--;
		if(selectedObject < 0)
		{
			selectedObject = world->getWorldObjects()->size() - 1;
		}
		WorldObject* obj = world->getWorldObjects()->at(selectedObject);
		camera->origin = glm::vec3(obj->getX(), obj->getY(), obj->getZ());
		std::cout << "Selected Object: " << selectedObject << std::endl;
	}
	if(key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		selectedObject++;
		if(selectedObject >= world->getWorldObjects()->size())
		{
			selectedObject = 0;
		}
		WorldObject* obj = world->getWorldObjects()->at(selectedObject);
		camera->origin = glm::vec3(obj->getX(), obj->getY(), obj->getZ());
		std::cout << "Selected Object: " << selectedObject << std::endl;
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

void Engine::window_size_callback(GLFWwindow* window, int width, int height)
{
	getInstance()->winWidth = width;
	getInstance()->winHeight = height;
	glViewport(0, 0, width, height);
}
void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}