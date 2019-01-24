#define CAMERA_CBUFFER(bnum) cbuffer camera : register(bnum) \
{	\
float4 eye;	\
float4 target;	\
float4x4 c_world; \
float4x4 c_view;	\
float4x4 c_projection;	\
}	

struct CameraElement
{
    float4 eye;
    float4 target;
    float4x4 c_world;
    float4x4 c_view;
    float4x4 c_projection;
};

#define MAX_CAMERA_NUM (4U)

#define MULTI_CAMERA(bnum) cbuffer mulcamera : register(bnum) \
{	\
	CameraElement cameras[MAX_CAMERA_NUM];	\
	uint cameraNum;	\
}	

#define LIGHT_CBUFFER(bnum) cbuffer directionalLight : register(bnum)\
{	\
    float4 tailPos;	\
    float4x4 lightviewProj;	\
}	