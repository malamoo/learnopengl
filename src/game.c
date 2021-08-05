#include <stdio.h>
#include "../external/glad.h"
#include "../external/glfw.h"
#include "../external/stb_image.h"
#include "../external/gb_math.h"
#include "../include/shader.h"
#include "../include/texture.h"

gbVec3 eye;
gbVec3 front;
gbVec3 up;
float deltatime;
float lastframe;
float yaw;
float pitch;
float lastx;
float lasty;
float fov;
int firstmouse;

void resize(GLFWwindow *window, int width, int height);
void procinput(GLFWwindow *window);
void updatecam(GLFWwindow* window, double xpos, double ypos);

/* Resizes the OpenGL viewport when the window dimensions change. */
void resize(GLFWwindow *window, int width, int height)
{
        glViewport(0, 0, width, height);
}

/* Processes the window input. */
void procinput(GLFWwindow *window)
{
        float speed = 5.0f * deltatime;
        gbVec3 deltapos;
        gbVec3 right;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, 1);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                gb_vec3_mul(&deltapos, front, speed);
                gb_vec3_add(&eye, eye, deltapos);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                gb_vec3_mul(&deltapos, front, speed);
                gb_vec3_sub(&eye, eye, deltapos);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                gb_vec3_cross(&right, front, up);
                gb_vec3_norm(&right, right);
                gb_vec3_mul(&deltapos, right, speed);
                gb_vec3_sub(&eye, eye, deltapos);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                gb_vec3_cross(&right, front, up);
                gb_vec3_norm(&right, right);
                gb_vec3_mul(&deltapos, right, speed);
                gb_vec3_add(&eye, eye, deltapos);
        }
}

/* Updates the camera position by using the cursor offset. */
void updatecam(GLFWwindow* window, double xpos, double ypos)
{
        float xoff;
        float yoff;
        float sensitivity;
        gbVec3 direction;

        if (firstmouse)
        {
                lastx = (float)xpos;
                lasty = (float)ypos;
                firstmouse = 0;
        }
        xoff = (float)xpos - lastx;
        yoff = lasty - (float)ypos;
        lastx = (float)xpos;
        lasty = (float)ypos;
        sensitivity = 0.1f;
        xoff *= sensitivity;
        yoff *= sensitivity;
        yaw += xoff;
        pitch += yoff;
        if(pitch > 89.0f)
                pitch = 89.0f;
        if(pitch < -89.0f)
                pitch = -89.0f;
        direction.x = gb_cos(gb_to_radians(yaw)) * gb_cos(gb_to_radians(pitch));
        direction.y = gb_sin(gb_to_radians(pitch));
        direction.z = gb_sin(gb_to_radians(yaw)) * gb_cos(gb_to_radians(pitch));
        gb_vec3_norm(&direction, direction);
        front = direction;
}

/* Updates the FOV by using the scroll offset. */
void updatefov(GLFWwindow* window, double xoff, double yoff)
{
        fov -= (float)yoff;
        if (fov < 1.0f)
                fov = 1.0f;
        if (fov > 45.0f)
                fov = 45.0f;
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
        gbMat4 rotate;
        gbMat4 trans;
        gbMat4 view;
        gbMat4 project;
        gbVec3 centre;
        float angle;
        float currframe;

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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, updatecam);
        glfwSetScrollCallback(window, updatefov);
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
        eye = gb_vec3(0.0f, 0.0f, 3.0f);
        front = gb_vec3(0.0f, 0.0f, -1.0f);
        up = gb_vec3(0.0f, 1.0f, 0.0f);
        yaw = -90.0f;
        pitch = 0.0f;
        lastx = 400;
        lasty = 300;
        fov = 45.0f;
        while (!glfwWindowShouldClose(window)) {
                gb_vec3_add(&centre, eye, front);
                gb_mat4_look_at(&view, eye, centre, up);
                gb_mat4_perspective(&project, gb_to_radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
                glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view.e);
                glUniformMatrix4fv(glGetUniformLocation(shader, "project"), 1, GL_FALSE, project.e);
                currframe = (float)glfwGetTime();
                deltatime = currframe - lastframe;
                lastframe = currframe;
                procinput(window);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, tex2);
                glBindVertexArray(vao);
                for (i = 0; i < 10; i++) {
                        gb_mat4_identity(&rotate);
                        if (i % 3 == 0) {
                                angle = 20.0f * (float)glfwGetTime();
                                gb_mat4_rotate(&rotate, gb_vec3(1.0f, 0.3f, 0.5f), gb_to_radians(angle));
                        }
                        gb_mat4_translate(&trans, cubepos[i]);
                        gb_mat4_mul(&model, &trans, &rotate);
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
