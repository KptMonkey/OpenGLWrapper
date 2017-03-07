#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;                                               
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;
                                                                 
out vec3 bla;                                                                 

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 gBones[MAX_BONES];

void main()
{       
    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform     += gBones[BoneIDs[3]] * Weights[3];

    vec4 PosL    = BoneTransform * vec4(Position, 1.0);
    gl_Position  = projection * view* model * PosL;
    bla = Normal;
}
