#include <stdlib.h>
#include <stdio.h>
#include "../external/glad/glad.h"
#include "../external/cglm/cglm.h"
#include "../include/shader.h"

typedef enum ShaderType {
    VERTEX,
    FRAGMENT,
    PROGRAM
} ShaderType;

long file_size(FILE *fp);
char *read_file(const char *name);
void check_errors(unsigned int shader, ShaderType type);

/* Initializes a shader program by linking the specified shaders. */
void shader_init(Shader *shader, const char *vertex_path,
                 const char *fragment_path)
{
        char *vertex_code;
        char *fragment_code;
        unsigned int vertex;
        unsigned int fragment;

        vertex_code = read_file(vertex_path);
        fragment_code = read_file(fragment_path);
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, (const char **)&vertex_code, NULL);
        glCompileShader(vertex);
        check_errors(vertex, VERTEX);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, (const char **)&fragment_code, NULL);
        glCompileShader(fragment);
        check_errors(fragment, FRAGMENT);
        shader->id = glCreateProgram();
        glAttachShader(shader->id, vertex);
        glAttachShader(shader->id, fragment);
        glLinkProgram(shader->id);
        check_errors(shader->id, PROGRAM);
        free(vertex_code);
        free(fragment_code);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
}

/* Activates the shader. */
void shader_use(Shader *shader)
{
        glUseProgram(shader->id);
}

/* Assigns a value to the named uniform in the shader program. */
void shader_assign_int(Shader *shader, const char *name, int value)
{
        glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_assign_float(Shader *shader, const char *name, float value)
{
        glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void shader_assign_vec2(Shader *shader, const char *name, vec2 value)
{
        glUniform2fv(glGetUniformLocation(shader->id, name), 1, value);
}

void shader_assign_vec3(Shader *shader, const char *name, vec3 value)
{
        glUniform3fv(glGetUniformLocation(shader->id, name), 1, value);
}

void shader_assign_vec4(Shader *shader, const char *name, vec4 value)
{
        glUniform4fv(glGetUniformLocation(shader->id, name), 1, value);
}

void shader_assign_mat2(Shader *shader, const char *name, mat2 value)
{
        glUniformMatrix2fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE,
                           (float *)value);
}

void shader_assign_mat3(Shader *shader, const char *name, mat3 value)
{
        glUniformMatrix3fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE,
                           (float *)value);
}

void shader_assign_mat4(Shader *shader, const char *name, mat4 value)
{
        glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE,
                           (float *)value);
}

/* Reads an entire file as a string. */
char *read_file(const char *name)
{
        FILE *fp;
        long size;
        char *str;

        fp = fopen(name, "r");
        size = file_size(fp);
        str = calloc(size + 1, sizeof(char));
        str[size] = '\0';
        fread(str, sizeof(char), size, fp);
        fclose(fp);
        return str;
}

/* Returns the number of bytes in a file. */
long file_size(FILE *fp)
{
        long size;

        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        rewind(fp);
        return size;
}

/*
 * Prints error messages if the shaders have compilation
 * or linking errors.
 */
void check_errors(unsigned int shader, ShaderType type)
{
        int success;
        char info_log[1024];
        char *shader_name;

        if (type != PROGRAM) {
                if (type == VERTEX)
                        shader_name = "vertex";
                else
                        shader_name = "fragment";
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                        glGetShaderInfoLog(shader, 1024, NULL, info_log);
                        printf("Error: Compilation of %s shader failed\n",
                               shader_name);
                        printf("%s\n", info_log);
                }
        } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                        glGetShaderInfoLog(shader, 1024, NULL, info_log);
                        printf("Error: Linking of shader program failed\n");
                        printf("%s\n", info_log);
                }
        }
}
