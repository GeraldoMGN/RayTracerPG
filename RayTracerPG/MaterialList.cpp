#include "MaterialList.h"

MaterialList::MaterialList(){}

Material * MaterialList::getMaterial(std::string name)
{
	for (int i = 0; i < materials.size(); i++) {
		if (materials.at(i)->getName() == name) {
			return materials.at(i);
		}
	}
}

bool MaterialList::nameUsed(std::string name)
{
	for (int i = 0; i < materials.size(); i++) {
		if (materials.at(i)->getName() == name) {
			return true;
		}
	}
	return false;
}

bool MaterialList::addMaterial(Material* material)
{
	if (!nameUsed(material->getName())) {
		materials.push_back(material);
		return true;
	}
	else {
		return false;
	}
}

