#version 330 core
in vec3 norm;
in vec3 frag_pos;

uniform vec3 cube_color;
uniform vec3 lamp_color;
uniform vec3 lamp_pos;
uniform vec3 view_pos;

out vec4 frag_color;

void main()
{
        float amb_int;
        float diff_int;
        float spec_int;
        vec3 normal;
        vec3 lamp_dir;
        vec3 view_dir;
        vec3 reflect_dir;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        /* ambient */
        amb_int = 0.1;
        ambient = amb_int * lamp_color;
        normal = normalize(norm);
        lamp_dir = normalize(lamp_pos - frag_pos);
        /* diffuse */
        diff_int = max(dot(normal, lamp_dir), 0.0);
        diffuse = diff_int * lamp_color;
        /* specular */
        spec_int = 0.5;
        view_dir = normalize(view_pos - frag_pos);
        reflect_dir = reflect(-lamp_dir, normal);
        specular = spec_int * pow(max(dot(view_dir, reflect_dir), 0.0), 32) *
                   lamp_color;
        frag_color = vec4((ambient + diffuse + specular) * cube_color, 1.0);
}
