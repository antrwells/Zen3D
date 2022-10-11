#include "pch.h"
#include "FSFile.h"
#include "Texture2D.h"
#include "TextureCube.h"

FSResource::FSResource(std::string name, std::string path,ResourceType type) {

	mName = name;
	mPath = path;
	mType = type;

}

void FSResource::Load() {

	if (mLoaded) return;
	switch (mType) {
	case ResourceType::TextureFlat:
	{
		auto tex = new Texture2D(mPath.c_str(), true);

		mResource = (void*)tex;
		//mResource = new Texture2D(mPath,false);

	}
	break;
	}
	mLoaded = true;
}

void* FSResource::GetResource() {

	return mResource;

}

std::string FSResource::GetName() {

	return mName;

}

bool FSResource::Loaded() {

	return mLoaded;

}