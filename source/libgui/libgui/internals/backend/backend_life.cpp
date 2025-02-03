#include "libgui/internals/backend/backend_life.hpp"
#include "libgui/internals/backend/backend_glfw_opengl2.hpp"
#include "libgui/internals/backend/backend_glfw_opengl3.hpp"
#include "libgui/internals/global.hpp"
#include "libgui/utilities/string.hpp"

using namespace libgui::internals;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL
///////////////////////////////////////////////////////////////////////////////

static std::unique_ptr<backend> create_backend() {
    std::unique_ptr<backend> retval = nullptr;

    switch (global::backend_type) {
        case LIBGUI_BACKEND_GLFW_OPENGL2: {
            retval = std::make_unique<backend_glfw_opengl2>();
            break;
        }
        case LIBGUI_BACKEND_GLFW_OPENGL3: {
            retval = std::make_unique<backend_glfw_opengl3>();
            break;
        }

        default: break;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC
///////////////////////////////////////////////////////////////////////////////

backend_life::~backend_life() {
    teardown();
}

libgui::result backend_life::initialize() {
    libgui::result result, res;

    if (m_backend) {
        result.success = true;
        return result;
    }

    try {
        m_backend = create_backend();
    }
    catch(const std::exception& e) {
        teardown();
        result.message = LIBGUI_FORMAT("Failed to create backend. | {}", e.what());
        return result;
    }

    if (!m_backend) {
        teardown();
        result.message = "Failed to create backend.";
        return result;
    }

    res = m_backend->initialize();
    if (!res) {
        teardown();
        result.message = LIBGUI_FORMAT("Failed to initialize backend. | {}", res.message);
        return result;
    }

    result.success = true;
    return result;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE
///////////////////////////////////////////////////////////////////////////////

void backend_life::teardown() {
    if (m_backend) {
        m_backend->teardown();
        m_backend.release();
    }
}
