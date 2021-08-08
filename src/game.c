#include <stdio.h>
#include "../external/glad/glad.h"
#include "../external/glfw/glfw.h"
#include "../external/cglm/cglm.h"
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

/* Lighting globals */
vec3 lamp_pos = (vec3){ 1.2f, 1.0f, 2.0f };

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
                /* vertices       */  /* normals       */
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };
        unsigned int vbo;
        unsigned int cube_vao;
        unsigned int lamp_vao;
        Shader lighting_shader;
        Shader lamp_shader;
        vec3 cube_color;
        vec3 lamp_color;
        mat4 model;
        mat4 view;
        mat4 projection;
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
        shader_init(&lighting_shader, "src/lighting.vert", "src/lighting.frag");
        shader_init(&lamp_shader, "src/lamp.vert", "src/lamp.frag");
        glEnable(GL_DEPTH_TEST);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);
        glGenVertexArrays(1, &cube_vao);
        glBindVertexArray(cube_vao);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glGenVertexArrays(1, &lamp_vao);
        glBindVertexArray(lamp_vao);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
        glm_vec3((vec3){1.0f, 0.5f, 0.31f}, cube_color);
        glm_vec3((vec3){1.0f, 1.0f, 1.0f}, lamp_color);
        camera_init(&camera, (vec3){0.0f, 0.0f, 3.0f},
                    (vec3){0.0f, 1.0f, 0.0f}, YAW, PITCH);
        while (!glfwWindowShouldClose(window)) {
                curr_frame = (float)glfwGetTime();
                delta_time = curr_frame - last_frame;
                last_frame = curr_frame;
                process_input(window);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                shader_use(&lighting_shader);
                shader_load_vec3(&lighting_shader, "cube_color", cube_color);
                shader_load_vec3(&lighting_shader, "lamp_color", lamp_color);
                shader_load_vec3(&lighting_shader, "lamp_pos", lamp_pos);
                shader_load_vec3(&lighting_shader, "view_pos", camera.pos);
                glm_mat4_identity(model);
                glm_rotate_y(model, glm_rad((float)glfwGetTime() * 20.0f),
                             model);
                camera_calc_view(&camera, view);
                glm_perspective(glm_rad(camera.zoom),
                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,
                                100.0f, projection);
                shader_load_mat4(&lighting_shader, "model", model);
                shader_load_mat4(&lighting_shader, "view", view);
                shader_load_mat4(&lighting_shader, "projection", projection);
                glBindVertexArray(cube_vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                shader_use(&lamp_shader);
                glm_mat4_identity(model);
                glm_translate(model, lamp_pos);
                glm_scale(model, (vec3){0.2f, 0.2f, 0.2f});
                shader_load_mat4(&lamp_shader, "model", model);
                shader_load_mat4(&lamp_shader, "view", view);
                shader_load_mat4(&lamp_shader, "projection", projection);
                glBindVertexArray(lamp_vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glfwSwapBuffers(window);
                glfwPollEvents();
        }
        glDeleteVertexArrays(1, &cube_vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
        return 0;
}
