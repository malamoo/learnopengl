#include <stdio.h>
#include <stdlib.h>
#include "../ext/glad.h"
#include "../ext/glfw.h"
#include "../include/shader.h"

long cntbytes(FILE *fp);
char *freadstr(char *name);

/* Returns the number of bytes in a file. */
long cntbytes(FILE *fp)
{
        long cnt;

        fseek(fp, 0, SEEK_END);
        cnt = ftell(fp);
        rewind(fp);
        return cnt;
}

/* Returns a file's characters as a string. */
char *freadstr(char *name)
{
        FILE *fp;
        long cnt;
        char *str;

        fp = fopen(name, "r");
        cnt = cntbytes(fp);
        str = calloc(cnt + 1, sizeof(char));
        str[cnt] = '\0';
        fread(str, sizeof(char), cnt, fp);
        fclose(fp);
        return str;
}

/*
 * Makes an OpenGL shader program with the specified vertex and fragment
 * shaders.
 */
unsigned int makeshader(char *vsname, char *fsname)
{
        unsigned int vshader;
        unsigned int fshader;
        unsigned int program;
        int success;
        char log[512];
        char *src;

        vshader = glCreateShader(GL_VERTEX_SHADER);
        src = freadstr(vsname);
        glShaderSource(vshader, 1, (const char **)&src, NULL);
        glCompileShader(vshader);
        glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
        if (!success) {
                glGetShaderInfoLog(vshader, 512, NULL, log);
                printf("Error: shader compilation failed\n");
                printf("%s\n", log);
        }
        free(src);
        fshader = glCreateShader(GL_FRAGMENT_SHADER);
        src = freadstr(fsname);
        glShaderSource(fshader, 1, (const char **)&src, NULL);
        glCompileShader(fshader);
        glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
        if (!success) {
                glGetShaderInfoLog(fshader, 512, NULL, log);
                printf("Error: shader compilation failed\n");
                printf("%s\n", log);
        }
        free(src);
        program = glCreateProgram();
        glAttachShader(program, vshader);
        glAttachShader(program, fshader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
                glGetShaderInfoLog(program, 512, NULL, log);
                printf("Error: shader program linking failed\n");
                printf("%s\n", log);
        }
        glDeleteShader(vshader);
        glDeleteShader(fshader);
        return program;
}
