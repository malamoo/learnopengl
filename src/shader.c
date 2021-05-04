#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <glfw/glfw.h>

/* Returns the text of a specified file. */
char *readfile(const char *filename)
{
	FILE *fp;
	int numbytes;
	char *str;

	fp = fopen(filename, "r");
	fseek(fp, 0, SEEK_END);
	numbytes = ftell(fp);
	rewind(fp);

	str = calloc(numbytes + 1, sizeof(char));
	str[numbytes] = '\0';

	fread(str, sizeof(char), numbytes, fp);
	fclose(fp);

	return str;
}

/*
 * Makes an OpenGL shader program with the specified vertex and fragment
 * shaders.
 */
unsigned int makeshader(const char *vsfilename, const char *fsfilename)
{
	unsigned int vshader;
	unsigned int fshader;
	unsigned int program;
	char *src;
	int success;
	char infolog[512];

	src = readfile(vsfilename);
	vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, (const char **) &src, NULL);
	glCompileShader(vshader);
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vshader, 512, NULL, infolog);
		printf("Error: vertex shader compilation failed\n");
		printf("%s\n", infolog);
	}
	free(src);

	src = readfile(fsfilename);
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, (const char **) &src, NULL);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fshader, 512, NULL, infolog);
		printf("Error: vertex shader compilation failed\n");
		printf("%s\n", infolog);
	}
	free(src);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(program, 512, NULL, infolog);
		printf("Error: shader program linking failed\n");
		printf("%s\n", infolog);
	}

	glDeleteShader(vshader);
	glDeleteShader(fshader);

	return program;
}
