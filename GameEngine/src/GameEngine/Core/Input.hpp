#pragma once

#include "GameEngine/Core/Core.hpp"
#include "GameEngine/Core/KeyCodes.hpp"
#include "GameEngine/Core/MouseCodes.hpp"

namespace GameEngine
{

class Input
{
public:
  static bool IsKeyPressed(KeyCode keycode);

  static bool IsMouseButtonPressed(MouseCode button);
  static std::pair<float, float> GetMousePosition();
  static float GetMouseX();
  static float GetMouseY();
};

} // namespace GameEngine
