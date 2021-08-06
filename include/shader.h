typedef struct Shader {
    unsigned int id;
} Shader;

void shader_init(Shader *shader, const char *vertex_path,
                 const char *fragment_path);
void shader_use(Shader *shader);
void shader_load_int(Shader *shader, const char *name, int value);
void shader_load_float(Shader *shader, const char *name, float value);
void shader_load_vec2(Shader *shader, const char *name, gbVec2 *value);
void shader_load_vec3(Shader *shader, const char *name, gbVec3 *value);
void shader_load_vec4(Shader *shader, const char *name, gbVec4 *value);
void shader_load_mat2(Shader *shader, const char *name, gbMat2 *value);
void shader_load_mat3(Shader *shader, const char *name, gbMat3 *value);
void shader_load_mat4(Shader *shader, const char *name, gbMat4 *value);