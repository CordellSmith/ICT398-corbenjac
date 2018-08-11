#shader vertex
#version 330 core

layout(location = 0) in vec3 VertexPosition_ModelSpace;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec3 VertexNormal_ModelSpace;

out vec2 v_VertexUV;
out vec3 v_VertexPosition_WorldSpace;
out vec3 v_VertexNormal_CameraSpace;
out vec3 v_VertexToCamera_CameraSpace;
out vec3 v_LightDirection_CameraSpace;
out vec3 v_LightPosition_WorldSpace;
out mat4 v_ModelViewMatrix;

uniform mat4 u_MVPMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;

void main() {
	vec3 u_LightPosition = vec3(1,-1,0);
	gl_Position = u_MVPMatrix * vec4(VertexPosition_ModelSpace, 1.0f);
	v_VertexPosition_WorldSpace = (u_ModelMatrix * vec4(VertexPosition_ModelSpace, 1.0f)).xyz;
	v_ModelViewMatrix = u_ViewMatrix * u_ModelMatrix;
	vec3 VertexPosition_CameraSpace = ( v_ModelViewMatrix * vec4(VertexPosition_ModelSpace, 1.0f)).xyz;
	v_VertexToCamera_CameraSpace = vec3(0,0,0) - VertexPosition_CameraSpace;
	vec3 LightPosition_CameraSpace = ( v_ModelViewMatrix * vec4(u_LightPosition, 1.0f)).xyz;
	v_LightDirection_CameraSpace = u_LightPosition + v_VertexToCamera_CameraSpace;
	v_VertexNormal_CameraSpace = ( u_ViewMatrix * u_ModelMatrix * vec4(VertexNormal_ModelSpace, 0.0f)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	v_VertexUV = VertexUV;
	v_LightPosition_WorldSpace = u_LightPosition;
}

#shader fragment
#version 330 core

in vec2 v_VertexUV;
uniform sampler2D u_Texture0;

//in vec3 v_VertexPosition_WorldSpace;
//in vec3 v_VertexNormal_CameraSpace;
//in vec3 v_VertexToCamera_CameraSpace;
//in vec3 v_LightDirection_CameraSpace;
//in vec3 v_LightPosition_WorldSpace;
//in mat4 v_ModelViewMatrix;

out vec4 color;

void main() {

	vec3 tex0 = texture(u_Texture0, v_VertexUV).rgb;

	color = vec4(tex0, 1.0f);
}