#pragma once
#include "DirectX12/DrawObject/DrawObjectLoader.h"

#include <string>
#include <vector>

class PMXModel;
struct PMXVertex;
struct PMXPlane;
struct TextBuffer;
struct PMXMatarial;
struct PMXBoneData;
struct PMXMorph;
struct PMXOutLine;
struct PMXRigidBody;
struct PMXJoint;

class PMXLoader :
	public DrawObjectLoader
{
public:
	PMXLoader();
	~PMXLoader();

	void Load(std::string path);
private:
	std::vector<PMXModel*> mModels;

	void LoadVertex(FILE* pmxfp);
	void LoadPlane(FILE* pmxfp);
	void LoadTexture(FILE* pmxfp);
	void LoadMaterial(FILE* pmxfp);
	void LoadBone(FILE* pmxfp);
	void LoadMorph(FILE* pmxfp);
	void LoadOutLine(FILE* pmxfp);
	void LoadRigid(FILE* pmxfp);
	void LoadJoint(FILE* pmxfp);
};

