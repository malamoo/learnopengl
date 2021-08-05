#include "../external/glad.h"
#include "../external/glfw.h"
#include "../external/gb_math.h"

typedef enum camera_move {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} CameraMove;

typedef struct camera {
    gbVec3 pos;
    gbVec3 front;
    gbVec3 up;
    gbVec3 right;
    gbVec3 world_up;
    float yaw;
    float pitch;
    float move_speed;
    float sensitivity;
    float zoom;
} Camera;

void
camera_init(Camera *camera, gbVec3 position, gbVec3 up, float yaw, float pitch);
void camera_calc_view(gbMat4 *view, Camera *camera);
void camera_proc_kb(Camera *camera, CameraMove camera_move, float delta_time);
void camera_proc_mouse(Camera *camera, float x_off, float y_off, int bounded);
void camera_proc_scroll(Camera *camera, float y_off);

