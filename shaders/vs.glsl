#version 410
//#version 330                              //Ueberlebender supports up to GLSL 3.3.

uniform mat4 projMat, viewMat, modelMat;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexcoord;

out vec3 position_eye, normal_eye;
out vec2 texture_coords;

void main()
{
    texture_coords = vertexTexcoord;
	position_eye = vec3 (viewMat * modelMat * vec4 (vertexPosition, 1.0));
	normal_eye = normalize(vec3 (viewMat * modelMat * vec4 (vertexNormal, 0.0)));

	gl_Position = projMat * vec4 (position_eye, 1.0);
}



