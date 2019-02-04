#pragma once
#include <memory>
#include <string>

class FbxMotionData;

class FADLoader
{
public:
	FADLoader();
	~FADLoader();
	std::shared_ptr<FbxMotionData> LoadFAD(const std::string& filePath);
};

