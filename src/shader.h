#pragma once

#include <string>
#include <unordered_map>

/**
 * @class shader
 *
 * @brief RAII wrapper around an OpenGL shader program.
 *
 * This class encapsulates the complete lifecycle of a GLSL shader program:
 *  - loading shader source code from files
 *  - compiling vertex and fragment shaders
 *  - linking them into a program
 *  - managing the OpenGL program lifetime
 *
 * The shader class intentionally hides all OpenGL details from users.
 * Consumers interact with it through a minimal, type-safe interface.
 */
class shader {
public:
    /**
     * @brief Construct a shader program from source files.
     *
     * Loads the vertex and fragment shader source code from disk,
     * compiles them, links them into a shader program, and validates
     * the result.
     *
     * @param vertex_path   Path to the vertex shader file
     * @param fragment_path Path to the fragment shader file
     *
     * @throws std::runtime_error if loading, compilation, or linking fails
     */
    shader(const std::string& vertex_path,
           const std::string& fragment_path);

    /**
     * @brief Destroy the shader program.
     *
     * Releases the owned OpenGL program object.
     * This makes the shader class an RAII type.
     */
    ~shader();

    // Shader programs have unique ownership and cannot be copied
    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    /**
     * @brief Move constructor.
     *
     * Transfers ownership of the OpenGL program from another shader.
     */
    shader(shader&& other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * Releases the current program and takes ownership of another.
     */
    shader& operator=(shader&& other) noexcept;

    /**
     * @brief Bind this shader program for use.
     *
     * After calling this function, all subsequent draw calls will
     * use this shader program until another program is bound.
     */
    void bind() const;

    /**
     * @brief Unbind any active shader program.
     *
     * Resets the OpenGL state so that no shader program is bound.
     */
    static void unbind();

    /**
     * @brief Set an integer uniform.
     *
     * @param name  Name of the uniform in the shader
     * @param value Integer value to assign
     */
    void set_int(const std::string& name, int value);

    /**
     * @brief Set a floating-point uniform.
     *
     * @param name  Name of the uniform in the shader
     * @param value Float value to assign
     */
    void set_float(const std::string& name, float value);

    /**
     * @brief Set a vec3 uniform.
     *
     * @param name  Name of the uniform in the shader
     * @param value Pointer to three floats
     */
    void set_vec3(const std::string& name, const float* value);

    /**
     * @brief Set a mat4 uniform.
     *
     * @param name  Name of the uniform in the shader
     * @param value Pointer to 16 floats (column-major order)
     */
    void set_mat4(const std::string& name, const float* value);

private:
    /**
     * @brief OpenGL program handle.
     *
     * A value of 0 indicates that no program is currently owned.
     */
    unsigned int program_ = 0;

    /**
     * @brief Cache of uniform locations.
     *
     * Uniform locations are queried once and stored to avoid
     * repeated calls to glGetUniformLocation.
     */
    mutable std::unordered_map<std::string, int> uniform_cache_;

    /**
     * @brief Retrieve the location of a uniform.
     *
     * Uses a cached value if available, otherwise queries OpenGL
     * and stores the result.
     *
     * @param name Name of the uniform
     * @return OpenGL uniform location
     */
    int get_uniform_location(const std::string& name) const;

    /**
     * @brief Compile a GLSL shader of a given type.
     *
     * @param type   OpenGL shader type (e.g. GL_VERTEX_SHADER)
     * @param source GLSL source code
     *
     * @return OpenGL shader handle
     *
     * @throws std::runtime_error if compilation fails
     */
    static unsigned int compile(unsigned int type,
                                const std::string& source);

    /**
     * @brief Load a text file into a string.
     *
     * @param path Path to the file
     *
     * @return Entire file contents as a string
     *
     * @throws std::runtime_error if the file cannot be opened
     */
    static std::string load_file(const std::string& path);
};
