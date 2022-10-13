#pragma once
#include <string>
#include <mutex>

enum ResourceType {


	Font,TextureFlat,TextureCubed,ModelProp,ModelActor,Binary,Sound,Song,Text

};

class FSResource
{
public:

	FSResource(std::string name, std::string path, ResourceType type);
	void Load();
	void* GetResource();
	std::string GetName();
	bool Loaded();
	std::string GetPath() {
		return mPath;
	}
	std::mutex lock;


private:
	std::string mName = "";
	std::string mPath = "";
	ResourceType mType;
	void* mResource = nullptr;
	bool mLoaded = false;
};

