#include <stdlib.h>
#include <stdio.h>
#include "../external/glad.h"
#include "../external/gb_math.h"
#include "../include/shader.h"

typedef enum ShaderType {
    VERTEX,
    FRAGMENT,
    PROGRAM
} ShaderType;

long count_bytes(FILE *fp);
char *fread_str(const char *name);
void check_success(unsigned int shader, ShaderType type);

/* Initializes a shader program by linking the specified shaders. */
void shader_init(Shader *shader, const char *vertex_path,
                 const char *fragment_path)
{
        char *vertex_code;
        char *fragment_code;
        unsigned int vertex;
        unsigned int fragment;

        vertex_code = fread_str(vertex_path);
        fragment_code = fread_str(fragment_path);
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, (const char **)&vertex_code, NULL);
        glCompileShader(vertex);
        check_success(vertex, VERTEX);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, (const char **)&fragment_code, NULL);
        glCompileShader(fragment);
        check_success(fragment, FRAGMENT);
        shader->id = glCreateProgram();
        glAttachShader(shader->id, vertex);
        glAttachShader(shader->id, fragment);
        glLinkProgram(shader->id);
        check_success(shader->id, PROGRAM);
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

/* Sets the named uniform to the specified value in the shader program. */
void shader_load_int(Shader *shader, const char *name, int value)
{
        glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_load_float(Shader *shader, const char *name, float value)
{
        glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void shader_load_vec2(Shader *shader, const char *name, gbVec2 *value)
{
        glUniform2fv(glGetUniformLocation(shader->id, name), 1, value->e);
}

void shader_load_vec3(Shader *shader, const char *name, gbVec3 *value)
{
        glUniform3fv(glGetUniformLocation(shader->id, name), 1, value->e);
}

void shader_load_vec4(Shader *shader, const char *name, gbVec4 *value)
{
        glUniform4fv(glGetUniformLocation(shader->id, name), 1, value->e);
}

void shader_load_mat2(Shader *shader, const char *name, gbMat2 *value)
{
        glUniformMatrix2fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE,
                           value->e);
}

void shader_load_mat3(Shader *shader, const char *name, gbMat3 *value)
{
        glUniformMatrix3fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE,
                           value->e);
}

void shader_load_mat4(Shader *shader, const char *name, gbMat4 *value)
{
        glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE,
                           value->e);
}

/* Reads an entire file and returns the characters as a string. */
char *fread_str(const char *name)
{
        FILE *fp;
        long count;
        char *str;

        fp = fopen(name, "r");
        count = count_bytes(fp);
        str = calloc(count + 1, sizeof(char));
        str[count] = '\0';
        fread(str, sizeof(char), count, fp);
        fclose(fp);
        return str;
}

/* Returns the number of bytes in a file. */
long count_bytes(FILE *fp)
{
        long count;

        fseek(fp, 0, SEEK_END);
        count = ftell(fp);
        rewind(fp);
        return count;
}

/*
 * Prints error messages if the shaders have compilation
 * or linking errors.
 */
void check_success(unsigned int shader, ShaderType type)
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
