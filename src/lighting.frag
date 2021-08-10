#version 330 core
struct Material {
        sampler2D diffuse;
        sampler2D specular;
        float shininess;
};
struct Light {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec3 position;
};

in vec3 norm;
in vec3 frag_pos;
in vec2 tex_coords;

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

out vec4 frag_color;

void main()
{
        float diff_int;
        float spec_int;
        vec3 normal;
        vec3 light_dir;
        vec3 view_dir;
        vec3 reflect_dir;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        /* ambient */
        ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));
        /* diffuse */
        normal = normalize(norm);
        light_dir = normalize(light.position - frag_pos);
        diff_int = max(dot(normal, light_dir), 0.0);
        diffuse = light.diffuse * diff_int *
                  vec3(texture(material.diffuse, tex_coords));
        /* specular */
        view_dir = normalize(view_pos - frag_pos);
        reflect_dir = reflect(-light_dir, normal);
        spec_int = pow(max(dot(view_dir, reflect_dir), 0.0),
                       material.shininess);
        specular = light.specular * spec_int *
                   vec3(texture(material.specular, tex_coords));
        vec3 result = ambient + diffuse + specular;
        frag_color = vec4(result, 1.0);
}
