cmake_minimum_required(VERSION 3.21)

project(atomeye LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(glfw_app
    src/main.cpp
)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(glfw_app
    PRIVATE
        glfw
        OpenGL::GL
)

if (WIN32)
    add_custom_command(TARGET glfw_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:glfw>
            $<TARGET_FILE_DIR:glfw_app>
    )
endif()