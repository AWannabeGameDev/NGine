#include <glad/glad.h>

#include <stdio.h>
#include <stdexcept>

#include "ngine/window/window.hpp"

ng::Window::Window(int width, int height, std::string_view title) :
	_window {s_createWindow(width, height, title)},
	_width {width}, _height {height}
{
	glfwSetWindowUserPointer(_window, this);

	for(auto& keyStatus : _keys)
	{
		keyStatus = false;
	}

	for(auto& mouseButtonStatus : _mouseButtons)
	{
		mouseButtonStatus = false;
	}
}

ng::Window::~Window()
{
	glfwTerminate();
}

GLFWwindow* ng::Window::s_createWindow(int width, int height, std::string_view title)
{
	if(glfwInit() != GLFW_TRUE)
	{
		throw std::runtime_error {"Failed to initialize GLFW."};
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	if(window == nullptr)
	{
		throw std::runtime_error {"Failed to create window."};
	}

	glfwMakeContextCurrent(window);

	if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
	{
		throw std::runtime_error {"Failed to initialize GLAD."};
	}

	printf("Using OpenGL version : %s\n", glGetString(GL_VERSION));

	glfwSetWindowAspectRatio(window, width, height);
	glfwSetKeyCallback(window, s_keyCallback);
	glfwSetMouseButtonCallback(window, s_mouseButtonCallback);
	glfwSetCursorPosCallback(window, s_mouseMoveCallback);
	glfwSetFramebufferSizeCallback(window, s_resizeCallback);

	return window;
}

void ng::Window::s_keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
{
	Window* window {(Window*)glfwGetWindowUserPointer(glfwWindow)};

	if((key >= 0) && (key < window->_keys.size()))
	{
		if(action == GLFW_PRESS)
		{
			window->_keys[key] = true;
		}
		else if(action == GLFW_RELEASE)
		{
			window->_keys[key] = false;
		}
	}
}

void ng::Window::s_mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
{
	Window* window {(Window*)glfwGetWindowUserPointer(glfwWindow)};

	if((button >= 0) && (button < window->_keys.size()))
	{
		if(action == GLFW_PRESS)
		{
			window->_mouseButtons[button] = true;
		}
		else if(action == GLFW_RELEASE)
		{
			window->_mouseButtons[button] = false;
		}
	}
}

void ng::Window::s_mouseMoveCallback(GLFWwindow* glfwWindow, double x, double y)
{
	Window* window {(Window*)glfwGetWindowUserPointer(glfwWindow)};

	window->_mouseX = (x / (window->_width / 2.0)) - 1.0;
	window->_mouseY = 1.0 - (y / (window->_height / 2.0));
}

void ng::Window::s_resizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
	Window* window {(Window*)glfwGetWindowUserPointer(glfwWindow)};

	window->_width = width;
	window->_height = height;
	window->_resizeCallback(width, height);
}

int ng::Window::width() const
{
	return _width;
}

int ng::Window::height() const
{
	return _height;
}

bool ng::Window::userClosedWindow() const
{
	return glfwWindowShouldClose(_window);
}

void ng::Window::pollEvents()
{
	_previousTime = _currentTime;
	_currentTime = glfwGetTime();

	_previousMouseX = _mouseX;
	_previousMouseY = _mouseY;

	glfwPollEvents();
}

void ng::Window::setResizeCallback(const std::function<void(int, int)>& callback)
{
	_resizeCallback = callback;
}

bool ng::Window::keyPressed(int keycode) const
{
	return _keys[keycode];
}

bool ng::Window::mouseButtonPressed(int buttonCode) const
{
	return _mouseButtons[buttonCode];
}

double ng::Window::mouseX() const
{
	return _mouseX;
}

double ng::Window::deltaMouseX() const
{
	return _mouseX - _previousMouseX;
}

double ng::Window::mouseY() const
{
	return _mouseY;
}

double ng::Window::deltaMouseY() const
{
	return _mouseY - _previousMouseY;
}

double ng::Window::deltaTime() const
{
	return _currentTime - _previousTime;
}

void ng::Window::swapBuffers()
{
	glfwSwapBuffers(_window);
}