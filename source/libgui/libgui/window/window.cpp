#include "libgui/internals/contexts/backend_context.hpp"
#include "libgui/internals/contexts/context_factory.hpp"
#include "libgui/utilities/string.hpp"
#include "libgui/window/window.hpp"

using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

window::window() : window_base() {}

window::~window() {
    teardown();
}

libgui::result window::initialize(const window_settings& settings, const window_startup_settings& startup_settings) {
    libgui::result result, res;

    m_backend_context = internals::context_factory::create_backend_context(this);
    if (!m_backend_context) {
        result.message = "Failed to create backend context.";
        return result;
    }

    res = m_backend_context->initialize(settings, startup_settings);
    if (!res) {
        result.message = LIBGUI_FORMAT("Failed to initialize backend context. | {}", res.message);
        return result;
    }

    result.success = true;
    return result;
}

void window::teardown() {
    if (m_backend_context) {
        m_backend_context->teardown();
        m_backend_context.release();
    }
}

void window::prepare_for_drawing() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->prepare_for_drawing();
}

void window::draw() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->draw();
}
