#shader vertex
#version 330 core

//CS = camera space, WS = world space

layout(location = 0) in vec3 MS_VertexPosition;
layout(location = 1) in vec2 MS_VertexUV;
layout(location = 2) in vec3 MS_VertexNormal;

out vec3  v_MS_VertexPosition;
out vec2  v_VertexUV;
out vec3  v_CS_VertexNormal;
out vec3  v_CS_EyeDirection;
out vec3  v_CS_LightDirection;
out vec3  v_WS_VertexPosition;
out vec3  v_WS_LightPosition;

uniform mat4 u_MVPMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;

void main() {
	vec3 u_WS_LightPosition = vec3(0, -1, 1);
	gl_Position = u_MVPMatrix * vec4(MS_VertexPosition, 1.0f);
	v_WS_VertexPosition = (u_ModelMatrix * vec4(MS_VertexPosition, 1.0f)).xyz;
	vec3 CS_VertexPosition = ( u_ViewMatrix * u_ModelMatrix * vec4(MS_VertexPosition, 1.0f)).xyz;
	v_CS_EyeDirection = vec3(0.0f, 0.0f, 0.0f) - CS_VertexPosition;
	vec3 CS_LightPosition = ( u_ViewMatrix * vec4(u_WS_LightPosition, 0.0f)).xyz;
	v_CS_LightDirection = CS_LightPosition + v_CS_EyeDirection;
	v_CS_VertexNormal = ( u_ViewMatrix * u_ModelMatrix * vec4(MS_VertexNormal, 0.0f)).xyz; // Use inverse transpose if model gets scaled.
	v_VertexUV = MS_VertexUV;
	v_WS_LightPosition = u_WS_LightPosition;
	v_MS_VertexPosition = MS_VertexPosition;
}

#shader fragment
#version 330 core

in vec3  v_MS_VertexPosition;
in vec2  v_VertexUV;
in vec3  v_CS_VertexNormal;
in vec3  v_CS_EyeDirection;
in vec3  v_CS_LightDirection;
in vec3  v_WS_VertexPosition;
in vec3  v_WS_LightPosition;

out vec4 color;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform sampler2D u_Texture4;
uniform sampler2D u_Texture5;

uniform vec3 u_PlayerPosition;
uniform float u_ScaleUV;

void main() {
	vec2 UV = v_VertexUV;
	
	vec3 vertex = vec3(v_MS_VertexPosition.x, 0, v_MS_VertexPosition.z);
	vec3 player = vec3(u_PlayerPosition.x, 0, u_PlayerPosition.z);
	
	float detail = length(player - vertex);
	
	if(detail < 50.0f && u_ScaleUV != 1.0f) {
		UV = (UV - 0.5) * u_ScaleUV + (0.5 * u_ScaleUV);
	}
		
	vec4 t0 = texture(u_Texture0, UV);
	vec4 t1 = texture(u_Texture1, UV);
	vec4 t2 = texture(u_Texture2, UV);
	vec4 t3 = texture(u_Texture3, UV);
	vec4 t4 = texture(u_Texture4, UV);
	vec4 t5 = texture(u_Texture5, UV);

	float v_Height = v_MS_VertexPosition.y;
	
	vec4 preColor;	
	if     (v_Height < 51)  { preColor = mix(t0, t1, (v_Height         ) / 51.0f); }
	else if(v_Height < 102) { preColor = mix(t1, t2, (v_Height -  51.0f) / 51.0f); }
	else if(v_Height < 153) { preColor = mix(t2, t3, (v_Height - 102.0f) / 51.0f); }
	else if(v_Height < 204) { preColor = mix(t3, t4, (v_Height - 153.0f) / 51.0f); }
	else 					{ preColor = mix(t4, t5, (v_Height - 204.0f) / 51.0f); }

	vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);
	float LightPower = 5000.0f;
	
	vec3 MaterialDiffuseColor = preColor.xyz;
	vec3 MaterialAmbientColor = vec3(0.8,0.8,0.8) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.5,0.5,0.3);
	float distance = length( v_WS_LightPosition - v_WS_VertexPosition );
	vec3 n = normalize( v_CS_VertexNormal );
	vec3 l = normalize( v_CS_LightDirection );
	float cosTheta = clamp( dot( n,l ), 0,1 );
	vec3 E = normalize(v_CS_EyeDirection);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	vec3 result = MaterialAmbientColor + MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) + MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
	
	color = vec4(result, 1);
}