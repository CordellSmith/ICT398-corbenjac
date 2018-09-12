#include "GL\glew.h"
#include "GLFWManager.h"
#include "GameControlEngine.h"

int main()
{
	// Create new GLFW window
	GLFWManager* pWindowManager = new GLFWManager();

	// Create new camera object
	Camera* camera = new Camera(0.0);

	GameControlEngine engine;
	engine.SetWindowManager(pWindowManager);

	// Pass camera object into engine
	engine.SetCamera(camera);
	pWindowManager->GetInputManager()->SetCamera(camera);
	
	return engine.RunEngine();
}

int GLFWManager::Initialize(int width, int height, std::string strTitle, bool bFullScreen)
{
	if (!glfwInit()) 
	{
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	// GLFW setup
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window
	m_window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr, nullptr);

	// Make sure the window is valid
	if (m_window == nullptr)
	{
		std::cout << "Failed to create a GLFW window! Press any key to continue!" << std::endl;
		system("Pause");
		Destroy();

		return -1;
	}

	// Sets window user pointer
	glfwSetWindowUserPointer(m_window, this);

	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Cursor properties
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(m_window, 0, 0);
	//glfwSetScrollCallback(m_window, scroll_callback); scroll callback not currently needed

	// Turn of V-Sync
	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if ( GLEW_OK != err )
	{
		std::cout << "Failed to initialize glew!" << std::endl;
		return -1;
	}

	srand(time(NULL));
	return 0;
}

void GLFWManager::SwapTheBuffers()
{
	glfwSwapBuffers(m_window);
}

// This function processes all the application's input and returns a bool to tell us if we should continue
bool GLFWManager::ProcessInput(bool continueGame = true)
{
	// Use the GLFW function to check for the user pressing the Escape button, as well as a window close event.
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(m_window) != 0)
		return false;

	if (glfwGetKey(m_window, GLFW_KEY_UP) || glfwGetKey(m_window, GLFW_KEY_W))
		m_inputManager.KeyPressed(InputCodes::Up);
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) || glfwGetKey(m_window, GLFW_KEY_S))
		m_inputManager.KeyPressed(InputCodes::Down);
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) || glfwGetKey(m_window, GLFW_KEY_A))
		m_inputManager.KeyPressed(InputCodes::Left);
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) || glfwGetKey(m_window, GLFW_KEY_D))
		m_inputManager.KeyPressed(InputCodes::Right);

	// Used to toggle wireframe, Q to toggle on and E to toggle off
	if (glfwGetKey(m_window, GLFW_KEY_Q))
		m_inputManager.KeyPressed(InputCodes::q);
	if (glfwGetKey(m_window, GLFW_KEY_E))
		m_inputManager.KeyPressed(InputCodes::e);

	// Mouse position
	double mouseX, mouseY;

	// Current mouse position
	glfwGetCursorPos(m_window, &mouseX, &mouseY);
	
	glfwSetCursorPos(m_window, 0, 0);

	// Tell input manager the mouse button has been (is being) clicked
	m_inputManager.MouseMove((float)mouseX, (float)mouseY);

	glfwPollEvents();

	return continueGame;
}

void GLFWManager::Destroy()
{
	glfwTerminate();
}