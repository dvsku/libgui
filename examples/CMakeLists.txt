﻿IF(LIBGUI_BUILD_OPENGL2)
    ADD_EXECUTABLE(example_simple_opengl2
        "example_simple.cpp"
    )

    TARGET_LINK_LIBRARIES(example_simple_opengl2 PRIVATE libgui_opengl2)

    ADD_EXECUTABLE(example_borderless_opengl2
        "example_borderless.cpp"
    )

    TARGET_LINK_LIBRARIES(example_borderless_opengl2 PRIVATE libgui_opengl2)
ENDIF()

IF(LIBGUI_BUILD_OPENGL3)
    ADD_EXECUTABLE(example_simple_opengl3
        "example_simple.cpp"
    )

    TARGET_LINK_LIBRARIES(example_simple_opengl3 PRIVATE libgui_opengl3)

    ADD_EXECUTABLE(example_borderless_opengl3
        "example_borderless.cpp"
    )

    TARGET_LINK_LIBRARIES(example_borderless_opengl3 PRIVATE libgui_opengl3)
ENDIF()
