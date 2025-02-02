#include "Window.h"

#include "Engine.h"
#include "Renderer/Renderer.h"

#include <glad/gl.h>
#include <glfw/glfw3.h>

#include <iostream>

Window::Window()
{
    // Initialize the library
    if (!glfwInit())
    {
        std::cout << "[Error] Failed to initialize GLFW.\n";
    }

    int major = 3, minor = 3;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

    // Create a windowed mode window and its OpenGL context
    std::string title = "Comet (OpenGL " + std::to_string(major) + "." + std::to_string(minor) + ")";
    m_GLFWwindow = glfwCreateWindow(1, 1, title.c_str(), NULL, NULL);
    if (!m_GLFWwindow)
    {
        std::cout << "[Error] Failed to create OpenGL window.\n";
        glfwTerminate();
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_GLFWwindow);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "[Error] Failed to initialize OpenGL context.\n";
    }

    glfwSwapInterval(1);

    // Centers window to current monitor
    Center();

    glfwSetWindowSizeCallback(glfwGetCurrentContext(), [](GLFWwindow* window, int width, int height) {});
    glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
}

Window::~Window()
{
    glfwSetWindowShouldClose(m_GLFWwindow, true);
}

void Window::Update()
{
    Renderer::Update();
}

void Window::Center()
{
    int count;
    int monitorX, monitorY;

    GLFWmonitor** monitors = glfwGetMonitors(&count);
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);

    int newWindowWidth = int(videoMode->width / 1.5);
    int newWindowHeight = int(newWindowWidth / 16 * 9);

    SetWindowWidth(newWindowWidth);
    SetWindowHeight(newWindowHeight);

    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    glfwSetWindowPos(glfwGetCurrentContext(), monitorX + (videoMode->width - newWindowWidth) / 2, monitorY + (videoMode->height - newWindowHeight) / 2);
    glfwSetWindowSize(glfwGetCurrentContext(), newWindowWidth, newWindowHeight);
    glViewport(0, 0, newWindowWidth, newWindowHeight);
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_GLFWwindow);
}

void Window::SetShouldClose(bool flag)
{
    glfwSetWindowShouldClose(m_GLFWwindow, flag);
}