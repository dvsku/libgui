#pragma once

#include "libgui/internals/backend/backend.hpp"

namespace libgui::internals {
    class backend_glfw_opengl3 final : public backend {
    public:
        backend_glfw_opengl3()                            = default;
        backend_glfw_opengl3(const backend_glfw_opengl3&) = delete;
        backend_glfw_opengl3(backend_glfw_opengl3&&)      = delete;

        backend_glfw_opengl3& operator=(const backend_glfw_opengl3&) = delete;
        backend_glfw_opengl3& operator=(backend_glfw_opengl3&&)      = delete;

        ~backend_glfw_opengl3() = default;

    public:
        libgui::result initialize() override final;
        void           teardown()   override final;
    };
}
