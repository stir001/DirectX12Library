#include "stdafx.h"
#include "PMXLoader.h"
#include "PMXModel.h"

void LoadTexBuffer(TextBuffer& buff, FILE* fp, size_t size = -1)
{
	unsigned int count = 0;
	fread(&count, sizeof(unsigned int), 1, fp);
	if (count == 0) return;
	buff.buffer.resize(count);
	fread(&buff.buffer[0], count * sizeof(unsigned char), 1, fp);
}

PMXLoader::PMXLoader()
{
}


PMXLoader::~PMXLoader()
{
}

template <typename T>
void LoadOwnSize(T& t, FILE* fp)
{
	fread(&t, sizeof(t), 1, fp);
}

bool CheckBit(unsigned short bit, unsigned short checkbit)
{
	return ((bit & checkbit) == checkbit);
}

//pmxを過程
void PMXLoader::Load(std::string path) {

	FILE* pmxfp = nullptr;
	fopen_s(&pmxfp, path.data(), "rb");

	PMXModel* model = new PMXModel();

	//ヘッダ
	LoadOwnSize(model->mHeader.type, pmxfp);
	LoadOwnSize(model->mHeader.ver, pmxfp);
	LoadOwnSize(model->mHeader.count, pmxfp);

	model->mHeader.info.resize(model->mHeader.count);
	fread(&model->mHeader.info[0], sizeof(model->mHeader.info[0]), model->mHeader.info.size(), pmxfp);

	//モデル情報(名前やコメントなど)
	LoadTexBuffer(model->mModelinfo.nameJP, pmxfp);
	LoadTexBuffer(model->mModelinfo.nameUS, pmxfp);
	LoadTexBuffer(model->mModelinfo.commentJP, pmxfp);
	LoadTexBuffer(model->mModelinfo.commentUS, pmxfp);

	mModels.push_back(model);

	//頂点
	LoadVertex(pmxfp);

	//面
	LoadPlane(pmxfp);

	//テクスチャ
	LoadTexture(pmxfp);

	//マテリアル
	LoadMaterial(pmxfp);

	//ボーン
	LoadBone(pmxfp);

	//モーフ
	LoadMorph(pmxfp);

	//表示枠
	LoadOutLine(pmxfp);

	//剛体
	LoadRigid(pmxfp);

	//joint
	LoadJoint(pmxfp);

	fclose(pmxfp);


}

void PMXLoader::LoadVertex(FILE* pmxfp)
{
	unsigned char boneIndexSize = mModels.back()->mHeader.info[BONEINDEXSIZE];
	unsigned int vertexcount = 0;
	LoadOwnSize(vertexcount, pmxfp);
	mModels.back()->mVertex.resize(vertexcount);

	for (unsigned int i = 0; i < vertexcount; i++)
	{
		mModels.back()->mVertex[i].addVertex.resize(16 * mModels.back()->mHeader.info[ADDUV]);
		fread(&mModels.back()->mVertex[i].pos, 32, 1, pmxfp);
		fread(&mModels.back()->mVertex[i].addVertex, 16 * 16 * mModels.back()->mHeader.info[ADDUV], 1, pmxfp);
		fread(&mModels.back()->mVertex[i].type, 1, 1, pmxfp);
		switch (mModels.back()->mVertex[i].type)
		{
		case 0:
			fread(&mModels.back()->mVertex[i].bone.data.bdef1.index, boneIndexSize, 1, pmxfp);
			break;
		case 1:
			fread(&mModels.back()->mVertex[i].bone.data.bdef2.index1, boneIndexSize, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef2.index2, boneIndexSize, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef2.weight1, sizeof(float), 1, pmxfp);
			break;
		case 2:
			fread(&mModels.back()->mVertex[i].bone.data.bdef4.index1, boneIndexSize, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef4.index2, boneIndexSize, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef4.index3, boneIndexSize, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef4.index4, boneIndexSize, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef4.weight1, sizeof(float), 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef4.weight2, sizeof(float), 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef4.weight3, sizeof(float), 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.bdef4.weight4, sizeof(float), 1, pmxfp);
			break;
		case 3:
			fread(&mModels.back()->mVertex[i].bone.data.sdef.index1, boneIndexSize, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.sdef.index2, boneIndexSize, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.sdef.weight1, sizeof(float), 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.sdef.c, 12, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.sdef.r0, 12, 1, pmxfp);
			fread(&mModels.back()->mVertex[i].bone.data.sdef.r1, 12, 1, pmxfp);
			break;
		default:
			break;
		}
		fread(&mModels.back()->mVertex[i].edgscale, sizeof(float), 1, pmxfp);
	}
}

void PMXLoader::LoadPlane(FILE* pmxfp)
{
	unsigned int indexcount = 0;
	LoadOwnSize(indexcount, pmxfp);
	indexcount /= 3;
	mModels.back()->mPlanes.index.resize(indexcount);
	for (unsigned int i = 0; i < indexcount; ++i)
	{
		fread(&mModels.back()->mPlanes.index[i].x, mModels.back()->mHeader.info[VERTEXINDEXSIZE], 1, pmxfp);
		fread(&mModels.back()->mPlanes.index[i].y, mModels.back()->mHeader.info[VERTEXINDEXSIZE], 1, pmxfp);
		fread(&mModels.back()->mPlanes.index[i].z, mModels.back()->mHeader.info[VERTEXINDEXSIZE], 1, pmxfp);
	}
}

void PMXLoader::LoadTexture(FILE* pmxfp)
{
	unsigned int texturecount = 0;
	LoadOwnSize(texturecount, pmxfp);
	mModels.back()->mTexPath.resize(texturecount);
	for (unsigned int i = 0; i < texturecount; i++)
	{
		LoadTexBuffer(mModels.back()->mTexPath[i], pmxfp);
	}
}

void PMXLoader::LoadMaterial(FILE* pmxfp)
{
	unsigned int materialcount = 0;
	fread(&materialcount, sizeof(unsigned int), 1, pmxfp);
	mModels.back()->mMaterials.resize(materialcount);
	for (auto& m : mModels.back()->mMaterials)
	{
		LoadTexBuffer(m.nameJP, pmxfp);

		LoadTexBuffer(m.nameUS, pmxfp);

		LoadOwnSize(m.diffuse, pmxfp);
		LoadOwnSize(m.specular, pmxfp);
		LoadOwnSize(m.specularrato, pmxfp);
		LoadOwnSize(m.ambient, pmxfp);

		LoadOwnSize(m.drawFlag, pmxfp);

		LoadOwnSize(m.edgecolor, pmxfp);
		LoadOwnSize(m.edgesize, pmxfp);

		fread(&m.tableindex, mModels.back()->mHeader.info[TEXINDEXSIZE], 1, pmxfp);
		fread(&m.spheretableindex, mModels.back()->mHeader.info[TEXINDEXSIZE], 1, pmxfp);
		LoadOwnSize(m.spheremode, pmxfp);

		LoadOwnSize(m.toonFlag, pmxfp);

		if (m.toonFlag == 0)
		{
			fread(&m.toonindex, sizeof(mModels.back()->mHeader.info[TEXINDEXSIZE]), 1, pmxfp);
		}
		else if (m.toonFlag == 1)
		{
			fread(&m.toonindex, sizeof(unsigned char), 1, pmxfp);
		}
		else
		{
			int i = 0;//でバック用
		}

		LoadTexBuffer(m.freetext, pmxfp);
		LoadOwnSize(m.adaptVertexcount, pmxfp);
	}
}

void PMXLoader::LoadBone(FILE* pmxfp)
{
	unsigned int bonecount = 0;
	fread(&bonecount, sizeof(unsigned int), 1, pmxfp);
	mModels.back()->mBonedata.resize(bonecount);
	for (auto& bone : mModels.back()->mBonedata)
	{
		LoadTexBuffer(bone.boneNameJP, pmxfp);
		LoadTexBuffer(bone.boneNameUS, pmxfp);

		LoadOwnSize(bone.pos, pmxfp);
		fread(&bone.parentIndex, mModels.back()->mHeader.info[BONEINDEXSIZE], 1, pmxfp);
		LoadOwnSize(bone.formlevel, pmxfp);
		LoadOwnSize(bone.boneflag, pmxfp);

		if (CheckBit(bone.boneflag, 0x0001))
		{
			fread(&bone.conectboneIndex, mModels.back()->mHeader.info[BONEINDEXSIZE], 1, pmxfp);
		}
		else
		{
			LoadOwnSize(bone.offsetpos, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0100))
		{
			fread(&bone.addparentboneIndex, mModels.back()->mHeader.info[BONEINDEXSIZE], 1, pmxfp);
			LoadOwnSize(bone.addparentRato, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0200))
		{
			fread(&bone.addparentboneIndex, mModels.back()->mHeader.info[BONEINDEXSIZE], 1, pmxfp);
			LoadOwnSize(bone.addparentRato, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0400))
		{
			LoadOwnSize(bone.dirvec, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0800))
		{
			LoadOwnSize(bone.xAxisvec, pmxfp);
			LoadOwnSize(bone.zAxisvec, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x2000))
		{
			LoadOwnSize(bone.key, pmxfp);
		}

		if (CheckBit(bone.boneflag, 0x0020))
		{
			fread(&bone.ikdata.targetboneIndex, sizeof(mModels.back()->mHeader.info[BONEINDEXSIZE]), 1, pmxfp);
			fseek(pmxfp, 1, SEEK_CUR);
			LoadOwnSize(bone.ikdata.roopcount, pmxfp);
			LoadOwnSize(bone.ikdata.maxrad, pmxfp);
			LoadOwnSize(bone.ikdata.linkcount, pmxfp);
			bone.ikdata.links.resize(bone.ikdata.linkcount);
			for (auto& ik : bone.ikdata.links)
			{
				fread(&ik.linkboneIndex, mModels.back()->mHeader.info[BONEINDEXSIZE], 1, pmxfp);
				LoadOwnSize(ik.maxradflag, pmxfp);
				if (ik.maxradflag == 1)
				{
					LoadOwnSize(ik.minrad, pmxfp);
					LoadOwnSize(ik.maxrad, pmxfp);
				}
			}
		}
	}
}

void PMXLoader::LoadMorph(FILE* pmxfp)
{
	unsigned int morphcount;
	LoadOwnSize(morphcount, pmxfp);
	mModels.back()->mMorphDatas.resize(morphcount);
	for (auto& data : mModels.back()->mMorphDatas)
	{
		LoadTexBuffer(data.nameJP, pmxfp);
		LoadTexBuffer(data.nameUS, pmxfp);
		LoadOwnSize(data.controllpanel, pmxfp);
		LoadOwnSize(data.morphtype, pmxfp);
		LoadOwnSize(data.morphcount, pmxfp);
		data.morphs.resize(data.morphcount);
		for (auto& morph : data.morphs)
		{
			switch (data.morphtype)
			{
			case mt_vertex:
				fread(&morph.vMorph.vertexIndex, mModels.back()->mHeader.info[VERTEXINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.vMorph.offsetpos.x, pmxfp);
				LoadOwnSize(morph.vMorph.offsetpos.y, pmxfp);
				LoadOwnSize(morph.vMorph.offsetpos.z, pmxfp);
				break;
			case mt_uv:
				fread(&morph.uMorph.vertexIndex, mModels.back()->mHeader.info[VERTEXINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.x, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.y, pmxfp);
				break;
			case mt_uvadd1:
				fread(&morph.uMorph.vertexIndex, mModels.back()->mHeader.info[VERTEXINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.x, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.y, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.z, pmxfp);
				break;
			case mt_uvadd2:
				fread(&morph.uMorph.vertexIndex, mModels.back()->mHeader.info[VERTEXINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.x, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.y, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.z, pmxfp);
				LoadOwnSize(morph.uMorph.uvoffset.w, pmxfp);
				break;
			case mt_bone:
				fread(&morph.bMorph.boneIndex, mModels.back()->mHeader.info[BONEINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.bMorph.velocity, pmxfp);
				LoadOwnSize(morph.bMorph.rotator, pmxfp);
				break;
			case mt_material:
				fread(&morph.mMorph.materialIndex, mModels.back()->mHeader.info[MATERIALINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.mMorph.offsetcalType, pmxfp);
				LoadOwnSize(morph.mMorph.diffuse, pmxfp);
				LoadOwnSize(morph.mMorph.specular, pmxfp);
				LoadOwnSize(morph.mMorph.specularrato, pmxfp);
				LoadOwnSize(morph.mMorph.ambient, pmxfp);
				LoadOwnSize(morph.mMorph.edgecolor, pmxfp);
				LoadOwnSize(morph.mMorph.edgesize, pmxfp);
				LoadOwnSize(morph.mMorph.texrato, pmxfp);
				LoadOwnSize(morph.mMorph.spheretexrato, pmxfp);
				LoadOwnSize(morph.mMorph.toontexrato, pmxfp);
				break;
			case mt_group:
				fread(&morph.gMorph.index, mModels.back()->mHeader.info[MORPHINDEXSIZE], 1, pmxfp);
				LoadOwnSize(morph.gMorph.mophrato, pmxfp);
				break;
			default:
				break;
			}
		}
	}
}

void PMXLoader::LoadOutLine(FILE* pmxfp)
{
	unsigned int outlincount = 0;
	LoadOwnSize(outlincount, pmxfp);
	mModels.back()->mOutline.resize(outlincount);

	for (auto& o : mModels.back()->mOutline)
	{
		LoadTexBuffer(o.nameJP, pmxfp);
		LoadTexBuffer(o.nameUS, pmxfp);

		LoadOwnSize(o.flag, pmxfp);
		LoadOwnSize(o.count, pmxfp);
		o.datas.resize(o.count);
		for (auto& d : o.datas)
		{
			LoadOwnSize(d.type, pmxfp);
			switch (d.type)
			{
			case 0://ボーン
				fread(&d.index, mModels.back()->mHeader.info[BONEINDEXSIZE], 1, pmxfp);
				break;
			case 1://モーフ
				fread(&d.index, mModels.back()->mHeader.info[MORPHINDEXSIZE], 1, pmxfp);
				break;
			default:
				break;
			}
		}
	}
}

void PMXLoader::LoadRigid(FILE* pmxfp)
{
	unsigned int rigidcount = 0;
	LoadOwnSize(rigidcount, pmxfp);
	mModels.back()->mRigid.resize(rigidcount);

	for (auto& r : mModels.back()->mRigid)
	{
		LoadTexBuffer(r.nameJP, pmxfp);
		LoadTexBuffer(r.nameUS, pmxfp);

		fread(&r.boneIndex, mModels.back()->mHeader.info[BONEINDEXSIZE], 1, pmxfp);

		LoadOwnSize(r.group, pmxfp);
		LoadOwnSize(r.groupflag, pmxfp);

		LoadOwnSize(r.shape, pmxfp);
		LoadOwnSize(r.size, pmxfp);

		LoadOwnSize(r.pos, pmxfp);
		LoadOwnSize(r.rotator, pmxfp);

		LoadOwnSize(r.weight, pmxfp);
		LoadOwnSize(r.accelvel, pmxfp);
		LoadOwnSize(r.accelrota, pmxfp);
		LoadOwnSize(r.boundpw, pmxfp);
		LoadOwnSize(r.frictionpw, pmxfp);

		LoadOwnSize(r.rigidflag, pmxfp);
	}
}

void PMXLoader::LoadJoint(FILE* pmxfp)
{
	unsigned int jointcount = 0;
	LoadOwnSize(jointcount, pmxfp);
	mModels.back()->mJoints.resize(jointcount);
	for (auto& j : mModels.back()->mJoints)
	{
		LoadTexBuffer(j.nameJP, pmxfp);
		LoadTexBuffer(j.nameUS, pmxfp);
		LoadOwnSize(j.type, pmxfp);
		fread(&j.spring.rigidAIndex, mModels.back()->mHeader.info[RIGIDINDEXSIZE], 1, pmxfp);
		fread(&j.spring.rigidBIndex, mModels.back()->mHeader.info[RIGIDINDEXSIZE], 1, pmxfp);
		LoadOwnSize(j.spring.pos, pmxfp);
		LoadOwnSize(j.spring.rotator, pmxfp);
		LoadOwnSize(j.spring.minVel, pmxfp);
		LoadOwnSize(j.spring.maxVel, pmxfp);
		LoadOwnSize(j.spring.minRotator, pmxfp);
		LoadOwnSize(j.spring.maxRotator, pmxfp);
		LoadOwnSize(j.spring.springCVel, pmxfp);
		LoadOwnSize(j.spring.springCRota, pmxfp);

	}
}