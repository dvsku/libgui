#include "libgui/extensions/imgui/app/window/imgui_window.hpp"
#include "libgui/extensions/imgui/app/internals/contexts/imgui_context_factory.hpp"
#include "libgui/utilities/string.hpp"

using namespace libgui::ex::imgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC
///////////////////////////////////////////////////////////////////////////////

imgui_window::imgui_window()
    : window_base() {}

imgui_window::~imgui_window() {
    teardown();
}

libgui::result imgui_window::initialize(const window_settings& settings,
    const imgui_window_startup_settings& startup_settings)
{
    libgui::result result, res;

    m_backend_context = internals::imgui_context_factory::create_backend_context(this);
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

void imgui_window::teardown() {
    if (m_backend_context) {
        m_backend_context->teardown();
        m_backend_context.release();
    }
}

void imgui_window::prepare_for_drawing() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->prepare_for_drawing();
}

void imgui_window::draw() {
    if (!m_backend_context) {
        return;
    }

    m_backend_context->draw();
}
