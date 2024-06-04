FILE(GLOB_RECURSE DV_GUI_SOURCES_GLFW CONFIGURE_DEPENDS 
    "glfw/*.c" "glfw/*.cpp"
)

FILE(GLOB_RECURSE DV_GUI_SOURCES_GLAD CONFIGURE_DEPENDS 
    "glad/*.c" "glad/*.cpp"
)

SET(DV_GUI_SOURCES_IMGUI_OPENGL2
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/misc/cpp/imgui_stdlib.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
    "imgui/backends/imgui_impl_opengl2.cpp"
)

SET(DV_GUI_SOURCES_IMGUI_OPENGL3
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/misc/cpp/imgui_stdlib.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
)

FILE(GLOB_RECURSE DV_GUI_SOURCES CONFIGURE_DEPENDS 
    "dv_gui_opengl/*.c" "dv_gui_opengl/*.cpp"
)

FILE(GLOB_RECURSE DV_GUI_HEADERS CONFIGURE_DEPENDS 
    "${DV_GUI_INCLUDES}/*.h" "${DV_GUI_INCLUDES}/*.hpp"
)

IF(DV_GUI_BUILD_OPENGL2)
    ADD_LIBRARY(dv_gui_opengl2 STATIC
        ${DV_GUI_SOURCES_GLFW}
        ${DV_GUI_SOURCES_IMGUI_OPENGL2}
        ${DV_GUI_SOURCES}
        ${DV_GUI_HEADERS}
    )

    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl2 PRIVATE "${DV_GUI_INCLUDES}")
    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl2 PRIVATE "${DV_GUI_INCLUDES}/imgui")
    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl2 PRIVATE "${DV_GUI_INCLUDES}/imgui/backends")
    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl2 PRIVATE "${DV_GUI_INCLUDES}/imgui/misc/cpp")
    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

    IF(MSVC)
        TARGET_LINK_LIBRARIES(dv_gui_opengl2 PUBLIC libutil)
        TARGET_LINK_LIBRARIES(dv_gui_opengl2 PUBLIC opengl32)
        TARGET_LINK_LIBRARIES(dv_gui_opengl2 PUBLIC "winmm.lib")

        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl2 PUBLIC  -D_GLFW_WIN32)
        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl2 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl2 PUBLIC  -DDV_USE_OPENGL2)
    ENDIF()
ENDIF()

IF(DV_GUI_BUILD_OPENGL3)
    ADD_LIBRARY(dv_gui_opengl3 STATIC
        ${DV_GUI_SOURCES_GLFW}
        ${DV_GUI_SOURCES_GLAD}
        ${DV_GUI_SOURCES_IMGUI_OPENGL3}
        ${DV_GUI_SOURCES}
        ${DV_GUI_HEADERS}
    )

    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl3 PRIVATE "${DV_GUI_INCLUDES}")
    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl3 PRIVATE "${DV_GUI_INCLUDES}/imgui")
    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl3 PRIVATE "${DV_GUI_INCLUDES}/imgui/backends")
    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl3 PRIVATE "${DV_GUI_INCLUDES}/imgui/misc/cpp")
    TARGET_INCLUDE_DIRECTORIES(dv_gui_opengl3 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

    IF(MSVC)
        TARGET_LINK_LIBRARIES(dv_gui_opengl3 PUBLIC libutil)
        TARGET_LINK_LIBRARIES(dv_gui_opengl3 PUBLIC "winmm.lib")

        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl3 PUBLIC  -D_GLFW_WIN32)
        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl3 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl3 PUBLIC  -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl3 PUBLIC  -DGLAD_GLAPI_EXPORT)
        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl3 PUBLIC  -DGLAD_GLAPI_EXPORT_BUILD)
        TARGET_COMPILE_DEFINITIONS(dv_gui_opengl3 PUBLIC  -DDV_USE_OPENGL3)
    ENDIF()
ENDIF()
