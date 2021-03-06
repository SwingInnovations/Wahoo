#INSERT_HEADER

#include <STLight.glinc>

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;
uniform STLight lights[64];

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;
out mat3 TBN;
out vec3 Tangent;
out vec4 FragPosLightSpace;
out ShadowSpecs shadowSpecs[64];

void main(void){
    gl_Position = projection * view * model * vec4(position, 1.0);
    Position = (model * vec4(position, 1.0)).xyz;
    TexCoord = texCoord;
    vec3 T = vec3(model * vec4(tangent, 0.0));
    vec3 N = vec3(model * vec4(normal, 0.0));
    vec3 B = cross(T,N);
    Tangent = T;
    Normal = N;
    TBN = mat3(T, B, N);
    FragPosLightSpace = transpose(lightSpaceMatrix) * vec4(Position, 1.0);
}