#ifndef NG_WINDOW_HPP
#define NG_WINDOW_HPP

#include <GLFW/glfw_custom.hpp>

#include <array>
#include <string_view>
#include <functional>

namespace ng
{

class Window
{
private:
	GLFWwindow* _window;
	int _width;
	int _height;

	std::array<bool, GLFW_KEY_LAST + 1> _keys {};
	std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> _mouseButtons {};

	double _mouseX {0.0};
	double _mouseY {0.0};
	double _previousMouseX {0.0};
	double _previousMouseY {0.0};

	double _previousTime {0.0};
	double _currentTime {0.0};

	std::function<void(int, int)> _resizeCallback {[] (int, int) {}};

	static void s_keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
	static void s_mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
	static void s_mouseMoveCallback(GLFWwindow* glfwWindow, double x, double y);
	static void s_resizeCallback(GLFWwindow* glfwWindow, int width, int height);

	static GLFWwindow* s_createWindow(int width, int height, std::string_view title);

public:
	Window(int width, int height, std::string_view title);
	~Window();

	Window(const Window& other) = delete;
	Window& operator=(const Window& other) = delete;

	int width() const;
	int height() const;
	bool userClosedWindow() const;

	void pollEvents();

	/**
	 * The parameters are the new window dimensions.
	 * Use caution when setting this to a reference/pointer capturing lambda, because then 
	 * the safety of calling the resize callback depends on the lifetime of the referred object
	 * (the referred object must live at least until the lifetime of the window).
	 */
	void setResizeCallback(const std::function<void(int, int)>& callback);

	bool keyPressed(int keycode) const;
	bool mouseButtonPressed(int buttonCode) const;

	// Window coordinates are normalized to the range [-1, 1]
	double mouseX() const;
	double deltaMouseX() const;
	double mouseY() const;
	double deltaMouseY() const;

	double deltaTime() const;

	void swapBuffers();
};

}

#endif