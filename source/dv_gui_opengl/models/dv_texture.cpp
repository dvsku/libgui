#include "dv_gui_opengl/dv_gui_opengl/models/dv_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#ifdef DV_USE_OPENGL2
    #include <windows.h>
    #include <gl/gl.h>
#elif DV_USE_OPENGL3
    #include <glad/glad.h>
#endif

using namespace dvsku;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

static void create_texture(unsigned char* data, int width, int height, uint32_t& id);

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_texture::dv_texture(const std::string& filename) {
    unsigned char* image_data = stbi_load(filename.c_str(), &width, &height, &components, 4);
    if (!image_data) return;

    create_texture(image_data, (int)width, (int)height, id);
    stbi_image_free(image_data);
}

dv_texture::dv_texture(const char* ptr, size_t size) {
    unsigned char* image_data = stbi_load_from_memory((stbi_uc*)ptr, (int)size, &width, &height, &components, 4);
    if (!image_data) return;

    create_texture(image_data, (int)width, (int)height, id);
    stbi_image_free(image_data);
}

dv_texture::~dv_texture() {
    if (!id) return;

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &id);
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

void create_texture(unsigned char* data, int width, int height, uint32_t& id) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef DV_USE_OPENGL2
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif

#ifdef DV_USE_OPENGL3
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
