#version 410
//#version 330              //Ueberlebender supports up to GLSL 3.3.

in vec3 position_eye, normal_eye;
in vec2 texture_coords;

uniform mat4 viewMat;

out vec4 fragColor;

// temporary lighting
vec3 light_position_world  = vec3 (10.0, 25.0, 10.0);
vec3 Ls = vec3 (1.0, 1.0, 1.0);
vec3 Ld = vec3 (0.8, 0.8, 0.8);
vec3 La = vec3 (0.12, 0.12, 0.12);
vec3 Ks = vec3 (1.0, 1.0, 1.0);
vec3 Kd = vec3 (0.32, 0.18, 0.5);
vec3 Ka = vec3 (1.0, 1.0, 1.0);
float specular_exponent = 400.0;

uniform sampler2D texture00;
uniform sampler2D texture01;

void main () {
	vec3 Ia = La * Ka;

    // place-holder for multi-texturing setup
    // vec4 texel_a = texture (texture00, texture_coords*4.0);
    // vec4 texel_b = texture (texture01, texture_coords*4.0);
    // vec4 mixed   = mix (texel_a, texel_b, texture_coords.x);
    // Kd.x = mixed.x;
    // Kd.y = mixed.y;
    // Kd.z = mixed.z;

	vec3 light_position_eye = vec3 (viewMat * vec4 (light_position_world, 1.0));
	vec3 distance_to_light_eye = light_position_eye - position_eye;
	vec3 direction_to_light_eye = normalize (distance_to_light_eye);

    vec3 normalEye;
    normalEye = normalize(normal_eye);

	float dot_prod = dot (direction_to_light_eye, normalEye);
	dot_prod = max (dot_prod, 0.0);
	vec3 Id = Ld * Kd * dot_prod;

	vec3 surface_to_viewer_eye = normalize (-position_eye);

	vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
	float dot_prod_specular = max (dot (half_way_eye, normalEye), 0.0);
	float specular_factor = pow (dot_prod_specular, specular_exponent);

	vec3 Is = Ls * Ks * specular_factor;

	fragColor = vec4 (Is + Id + Ia, 1.0);
}

