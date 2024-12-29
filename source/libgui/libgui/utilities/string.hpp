#pragma once

#include <format>

#define LIBGUI_FORMAT(fmt, ...)	\
		std::format(fmt, ##__VA_ARGS__)

#define LIBGUI_FORMAT_C(fmt, ...)	\
		std::format(fmt, ##__VA_ARGS__).c_str()
