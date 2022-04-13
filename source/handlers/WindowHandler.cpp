#include "WindowHandler.h"

void WindowHandler::Initialize()
{
    Instance().CreateWindow();
    Instance().SetupCallbacks();
}

int WindowHandler::CreateWindow()
{
    // Initialize the library
    if (!glfwInit())
    {
        std::cout << "[Error] Failed to initialize GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Create a windowed mode window and its OpenGL context
    p_GLFWwindow = glfwCreateWindow(1, 1, "Comet (OpenGL 4.6)", NULL, NULL);
    if (!p_GLFWwindow)
    {
        std::cout << "[Error] Failed to create OpenGL window.\n";
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(p_GLFWwindow);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "[Error] Failed to initialize OpenGL context.\n";
        return -1;
    }

    glfwSetWindowUserPointer(p_GLFWwindow, this);
    glfwSwapInterval(1);

    CenterWindow();

    glfwShowWindow(p_GLFWwindow);

    return 0;
}

void WindowHandler::CenterWindow()
{
    int count;
    int monitorX, monitorY;

    GLFWmonitor **monitors = glfwGetMonitors(&count);
    const GLFWvidmode *videoMode = glfwGetVideoMode(monitors[0]);

    int newWindowWidth = int(videoMode->width / 1.5);
    int newWindowHeight = int(newWindowWidth / 16 * 9);

    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    glfwSetWindowPos(glfwGetCurrentContext(),
                     monitorX + (videoMode->width - newWindowWidth) / 2,
                     monitorY + (videoMode->height - newWindowHeight) / 2);
    glfwSetWindowSize(glfwGetCurrentContext(), newWindowWidth, newWindowHeight);
    glViewport(0, 0, newWindowWidth, newWindowHeight);
}

void WindowHandler::SetupCallbacks()
{
    glfwSetWindowUserPointer(glfwGetCurrentContext(), &WindowHandler::Instance());

    auto WindowSizeCallbackWrapper = [](GLFWwindow *window, int width, int height)
    {
        static_cast<WindowHandler *>(glfwGetWindowUserPointer(window))->WindowSizeCallback(width, height);
    };
    auto FrameBufferCallbackWrapper = [](GLFWwindow *window, int width, int height)
    {
        static_cast<WindowHandler *>(glfwGetWindowUserPointer(window))->FramebufferSizeCallback(width, height);
    };

    glfwSetWindowSizeCallback(glfwGetCurrentContext(), WindowSizeCallbackWrapper);
    glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), FrameBufferCallbackWrapper);
}

void WindowHandler::WindowSizeCallback(int width, int height)
{
}

void WindowHandler::FramebufferSizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
}

bool WindowHandler::ShouldWindowClose()
{
    return glfwWindowShouldClose(Instance().p_GLFWwindow);
}

GLFWwindow *WindowHandler::GetGLFWWindow()
{
    return Instance().p_GLFWwindow;
}