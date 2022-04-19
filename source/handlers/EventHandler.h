#pragma once

#include <comet.pch>

class EventHandler
{
public:
  inline static auto &Instance()
  {
    static EventHandler instance;
    return instance;
  }

  static void Initialize();
  static void PollEvents();

private:
  EventHandler() {}
  EventHandler(EventHandler const &);
  void operator=(EventHandler const &);
};