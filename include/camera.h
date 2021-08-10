typedef struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;
    float yaw;
    float pitch;
    float move_speed;
    float sensitivity;
    float zoom;
} Camera;

typedef enum CameraMove {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP
} CameraMove;

void camera_init(Camera *camera, vec3 position, vec3 up, float yaw, float
                 pitch);
void camera_calc_view(Camera *camera, mat4 view);
void camera_proc_kb(Camera *camera, CameraMove camera_move, float delta_time);
void camera_proc_mouse(Camera *camera, float x_off, float y_off, int bounded);
void camera_proc_scroll(Camera *camera, float y_off);

