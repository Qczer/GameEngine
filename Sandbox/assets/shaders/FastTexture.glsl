#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in flat int a_TexIndex;
layout(location = 4) in float a_TilingFactor;

layout(location = 5) in vec3 i_Translation;
layout(location = 6) in vec2 i_Scale;
layout(location = 7) in float i_Rotation;


uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
flat int float v_TexIndex;
out float v_TilingFactor;

void main()
{
	float s = sin(i_Rotation);
	float c = cos(i_Rotation);
	mat2 rotation = mat2(c, -s, s, c);

	vec2 localPos = rotation * (a_Position.xy * i_Scale);

	vec3 worldPos = i_Translation + vec3(localPos, a_Position.z);

	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(worldPos, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat int v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[v_TexIndex], v_TexCoord * v_TilingFactor) * v_Color;
	color = vec4(v_TexIndex, 0, 0, 1);
}