#version 430 core
layout(location = 0) in vec2 vertex;

out vec4 vert;
out vec4 sample_norm;
out vec2 uv;

uniform float terrainScaling = 1.f;
uniform mat4 projViewMatrix;
uniform mat4 transInvViewMatrix;

uniform sampler2D height_sampler;
uniform sampler2D normal_sampler;

void main()
{

    // vertex.x : x coordinate
    // vertex.y : z coordinate
    vec2 uv = vertex.xy;
    uv /= terrainScaling;
    uv = fract(uv);

    // sample and pass norm to fragment shader
    sample_norm  = transInvViewMatrix * vec4(texture(normal_sampler, uv).rgb, 0.0);

    // height map sampling
    float height = texture(height_sampler, uv).r * terrainScaling;
    vec3 pos = vec3(vertex.x, height, vertex.y);
    gl_Position = projViewMatrix * vec4(pos, 1.0);

//// DEBUG
//    pos[1] = 0;
//    gl_Position = projViewMatrix * vec4(pos, 1.0); // to flat for seamless checking
}
