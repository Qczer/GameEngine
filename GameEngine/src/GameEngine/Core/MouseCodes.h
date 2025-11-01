#pragma once

#include <format>
#include <string>
#include <cstdint>

namespace GameEngine
{
	enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};
	using Mouse = MouseCode;

	inline std::string format_as(const MouseCode& mouseCode) {
		return std::to_string(static_cast<int32_t>(mouseCode));
	}

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		return os << static_cast<int32_t>(mouseCode);
	}
}

template<>
struct std::formatter<GameEngine::MouseCode, char> {
	constexpr auto parse(std::format_parse_context& ctx) {
		return ctx.begin();
	}

	auto format(const GameEngine::MouseCode& mc, std::format_context& ctx) const {
		std::formatter<int> int_formatter;
		return int_formatter.format(static_cast<int32_t>(mc), ctx);
	}
};

// From glfw3.h
#define GE_MOUSE_BUTTON_0         ::GameEngine::Mouse::Button0
#define GE_MOUSE_BUTTON_1         ::GameEngine::Mouse::Button1
#define GE_MOUSE_BUTTON_2         ::GameEngine::Mouse::Button2
#define GE_MOUSE_BUTTON_3         ::GameEngine::Mouse::Button3
#define GE_MOUSE_BUTTON_4         ::GameEngine::Mouse::Button4
#define GE_MOUSE_BUTTON_5         ::GameEngine::Mouse::Button5
#define GE_MOUSE_BUTTON_6         ::GameEngine::Mouse::Button6
#define GE_MOUSE_BUTTON_7         ::GameEngine::Mouse::Button7
#define GE_MOUSE_BUTTON_LAST      ::GameEngine::Mouse::ButtonLast
#define GE_MOUSE_BUTTON_LEFT      ::GameEngine::Mouse::ButtonLeft
#define GE_MOUSE_BUTTON_RIGHT     ::GameEngine::Mouse::ButtonRight
#define GE_MOUSE_BUTTON_MIDDLE    ::GameEngine::Mouse::ButtonMiddle