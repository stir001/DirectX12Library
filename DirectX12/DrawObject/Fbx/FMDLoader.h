#pragma once
#include <fstream>
#include <memory>
#include <string>

struct FMDFileData;

class FMDLoader
{
public:
	FMDLoader();
	~FMDLoader();

	FMDFileData LoadFMD(const std::string& fmdPath);
private:
	void LoadVertices(std::ifstream& stream, FMDFileData& modelData);
	void LoadIndices(std::ifstream& stream, FMDFileData& modelData);
	void LoadMaterials(std::ifstream& stream, FMDFileData& modelData);
	void LoadSkeletons(std::ifstream& stream, FMDFileData& modelData);
};

