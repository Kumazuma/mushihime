#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 960
#define FPS 60
#define PI 3.141592f
#define DELTA_TIME 0.01f

template<typename EnumT>
constexpr int ENUM_MAX()
{
	return (int)EnumT::END;
}

enum class Direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	LEFT_UP,
	LEFT_DOWN,
	RIGHT_UP,
	RIGHT_DOWN,
	END
};