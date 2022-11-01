#pragma once
#include "pch.h"
#include "FSFile.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "Importer.h"

FSResource::FSResource(std::string name, std::string path,ResourceType type) {

	mName = name;
	mPath = path;
	mType = type;

}

void FSResource::Load() {

	lock.lock();
	bool mr = mLoaded;
	lock.unlock();
	if (mr) return;
	lock.lock();
	switch (mType) {
	case ResourceType::TextureFlat:
	{
		auto tex = new Texture2D(mPath.c_str(), true);

		mResource = (void*)tex;
		//mResource = new Texture2D(mPath,false);

	}
	break;
	case ResourceType::ModelProp:
	{
		auto imp = new Importer;
		
		auto model = imp->ImportAI(mPath.c_str());

		mResource = (void*)model;

	}
		break;
	}
	mLoaded = true;
	lock.unlock();
	
}

void* FSResource::GetResource() {

	return mResource;

}

std::string FSResource::GetName() {

	return mName;

}

bool FSResource::Loaded() {


	lock.lock();
	
	bool r = mLoaded;

	lock.unlock();
	return r;

}