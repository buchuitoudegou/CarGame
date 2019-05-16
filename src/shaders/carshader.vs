#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 acolor;
layout (location = 4) in vec3 bcolor;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 dcolor;
// out vec3 scolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;  
	Normal=aNormal;
	FragPos=vec3(model*vec4(aPos,1.0));
    gl_Position = projection*view*vec4(FragPos,1.0);
    dcolor = acolor;
    // scolor = bcolor;
}
// #version 330

// uniform mat4 projection;
// uniform mat4 model;
// uniform mat4 view;

// uniform mat4 depth_pv;
// uniform vec4 clip_plane;

// // using named parameters for a test as defined in EntityShader.cpp
// in vec3 a_vertex;
// in vec3 a_normal;
// in vec2 a_tex_coord;

// out vec4 vertex; // vertex position in world space
// out vec3 normal; // the world space normal
// out vec4 shadowCoord; // shadow vertex position in world space
// out vec2 st;

// void main(void) {
//     vertex = model * vec4(a_vertex, 1.0);
//     normal = normalize(mat3(model) * a_normal);     // not using inverse-transpose but still seems to work
//     st = vec2(a_tex_coord.x, 1.0 - a_tex_coord.y);

//     gl_ClipDistance[0] = dot(vertex, clip_plane);
//     gl_Position = projection * view * vertex;
//     shadowCoord = depth_pv * vertex;
// }
