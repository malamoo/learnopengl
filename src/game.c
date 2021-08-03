#include <stdio.h>
#include "../external/glad.h"
#include "../external/glfw.h"
#include "../external/stb_image.h"
#include "../external/gb_math.h"
#include "../include/shader.h"
#include "../include/texture.h"

void resize(GLFWwindow *window, int width, int height);
void procinput(GLFWwindow *window);

/* Resizes the OpenGL viewport when the window dimensions change. */
void resize(GLFWwindow *window, int width, int height)
{
        glViewport(0, 0, width, height);
}

/* Processes the window input. */
void procinput(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, 1);
}

int main(void)
{
        GLFWwindow *window;
        float vertices[] = {
                /* positions            texture coords */
               -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,     1.0f, 1.0f,

               -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
               -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
               -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,     1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
               -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,
               -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,

               -0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
               -0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
               -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
               -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,

               -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
               -0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,     1.0f, 1.0f,

                0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,     1.0f, 0.0f,

               -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,     1.0f, 0.0f,

               -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
               -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
               -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,     1.0f, 1.0f,

                0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
               -0.5f,  0.5f,  0.5f,     0.0f, 0.0f,
               -0.5f,  0.5f, -0.5f,     0.0f, 1.0f
        };
        gbVec3 cubepos[] = {
                gb_vec3( 0.0f, 0.0f, 0.0f),
                gb_vec3( 2.0f, 5.0f, -15.0f),
                gb_vec3(-1.5f, -2.2f, -2.5f),
                gb_vec3(-3.8f, -2.0f, -12.3f),
                gb_vec3( 2.4f, -0.4f, -3.5f),
                gb_vec3(-1.7f, 3.0f, -7.5f),
                gb_vec3( 1.3f, -2.0f, -2.5f),
                gb_vec3( 1.5f, 2.0f, -2.5f),
                gb_vec3( 1.5f, 0.2f, -1.5f),
                gb_vec3(-1.3f, 1.0f, -1.5f)
        };
        int i;
        unsigned int vbo;
        unsigned int vao;
        unsigned int shader;
        unsigned int tex1;
        unsigned int tex2;
        gbMat4 model;
        gbMat4 modrot;
        gbMat4 modtrans;
        gbMat4 view;
        gbMat4 project;
        float angle;

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
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                printf("Failed to initialize GLAD\n");
                return -1;
        }
        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window, resize);
        shader = makeshader("src/vshader.glsl", "src/fshader.glsl");
        tex1 = maketexture("assets/container.jpeg");
        tex2 = maketexture("assets/awesomeface.png");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(shader);
        glUniform1i(glGetUniformLocation(shader, "tex1"), 0);
        glUniform1i(glGetUniformLocation(shader, "tex2"), 1);
        while (!glfwWindowShouldClose(window)) {
                gb_mat4_translate(&view, gb_vec3(0.0f, 0.0f, -3.0f));
                gb_mat4_perspective(&project, gb_to_radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
                glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view.e);
                glUniformMatrix4fv(glGetUniformLocation(shader, "project"), 1, GL_FALSE, project.e);
                procinput(window);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, tex2);
                glBindVertexArray(vao);
                for (i = 0; i < 10; i++) {
                        angle = 20.0f * (float)i;
                        gb_mat4_rotate(&modrot, gb_vec3(1.0f, 0.3f, 0.5f), gb_to_radians(angle));
                        gb_mat4_translate(&modtrans, cubepos[i]);
                        gb_mat4_mul(&model, &modtrans, &modrot);
                        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model.e);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                glfwSwapBuffers(window);
                glfwPollEvents();
        }
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteProgram(shader);
        glfwTerminate();
        return 0;
}
