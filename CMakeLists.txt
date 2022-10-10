cmake_minimum_required(VERSION 3.0.0)
project(objLoader VERSION 0.1.0)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_compile_definitions(GLEW_STATIC)

# all imgui .cpp files
file( GLOB IMGUI_SRC src/imgui/*.cpp )

add_executable(${PROJECT_NAME} src/main.cpp ${IMGUI_SRC})
target_include_directories( ${PROJECT_NAME} PUBLIC include/ include/imgui )
target_link_directories( ${PROJECT_NAME} PRIVATE lib/ )
target_link_libraries(${PROJECT_NAME} glew32s glfw3 opengl32 )

add_executable(ImGUI_demo src/demoImGUI.cpp ${IMGUI_SRC})
target_include_directories( ImGUI_demo PUBLIC include/ include/imgui/)
target_link_directories( ImGUI_demo PRIVATE lib/ )
target_link_libraries(ImGUI_demo glew32s glfw3 opengl32)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
