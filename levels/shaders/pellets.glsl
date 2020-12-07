#shader #vertex
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_TransformationMat = mat4(1);
uniform mat4 u_ViewMat           = mat4(1);
uniform mat4 u_ProjectionMat     = mat4(1);
uniform mat4 u_Scale			 = mat4(1);


void main()
{
	gl_Position = u_ProjectionMat * u_ViewMat * u_TransformationMat * u_Scale * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	v_TexCoord = texCoord;
};


#shader #fragment
#version 430 core

layout (location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
   vec4 texColor = texture(u_Texture, v_TexCoord);
   FragColor = texColor;
};