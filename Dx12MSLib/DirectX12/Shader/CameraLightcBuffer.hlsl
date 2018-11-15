#define CAMERA_CBUFFER(bnum) cbuffer camera : register(bnum) \
{	\
float4 eye;	\
float4 target;	\
float4x4 c_world; \
float4x4 c_view;	\
float4x4 c_projection;	\
}	

struct CameraStructure
{
    float4 eye;
    float4 target;
    float4x4 c_world;
    float4x4 c_view;
    float4x4 c_projection;
};

#define MULTI_CAMERA(bnum) cbuffer mulcamera : register(bnum) \
{	\
	uint cameraNum;	\
	CameraStructure	cameras[4];	\
}	

#define LIGHT_CBUFFER(bnum) cbuffer directionalLight : register(bnum)\
{	\
    float4 dir;	\
    float4x4 viewProj;	\
}	