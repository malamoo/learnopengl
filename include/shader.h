typedef struct Shader {
    unsigned int id;
} Shader;

void shader_init(Shader *shader, const char *vertex_path,
                 const char *fragment_path);
void shader_use(Shader *shader);
void shader_assign_int(Shader *shader, const char *name, int value);
void shader_assign_float(Shader *shader, const char *name, float value);
void shader_assign_vec2(Shader *shader, const char *name, vec2 value);
void shader_assign_vec3(Shader *shader, const char *name, vec3 value);
void shader_assign_vec4(Shader *shader, const char *name, vec4 value);
void shader_assign_mat2(Shader *shader, const char *name, mat2 value);
void shader_assign_mat3(Shader *shader, const char *name, mat3 value);
void shader_assign_mat4(Shader *shader, const char *name, mat4 value);