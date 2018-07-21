#version 450

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec4 color_in;

layout(set = 0, binding = 0) uniform per_frame_uniforms {
    mat4 gbufferModelView;
    mat4 gbufferModelViewInverse;
    mat4 gbufferPreviousModelView;
    mat4 gbufferProjection;
    mat4 gbufferProjectionInverse;
    mat4 gbufferPreviousProjection;
    mat4 shadowProjection;
    mat4 shadowProjectionInverse;
    mat4 shadowModelView;
    mat4 shadowModelViewInverse;
    vec4 entityColor;
    vec3 fogColor;
    vec3 skyColor;
    vec3 sunPosition;
    vec3 moonPosition;
    vec3 shadowLightPosition;
    vec3 upPosition;
    vec3 cameraPosition;
    vec3 previousCameraPosition;
    ivec2 eyeBrightness;
    ivec2 eyeBrightnessSmooth;
    ivec2 terrainTextureSize;
    ivec2 atlasSize;
    int heldItemId;
    int heldBlockLightValue;
    int heldItemId2;
    int heldBlockLightValue2;
    int fogMode;
    int worldTime;
    int moonPhase;
    int terrainIconSize;
    int isEyeInWater;
    int hideGUI;
    int entityId;
    int blockEntityId;
    float frameTimeCounter;
    float sunAngle;
    float shadowAngle;
    float rainStrength;
    float aspectRatio;
    float viewWidth;
    float viewHeight;
    float near;
    float far;
    float wetness;
    float eyeAltitude;
    float centerDepthSmooth;
};

layout(set = 1, binding = 0) uniform per_model_uniforms{
    mat4 gbufferModel;
};

layout(location = 0) out vec2 uv;
layout(location = 1) out vec4 color;

void main() {
    float w=854.0f;
    float h=480.0f;
    float a=w/h;
    float fov=3.14159265*20.0/180.0;
    float d=1.0f/tan(fov/2.0f);
    float n=0.001f;
    float f=3.0f;
    mat4 m;
    m[0] = vec4(d/a,0.0f,0.0f,0.0f); // sets the first column
    m[1] = vec4(0.0f,d,0.0f,0.0f); // sets the second column
    m[2] = vec4(0.0f,0.0f,(n+f)/(n-f),-1.0f); // sets the third column
    m[2] = vec4(0.0f,0.0f,(2.0f*n*f)/(n-f),0.0f); // sets the fourth column
    vec4 test =  m*vec4(position_in,1.0f);//vec4(vec3(position_in.x,position_in.y,0.0f)+vec3(0.25f,0.25f,1.0f), 1.0f);//vec4(vec3((gbufferProjection *vec4(position_in*10.0f,1.0f)).xy,0.0f),1.0f);//vec4(vec3(position_in.x,position_in.y,0.0f)+vec3(0.5f,0.5f,0.0f), 1.0f);
    gl_Position=vec4(vec3(test.xy,1.0f),1.0f);
    uv = uv_in;
    color = color_in;
}
