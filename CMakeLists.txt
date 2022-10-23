cmake_minimum_required(VERSION 3.0.0)
project(objLoader VERSION 0.1.0)

# generate less cmake bullshit
set(CMAKE_SUPPRESS_REGENERATION true)
# set main project as the starting project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# define constant at compilation time (glew static linking)
add_compile_definitions(GLEW_STATIC)

# all imgui .cpp files
file( GLOB IMGUI_SRC src/imgui/*.cpp )

# Main Project
    # add all cpp files
add_executable(${PROJECT_NAME} src/main.cpp ${IMGUI_SRC} src/tinyfiledialogs/tinyfiledialogs.c)
    # target all .h and .hpp files
target_include_directories( ${PROJECT_NAME} PUBLIC include/ include/imgui include/tinyfiledialogs)
    # target all .lib filesfor linking
target_link_directories( ${PROJECT_NAME} PRIVATE lib/ )
    # specify libraries names for linking
target_link_libraries(${PROJECT_NAME} glew32s glfw3 opengl32 )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/shaders/
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/ )

# ImGui Demo
add_executable(ImGUI_demo src/ImGuiDemo.cpp ${IMGUI_SRC})
target_include_directories( ImGUI_demo PUBLIC include/ include/imgui/)
target_link_directories( ImGUI_demo PRIVATE lib/ )
target_link_libraries(ImGUI_demo glew32s glfw3 opengl32)

# tinyfiledialogs Demo
add_executable(tinyfiledialogs_demo src/tinyfiledialogsDemo.c src/tinyfiledialogs/tinyfiledialogs.c)
target_include_directories(tinyfiledialogs_demo PUBLIC include/tinyfiledialogs)
target_link_libraries(tinyfiledialogs_demo comdlg32 ole32)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
