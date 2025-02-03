#pragma once

#include "libgui/internals/backend/backend.hpp"

namespace libgui::internals {
    class backend_glfw_opengl2 final : public backend {
    public:
        backend_glfw_opengl2()                            = default;
        backend_glfw_opengl2(const backend_glfw_opengl2&) = delete;
        backend_glfw_opengl2(backend_glfw_opengl2&&)      = delete;

        backend_glfw_opengl2& operator=(const backend_glfw_opengl2&) = delete;
        backend_glfw_opengl2& operator=(backend_glfw_opengl2&&)      = delete;

        ~backend_glfw_opengl2() = default;

    public:
        libgui::result initialize() override final;
        void           teardown()   override final;
    };
}
