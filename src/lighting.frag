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
        vec3 direction;
        float cut_off;
        float outer_cut_off;
        float constant;
        float linear;
        float quadratic;
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
        float distance;
        float attenuation;
        float theta;
        float epsilon;
        float intensity;
        vec3 normal;
        vec3 light_dir;
        vec3 view_dir;
        vec3 reflect_dir;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        /* ambient */
        ambient = light.ambient * texture(material.diffuse, tex_coords).rgb;
        /* diffuse */
        normal = normalize(norm);
        light_dir = normalize(light.position - frag_pos);
        diff_int = max(dot(normal, light_dir), 0.0);
        diffuse = light.diffuse * diff_int *
                  texture(material.diffuse, tex_coords).rgb;
        /* specular */
        view_dir = normalize(view_pos - frag_pos);
        reflect_dir = reflect(-light_dir, normal);
        spec_int = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        specular = light.specular * spec_int *
                   texture(material.specular, tex_coords).rgb;
        /* attenuation */
        distance = length(light.position - frag_pos);
        attenuation = 1.0 / (light.constant + light.linear * distance +
                      light.quadratic * (distance * distance));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        /* spotlight */
        theta = dot(light_dir, normalize(-light.direction));
        epsilon = light.cut_off - light.outer_cut_off;
        intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;
        /* resulting color */
        frag_color = vec4(ambient + diffuse + specular, 1.0);
}
