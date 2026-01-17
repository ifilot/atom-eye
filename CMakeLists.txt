cmake_minimum_required(VERSION 3.21)

project(atomeye LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(atomeye
    src/main.cpp
    src/shader.cpp
    src/mesh_generators.cpp
    src/mesh.cpp
    src/molecule.cpp
    src/mesh_library.cpp
    src/background.cpp
)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(atomeye
    PRIVATE
        glfw
        OpenGL::GL
        GLEW::GLEW
)

if (WIN32)
    add_custom_command(TARGET atomeye POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:glfw>
            $<TARGET_FILE_DIR:atomeye>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:GLEW::GLEW>
            $<TARGET_FILE_DIR:atomeye>
    )
endif()
