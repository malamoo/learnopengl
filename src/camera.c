#include "../external/cglm/cglm.h"
#include "../include/camera.h"

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

void camera_update_vectors(Camera *camera);

/* Initializes a camera with the specified configuration. */
void camera_init(Camera *camera, vec3 position, vec3 up, float yaw, float pitch)
{
        glm_vec3(position, camera->position);
        glm_vec3(up, camera->world_up);
        camera->yaw = yaw;
        camera->pitch = pitch;
        glm_vec3((vec3){ 0.0f, 0.0f, -1.0f }, camera->front);
        camera->move_speed = SPEED;
        camera->sensitivity = SENSITIVITY;
        camera->zoom = ZOOM;
        camera_update_vectors(camera);
}

/*
 * Returns the view matrix calculated using
 * Euler Angles and the "Look-At" Matrix.
 */
void camera_calc_view(Camera *camera, mat4 view)
{
        vec3 centre;

        glm_vec3_add(camera->position, camera->front, centre);
        glm_lookat(camera->position, centre, camera->up, view);
}

/* Processes input received from any keyboard-like input system. */
void camera_proc_kb(Camera *camera, CameraMove camera_move, float delta_time)
{
        float velocity = camera->move_speed * delta_time;
        vec3 delta_pos;

        if (camera_move == FORWARD) {
                glm_vec3_scale(camera->front, velocity, delta_pos);
                glm_vec3_add(camera->position, delta_pos, camera->position);
        }
        if (camera_move == BACKWARD) {
                glm_vec3_scale(camera->front, velocity, delta_pos);
                glm_vec3_sub(camera->position, delta_pos, camera->position);
        }
        if (camera_move == LEFT) {
                glm_vec3_scale(camera->right, velocity, delta_pos);
                glm_vec3_sub(camera->position, delta_pos, camera->position);
        }
        if (camera_move == RIGHT) {
                glm_vec3_scale(camera->right, velocity, delta_pos);
                glm_vec3_add(camera->position, delta_pos, camera->position);
        }
        if (camera_move == UP) {
                glm_vec3_scale(camera->world_up, velocity, delta_pos);
                glm_vec3_add(camera->position, delta_pos, camera->position);
        }
}

/*
 * Processes input received from a mouse scroll-wheel event.
 * Only requires input on the vertical wheel-axis.
 */
void camera_proc_scroll(Camera *camera, float y_off)
{
        camera->zoom -= (float)y_off;
        if (camera->zoom < 1.0f)
                camera->zoom = 1.0f;
        if (camera->zoom > 45.0f)
                camera->zoom = 45.0f;
}

/*
 * Processes input received from a mouse input system.
 * Expects the offset value in both the x and y direction.
 */
void camera_proc_mouse(Camera *camera, float x_off, float y_off, int bounded)
{
        x_off *= camera->sensitivity;
        y_off *= camera->sensitivity;
        camera->yaw += x_off;
        camera->pitch += y_off;
        if (bounded) {
                if (camera->pitch > 89.0f)
                        camera->pitch = 89.0f;
                else if (camera->pitch < -89.0f)
                        camera->pitch = -89.0f;
        }
        camera_update_vectors(camera);
}

/* Calculates the camera's vectors from the its (updated) Euler Angles. */
void camera_update_vectors(Camera *camera)
{
        float x;
        float y;
        float z;

        x = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
        y = sinf(glm_rad(camera->pitch));
        z = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
        glm_vec3((vec3){ x, y, z }, camera->front);
        glm_normalize(camera->front);
        glm_cross(camera->front, camera->world_up, camera->right);
        glm_normalize(camera->right);
        glm_cross(camera->right, camera->front, camera->up);
        glm_normalize(camera->up);
}

