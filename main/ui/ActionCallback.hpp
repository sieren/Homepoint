#pragma once

#include <functional>

namespace gfx
{
  using ActionCallback = std::function<void()>;

  enum class MenuActionState
  {
    NONE = 0,
    SETTINGSMENU = 1,
    MAINSCREEN = 2
  };

  struct MenuAction
  {
    MenuActionState mAction;
    ActionCallback mCallback;
  };
} // namespace gfx::ui
