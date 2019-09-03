#pragma once

#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

// Very simple Timer utility
// create thread, let it sleep for given time, then execute task.

class Timer
{
public:

  template <typename Time, class callable, class... arguments>
  static auto async (Time time, callable&& f, arguments&&... args)
  {
    std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f),
      std::forward<arguments>(args)...));

    std::thread([time, task]() {
          std::this_thread::sleep_for(time);
          task();
      }).detach();
  }
};
