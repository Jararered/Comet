#include "MouseHandler.h"

void MouseHandler::Initialize() { Instance().SetupCallbacks(); }

void MouseHandler::SetupCallbacks()
{
  glfwSetWindowUserPointer(glfwGetCurrentContext(), &MouseHandler::Instance());

  auto ScrollCallbackWrapper = [](GLFWwindow *window, double xoffset,
                                  double yoffset) {
    static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))
        ->ScrollCallback(xoffset, yoffset);
  };
  auto MouseButtonCallbackWrapper = [](GLFWwindow *window, int button,
                                       int action, int mods) {
    static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))
        ->MouseButtonCallback(button, action, mods);
  };
  auto CursorPosCallbackWrapper = [](GLFWwindow *window, double xpos,
                                     double ypos) {
    static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))
        ->CursorPosCallback(xpos, ypos);
  };

  glfwSetScrollCallback(glfwGetCurrentContext(), ScrollCallbackWrapper);
  glfwSetMouseButtonCallback(glfwGetCurrentContext(),
                             MouseButtonCallbackWrapper);
  glfwSetCursorPosCallback(glfwGetCurrentContext(), CursorPosCallbackWrapper);
}

void MouseHandler::ScrollCallback(double xoffset, double yoffset) {}

void MouseHandler::MouseButtonCallback(int button, int action, int mods)
{
  // Captures cursor if not currently captured, reguardless of mouse button
  if (action == GLFW_PRESS && !m_CursorCaptured)
  {
    if (glfwRawMouseMotionSupported())
    {
      glfwSetInputMode(WindowHandler::GetGLFWWindow(), GLFW_RAW_MOUSE_MOTION,
                       GLFW_TRUE);
    }

    CaptureCursor();

    // Need to center cursor before cursor position callback is run
    // Prevents a possibly large xpos/ypos when entering the window
    glfwSetCursorPos(WindowHandler::GetGLFWWindow(), 0.0, 0.0);
  }
}

void MouseHandler::CursorPosCallback(double xpos, double ypos)
{
  // Only tracks cursor movement if the cursor is captured
  if (m_CursorCaptured)
  {
    // Adding up all movement since the last frame was rendered
    m_MovementSinceLastFrame[0] += xpos;
    m_MovementSinceLastFrame[1] -= ypos;

    // Setting cursor back so next callback is relative from center again
    glfwSetCursorPos(WindowHandler::GetGLFWWindow(), 0.0, 0.0);
  }
  else
  {
    // No movement if not tracking cursor
    m_MovementSinceLastFrame = {0.0, 0.0};
  }
}

void MouseHandler::UpdateStates()
{
  if (glfwGetMouseButton(WindowHandler::GetGLFWWindow(),
                         GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    if (!Instance().m_LeftClick && !Instance().m_LeftHold)
    {
      Instance().m_LeftClick = true;
    }
    else
    {
      Instance().m_LeftClick = false;
      Instance().m_LeftHold = true;
    }
  }
  else
  {
    Instance().m_LeftClick = false;
    Instance().m_LeftHold = false;
  }

  if (glfwGetMouseButton(WindowHandler::GetGLFWWindow(),
                         GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
  {
    if (!Instance().m_RightClick && !Instance().m_RightHold)
    {
      Instance().m_RightClick = true;
    }
    else
    {
      Instance().m_RightClick = false;
      Instance().m_RightHold = true;
    }
  }
  else
  {
    Instance().m_RightClick = false;
    Instance().m_RightHold = false;
  }
}

void MouseHandler::ResetStates()
{
  Instance().m_LeftClick = false;
  Instance().m_LeftHold = false;
  Instance().m_RightClick = false;
  Instance().m_RightHold = false;
}

void MouseHandler::CaptureCursor()
{
  glfwSetInputMode(WindowHandler::GetGLFWWindow(), GLFW_CURSOR,
                   GLFW_CURSOR_DISABLED);
  Instance().m_CursorCaptured = true;
}
void MouseHandler::ReleaseCursor()
{
  glfwSetInputMode(WindowHandler::GetGLFWWindow(), GLFW_CURSOR,
                   GLFW_CURSOR_NORMAL);
  Instance().m_CursorCaptured = false;
}

double MouseHandler::GetDeltaX()
{
  return Instance().m_MovementSinceLastFrame[0];
}
double MouseHandler::GetDeltaY()
{
  return Instance().m_MovementSinceLastFrame[1];
}

bool MouseHandler::GetLeftClick() { return Instance().m_LeftClick; }

bool MouseHandler::GetLeftHold() { return Instance().m_LeftHold; }

bool MouseHandler::GetRightClick() { return Instance().m_RightClick; }

bool MouseHandler::GetRightHold() { return Instance().m_RightHold; }

void MouseHandler::ResetMovement()
{
  Instance().m_MovementSinceLastFrame = {0.0, 0.0};
}