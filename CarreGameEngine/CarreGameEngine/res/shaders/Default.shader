#shader vertex
#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec4 inColor;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoord = inTexCoord;
    gl_Position = projection * view * model * vec4(inPos, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoord);
}