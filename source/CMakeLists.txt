FILE(GLOB_RECURSE LIBGUI_SOURCES_GLFW CONFIGURE_DEPENDS 
    "glfw/*.c" "glfw/*.cpp"
)

FILE(GLOB_RECURSE LIBGUI_SOURCES_GLAD CONFIGURE_DEPENDS 
    "glad/*.c" "glad/*.cpp"
)

SET(LIBGUI_SOURCES_IMGUI_OPENGL2
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/misc/cpp/imgui_stdlib.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
    "imgui/backends/imgui_impl_opengl2.cpp"
)

SET(LIBGUI_SOURCES_IMGUI_OPENGL3
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/misc/cpp/imgui_stdlib.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
)

FILE(GLOB_RECURSE LIBGUI_SOURCES CONFIGURE_DEPENDS 
    "dv_gui_opengl/*.c" "dv_gui_opengl/*.cpp"
)

FILE(GLOB_RECURSE LIBGUI_HEADERS CONFIGURE_DEPENDS 
    "${LIBGUI_INCLUDES}/*.h" "${LIBGUI_INCLUDES}/*.hpp"
)

IF(LIBGUI_BUILD_OPENGL2)
    ADD_LIBRARY(libgui_opengl2 STATIC
        ${LIBGUI_SOURCES_GLFW}
        ${LIBGUI_SOURCES_IMGUI_OPENGL2}
        ${LIBGUI_SOURCES}
        ${LIBGUI_HEADERS}
    )

    TARGET_INCLUDE_DIRECTORIES(libgui_opengl2 PUBLIC  "${LIBGUI_INCLUDES}")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl2 PUBLIC  "${LIBGUI_INCLUDES}/dv_gui_opengl")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl2 PRIVATE "${LIBGUI_INCLUDES}/imgui")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl2 PRIVATE "${LIBGUI_INCLUDES}/imgui/backends")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl2 PRIVATE "${LIBGUI_INCLUDES}/imgui/misc/cpp")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

    IF(MSVC)
        TARGET_LINK_LIBRARIES(libgui_opengl2 PUBLIC libutil)
        TARGET_LINK_LIBRARIES(libgui_opengl2 PUBLIC opengl32)
        TARGET_LINK_LIBRARIES(libgui_opengl2 PUBLIC "winmm.lib")

        TARGET_COMPILE_DEFINITIONS(libgui_opengl2 PUBLIC  -D_GLFW_WIN32)
        TARGET_COMPILE_DEFINITIONS(libgui_opengl2 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
        TARGET_COMPILE_DEFINITIONS(libgui_opengl2 PUBLIC  -DDV_USE_OPENGL2)
    ENDIF()
ENDIF()

IF(LIBGUI_BUILD_OPENGL3)
    ADD_LIBRARY(libgui_opengl3 STATIC
        ${LIBGUI_SOURCES_GLFW}
        ${LIBGUI_SOURCES_GLAD}
        ${LIBGUI_SOURCES_IMGUI_OPENGL3}
        ${LIBGUI_SOURCES}
        ${LIBGUI_HEADERS}
    )

    TARGET_INCLUDE_DIRECTORIES(libgui_opengl3 PUBLIC  "${LIBGUI_INCLUDES}")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl3 PUBLIC  "${LIBGUI_INCLUDES}/dv_gui_opengl")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl3 PRIVATE "${LIBGUI_INCLUDES}/imgui")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl3 PRIVATE "${LIBGUI_INCLUDES}/imgui/backends")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl3 PRIVATE "${LIBGUI_INCLUDES}/imgui/misc/cpp")
    TARGET_INCLUDE_DIRECTORIES(libgui_opengl3 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

    IF(MSVC)
        TARGET_LINK_LIBRARIES(libgui_opengl3 PUBLIC libutil)
        TARGET_LINK_LIBRARIES(libgui_opengl3 PUBLIC "winmm.lib")

        TARGET_COMPILE_DEFINITIONS(libgui_opengl3 PUBLIC  -D_GLFW_WIN32)
        TARGET_COMPILE_DEFINITIONS(libgui_opengl3 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
        TARGET_COMPILE_DEFINITIONS(libgui_opengl3 PUBLIC  -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
        TARGET_COMPILE_DEFINITIONS(libgui_opengl3 PUBLIC  -DGLAD_GLAPI_EXPORT)
        TARGET_COMPILE_DEFINITIONS(libgui_opengl3 PUBLIC  -DGLAD_GLAPI_EXPORT_BUILD)
        TARGET_COMPILE_DEFINITIONS(libgui_opengl3 PUBLIC  -DDV_USE_OPENGL3)
    ENDIF()
ENDIF()
