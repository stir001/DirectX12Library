#include "stdafx.h"
#include "PMDModel.h"

PMDModel::PMDModel()
{
}

PMDModel::~PMDModel()
{
}

void PMDModel::SetMaterialBuffer()
{
	
}

std::shared_ptr<PMDModel> PMDModel::Create()
{
	class CreateHelperPMD : public PMDModel
	{
	public:
		CreateHelperPMD() : PMDModel() {}
		~CreateHelperPMD(){}
	};

	auto helper = std::make_shared<CreateHelperPMD>();
	return std::move(helper);
}

