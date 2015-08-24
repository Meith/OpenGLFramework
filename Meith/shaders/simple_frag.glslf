#version 330 core

in vec3 temp_normal;
in vec2 temp_texcoords;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirectionalLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 direction;
};

uniform Material material;
uniform DirectionalLight directional_light;

out vec4 out_colour;

void main()
{
	vec3 ambient = directional_light.ambient * vec3(texture(material.texture_diffuse1, temp_texcoords));
	
	vec3 norm = normalize(temp_normal);
	vec3 light_dir = normalize(-directional_light.direction);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = directional_light.diffuse * diff * vec3(texture(material.texture_diffuse1, temp_texcoords));

	out_colour = vec4(ambient + diffuse, 1.0f);
}
