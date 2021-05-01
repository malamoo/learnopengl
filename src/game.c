#include <stdio.h>
#include <math.h>
#include <glad/glad.h>
#include <glfw/glfw.h>

const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 a_pos;\n"
    "\n"
    "void\n"
    "main()\n"
    "{\n"
    "        gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
    "}\0";

const char *fragment_shader_source =
    "#version 330 core\n"
    "out vec4 frag_color;\n"
    "\n"
    "uniform vec4 color;\n"
    "\n"
    "void\n"
    "main()\n"
    "{\n"
    "        frag_color = color;\n"
    "}\0";

/*
 * Callback registered with GLFW to resize the OpenGL viewport
 * when the window's dimensions change.
 */
void resize_viewport(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*
 * Process input key presses to the window.
 */
void process_input(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

int main(int argc, char **argv)
{
	GLFWwindow *window;
	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	unsigned int shader_program;
	int success;
	char info_log[512];
        float secs;
        float green_val;
        int color_loc;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	window = glfwCreateWindow(800, 600, "Game", NULL, NULL);

	if (window == NULL) {
		printf("Error: failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, resize_viewport);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("Error: vertex shader compilation failed\n");
		printf("%s\n", info_log);
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		printf("Error: fragment shader compilation failed\n");
		printf("%s\n", info_log);
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader_program, 512, NULL, info_log);
		printf("Error: shader program linking failed\n");
		printf("%s\n", info_log);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			      (void *) 0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

                secs = glfwGetTime();
                green_val = (sin(secs) / 2.0f) + 0.5f;
                color_loc = glGetUniformLocation(shader_program, "color");
		glUseProgram(shader_program);
                glUniform4f(color_loc, 0.0f, green_val, 0.0f, 1.0f);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteProgram(shader_program);

	glfwTerminate();

	return 0;
}
