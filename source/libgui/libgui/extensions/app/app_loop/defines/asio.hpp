#pragma once

#define ASIO_STANDALONE

#ifndef _WIN32_WINNT
    #define LIBGUI_ASIO_WIN_DEFINED
    #define _WIN32_WINNT 0x0601
#endif 

#include <asio/asio.hpp>
#include <asio/asio/experimental/awaitable_operators.hpp>

#ifdef LIBGUI_ASIO_WIN_DEFINED
    #undef LIBGUI_ASIO_WIN_DEFINED
    #undef _WIN32_WINNT
#endif
