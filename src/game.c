#include <stdio.h>
#include "../external/glad.h"
#include "../external/glfw.h"
#include "../external/gb_math.h"
#include "../include/shader.h"
#include "../include/texture.h"
#include "../include/camera.h"

/* Screen globals */
const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

/* Camera globals */
Camera camera;
const float YAW = -90.0f;
const float PITCH = 0.0f;
float last_y = (float)SCR_WIDTH / 2.0f;
float last_x = (float)SCR_HEIGHT / 2.0f;
int first_mouse = 1;

/* Timing globals */
float delta_time = 0.0f;
float last_frame = 0.0f;

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x_pos, double y_pos);

/*
 * Query GLFW as to whether relevant keys are pressed/released
 * this frame and react accordingly.
 */
void process_input(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, 1);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera_proc_kb(&camera, FORWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera_proc_kb(&camera, BACKWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera_proc_kb(&camera, LEFT, delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera_proc_kb(&camera, RIGHT, delta_time);
}

/* Called by GLFW whenever the framebuffer is resized. */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
        glViewport(0, 0, width, height);
}

/* Called by GLFW whenever the cursor moves within the window. */
void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
        float x_off;
        float y_off;

        if (first_mouse) {
                last_x = (float)x_pos;
                last_y = (float)y_pos;
                first_mouse = 0;
        }
        x_off = (float)x_pos - last_x;
        /* reversed because y-coordinates range from bottom to top */
        y_off = last_y - (float)y_pos;
        last_x = (float)x_pos;
        last_y = (float)y_pos;
        camera_proc_mouse(&camera, x_off, y_off, 1);
}

/* Called by GLFW whenever the mouse scroll wheel scrolls. */
void scroll_callback(GLFWwindow *window, double x_off, double y_off)
{
        camera_proc_scroll(&camera, (float)y_off);
}

int main(void)
{
        GLFWwindow *window;
        float vertices[] = {
             /* positions */      /* texture coordinates */
               -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

               -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
               -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
               -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
               -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
               -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

               -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
               -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
               -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
               -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

               -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
               -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

               -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

               -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
               -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
               -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
               -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
               -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        gbVec3 cube_positions[] = {
                gb_vec3(0.0f, 0.0f, 0.0f),
                gb_vec3(2.0f, 5.0f, -15.0f),
                gb_vec3(-1.5f, -2.2f, -2.5f),
                gb_vec3(-3.8f, -2.0f, -12.3f),
                gb_vec3(2.4f, -0.4f, -3.5f),
                gb_vec3(-1.7f, 3.0f, -7.5f),
                gb_vec3(1.3f, -2.0f, -2.5f),
                gb_vec3(1.5f, 2.0f, -2.5f),
                gb_vec3(1.5f, 0.2f, -1.5f),
                gb_vec3(-1.3f, 1.0f, -1.5f)
        };
        int i;
        unsigned int vbo;
        unsigned int vao;
        unsigned int tex1;
        unsigned int tex2;
        Shader shader;
        gbMat4 model;
        gbMat4 rotate;
        gbMat4 translate;
        gbMat4 view;
        gbMat4 projection;
        float angle;
        float curr_frame;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game", NULL, NULL);
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
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        shader_init(&shader, "src/vertex.glsl", "src/fragment.glsl");
        tex1 = make_texture("assets/container.jpeg");
        tex2 = make_texture("assets/awesomeface.png");
        glEnable(GL_DEPTH_TEST);
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
        shader_use(&shader);
        shader_load_int(&shader, "tex1", 0);
        shader_load_int(&shader, "tex2", 1);
        camera_init(&camera, gb_vec3(0.0f, 0.0f, 3.0f),
                    gb_vec3(0.0f, 1.0f, 0.0f), YAW, PITCH);
        while (!glfwWindowShouldClose(window)) {
                curr_frame = (float)glfwGetTime();
                delta_time = curr_frame - last_frame;
                last_frame = curr_frame;
                process_input(window);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, tex2);
                gb_mat4_perspective(&projection, gb_to_radians(camera.zoom),
                                    (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,
                                    100.0f);
                shader_load_mat4(&shader, "projection", &projection);
                camera_calc_view(&view, &camera);
                shader_load_mat4(&shader, "view", &view);
                glBindVertexArray(vao);
                for (i = 0; i < 10; i++) {
                        gb_mat4_identity(&rotate);
                        angle = 20.0f * (float)i;
                        gb_mat4_rotate(&rotate,
                                       gb_vec3(1.0f, 0.3f, 0.5f),
                                       gb_to_radians(angle));
                        gb_mat4_translate(&translate, cube_positions[i]);
                        gb_mat4_mul(&model, &translate, &rotate);
                        shader_load_mat4(&shader, "model", &model);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                glfwSwapBuffers(window);
                glfwPollEvents();
        }
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
        return 0;
}
