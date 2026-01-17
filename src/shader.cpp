#include "shader.h"

#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

/**
 * @brief Construct a shader program from source files.
 *
 * Performs the following steps:
 *  1. Load shader source code from disk
 *  2. Compile vertex and fragment shaders
 *  3. Link them into an OpenGL program
 *  4. Validate the resulting program
 */
Shader::Shader(const std::string& vertex_path,
               const std::string& fragment_path)
{
    // Load shader source code
    const std::string vertex_source   = load_file(vertex_path);
    const std::string fragment_source = load_file(fragment_path);

    // Compile shaders
    const unsigned int vs = compile(GL_VERTEX_SHADER, vertex_source);
    const unsigned int fs = compile(GL_FRAGMENT_SHADER, fragment_source);

    // Create and link program
    program_ = glCreateProgram();
    glAttachShader(program_, vs);
    glAttachShader(program_, fs);
    glLinkProgram(program_);

    // Check link status
    int success = 0;
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetProgramInfoLog(program_, sizeof(log), nullptr, log);
        glDeleteProgram(program_);
        throw std::runtime_error(
            std::string("shader program linking failed: ") + log
        );
    }

    // Shaders are no longer needed after linking
    glDeleteShader(vs);
    glDeleteShader(fs);
}

/**
 * @brief Destroy the shader program.
 */
Shader::~Shader() {
    if (program_ != 0) {
        glDeleteProgram(program_);
    }
}

/**
 * @brief Move constructor.
 */
Shader::Shader(Shader&& other) noexcept {
    program_ = other.program_;
    other.program_ = 0;
}

/**
 * @brief Move assignment operator.
 */
Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (program_ != 0) {
            glDeleteProgram(program_);
        }
        program_ = other.program_;
        other.program_ = 0;
    }
    return *this;
}

/**
 * @brief Bind the shader program.
 */
void Shader::bind() const {
    glUseProgram(program_);
}

/**
 * @brief Unbind any active shader program.
 */
void Shader::unbind() {
    glUseProgram(0);
}

/**
 * @brief Set an integer uniform.
 */
void Shader::set_int(const std::string& name, int value) {
    glUniform1i(get_uniform_location(name), value);
}

/**
 * @brief Set a floating-point uniform.
 */
void Shader::set_float(const std::string& name, float value) {
    glUniform1f(get_uniform_location(name), value);
}

/**
 * @brief Set a vec3 uniform.
 */
void Shader::set_vec3(const std::string& name, const float* value) {
    glUniform3fv(get_uniform_location(name), 1, value);
}

/**
 * @brief Set a vec2 uniform.
 */
void Shader::set_vec2(const std::string& name, const float* value) {
    glUniform2fv(get_uniform_location(name), 1, value);
}

/**
 * @brief Set a mat4 uniform.
 */
void Shader::set_mat4(const std::string& name, const float* value) {
    glUniformMatrix4fv(
        get_uniform_location(name), 1, GL_FALSE, value
    );
}

/**
 * @brief Retrieve (and cache) a uniform location.
 */
int Shader::get_uniform_location(const std::string& name) const {
    auto it = uniform_cache_.find(name);
    if (it != uniform_cache_.end()) {
        return it->second;
    }

    int location = glGetUniformLocation(program_, name.c_str());
    uniform_cache_[name] = location;
    return location;
}

/**
 * @brief Compile a GLSL shader.
 */
unsigned int Shader::compile(unsigned int type,
                             const std::string& source)
{
    const unsigned int sh = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(sh, 1, &src, nullptr);
    glCompileShader(sh);

    int success = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(sh, sizeof(log), nullptr, log);
        glDeleteShader(sh);
        throw std::runtime_error(
            std::string("shader compilation failed: ") + log
        );
    }

    return sh;
}

/**
 * @brief Load an entire text file into a string.
 */
std::string Shader::load_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("failed to open shader file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
