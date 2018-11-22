#pragma once

#include "Material.h"

class MaterialList
{
private:
	std::vector<Material*> materials;
public:
	MaterialList();
	
	Material* getMaterial(std::string name);
	bool nameUsed(std::string name);
	bool addMaterial(Material* material);
};

