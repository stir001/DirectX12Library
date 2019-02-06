#include "stdafx.h"
#include "FADLoader.h"
#include "FbxMotionData.h"

#include <fstream>
#include <Windows.h>

FADLoader::FADLoader()
{
}


FADLoader::~FADLoader()
{
}

std::shared_ptr<FbxMotionData> FADLoader::LoadFAD(const std::string & filePath)
{
	auto data = std::make_shared<FbxMotionData>();

	std::ifstream fileStream(filePath, std::ios_base::in | std::ios_base::binary);
	if (!fileStream)
	{
		MessageBoxA(nullptr, "ƒtƒ@ƒCƒ‹‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñ", "FMDLoader", MB_OK);
		return nullptr;
	}

	return nullptr;
}
