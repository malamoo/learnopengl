#include "../include/camera.h"

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

void camera_update_vectors(Camera *camera);

/* Initializes a camera with the specified configuration. */
void
camera_init(Camera *camera, gbVec3 position, gbVec3 up, float yaw, float pitch)
{
        camera->pos = position;
        camera->world_up = up;
        camera->yaw = yaw;
        camera->pitch = pitch;
        camera->front = gb_vec3(0.0f, 0.0f, -1.0f);
        camera->move_speed = SPEED;
        camera->sensitivity = SENSITIVITY;
        camera->zoom = ZOOM;
        camera_update_vectors(camera);
}

/*
 * Returns the view matrix calculated using
 * Euler Angles and the "Look-At" Matrix.
 */
void camera_calc_view(gbMat4 *view, Camera *camera)
{
        gbVec3 centre;

        gb_vec3_add(&centre, camera->pos, camera->front);
        gb_mat4_look_at(view, camera->pos, centre, camera->up);
}

/* Processes input received from any keyboard-like input system. */
void camera_proc_kb(Camera *camera, CameraMove camera_move, float delta_time)
{
        float velocity = camera->move_speed * delta_time;
        gbVec3 delta_pos;

        if (camera_move == FORWARD) {
                gb_vec3_mul(&delta_pos, camera->front, velocity);
                gb_vec3_add(&camera->pos, camera->pos, delta_pos);
        }
        if (camera_move == BACKWARD) {
                gb_vec3_mul(&delta_pos, camera->front, velocity);
                gb_vec3_sub(&camera->pos, camera->pos, delta_pos);
        }
        if (camera_move == LEFT) {
                gb_vec3_mul(&delta_pos, camera->right, velocity);
                gb_vec3_sub(&camera->pos, camera->pos, delta_pos);
        }
        if (camera_move == RIGHT) {
                gb_vec3_mul(&delta_pos, camera->right, velocity);
                gb_vec3_add(&camera->pos, camera->pos, delta_pos);
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
        gbVec3 front;

        front.x = gb_cos(gb_to_radians(camera->yaw)) *
                  gb_cos(gb_to_radians(camera->pitch));
        front.y = gb_sin(gb_to_radians(camera->pitch));
        front.z = gb_sin(gb_to_radians(camera->yaw)) *
                  gb_cos(gb_to_radians(camera->pitch));
        gb_vec3_norm(&front, front);
        camera->front = front;
        gb_vec3_cross(&camera->right, camera->front, camera->world_up);
        gb_vec3_norm(&camera->right, camera->right);
        gb_vec3_cross(&camera->up, camera->right, camera->front);
        gb_vec3_norm(&camera->up, camera->up);
}

