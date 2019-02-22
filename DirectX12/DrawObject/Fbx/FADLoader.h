#pragma once
#include <memory>
#include <string>

class FbxAnimationData;

class FADLoader
{
public:
	FADLoader();
	~FADLoader();
	std::shared_ptr<FbxAnimationData> LoadFAD(const std::string& filePath);
};

