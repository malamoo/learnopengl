#version 330 core

struct Material {
        sampler2D diffuse;
        sampler2D specular;
        float shininess;
};

struct DirLight {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};

struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
};

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos,
                      vec3 view_dir);

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coords;

uniform Material material;
uniform DirLight dir_light;
uniform PointLight point_light;
uniform vec3 view_pos;

out vec4 frag_color;

void main()
{
        vec3 norm;
        vec3 view_dir;
        vec3 result;

        /* properties */
        norm = normalize(normal);
        view_dir = normalize(view_pos - frag_pos);
        /* phase 1: directional lighting */
        result = calc_dir_light(dir_light, norm, view_dir);
        /* phase 2: point light */
        result += calc_point_light(point_light, norm, frag_pos, view_dir);
        frag_color = vec4(result, 1.0);
}

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir)
{
        vec3 light_dir;
        vec3 reflect_dir;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float diff_int;
        float spec_int;

        light_dir = normalize(-light.direction);
        /* diffuse shading */
        diff_int = max(dot(normal, light_dir), 0.0);
        /* specular shading */
        reflect_dir = reflect(-light_dir, normal);
        spec_int = pow(max(dot(view_dir, reflect_dir), 0.0),
                       material.shininess);
        /* combine results */
        ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));
        diffuse = light.diffuse * diff_int *
                  vec3(texture(material.diffuse, tex_coords));
        specular = light.specular * spec_int *
                   vec3(texture(material.specular, tex_coords));
        return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos,
                      vec3 view_dir)
{
        vec3 light_dir;
        vec3 reflect_dir;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float diff_int;
        float spec_int;
        float distance;
        float attenuation;

        light_dir = normalize(light.position - frag_pos);
        /* diffuse shading */
        diff_int = max(dot(normal, light_dir), 0.0);
        /* specular shading */
        reflect_dir = reflect(-light_dir, normal);
        spec_int = pow(max(dot(view_dir, reflect_dir), 0.0),
                       material.shininess);
        /* attenuation */
        distance = length(light.position - frag_pos);
        attenuation = 1.0 / (light.constant + light.linear * distance +
                            light.quadratic * (distance * distance));
        /* combine results */
        ambient = light.ambient * vec3(texture(material.diffuse, tex_coords));
        diffuse = light.diffuse * diff_int *
                  vec3(texture(material.diffuse, tex_coords));
        specular = light.specular * spec_int *
                   vec3(texture(material.specular, tex_coords));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        return (ambient + diffuse + specular);
}
