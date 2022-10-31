#include "pch.h"
#include "FSPayload.h"


FSPayload::FSPayload(std::string name)
{

	mName = name;

}

void FSPayload::AddFile(std::string name,std::string path, ResourceType type) {

	auto new_res = new FSResource(name, path, type);

	mResources.push_back(new_res);

}


void FSPayload::Load() {

	for (int i = 0; i < mResources.size(); i++)
	{

		auto res = mResources[i];
		res->Load();

	}

}

void th_load(FSPayload* pl) {

//	printf("Loading resources in resource thread\n");

	auto resources = pl->GetResources();

	for (int i = 0; i < resources.size(); i++) {
		auto res = resources[i];
		//printf("Loading Resource:");
	//	printf(res->GetName().c_str());
	//	printf("\n");
	
		res->Load();
	//	printf("Loaded Resource");

	}
	printf("Payload complete.\n");

}

void FSPayload::LoadThreaded() {

	load_thread = std::thread(&th_load,this);
	load_thread.detach();

}

void* FSPayload::GetResource(std::string name) {

	for (int i = 0; i < mResources.size(); i++)
	{
		auto res = mResources[i];
		if (name == res->GetName()) {

			return res->GetResource();

		}
	}
	return nullptr;
}

FSResource* FSPayload::GetResourceObj(std::string name) {

	for (int i = 0; i < mResources.size(); i++)
	{
		auto res = mResources[i];
		if (name == res->GetName()) {

			return res;
		}
	}
	return nullptr;
}

std::vector<FSResource*> FSPayload::GetResources() {

	return mResources;

}

bool FSPayload::Loaded() {

	int num = (int)mResources.size();

	for (int i = 0; i < mResources.size(); i++) {

		if(mResources[i]->Loaded())
		{ 
			num--;
		}

	}
	if (num == 0) {
		return true;
	}
	return false;

}

float FSPayload::LoadedPercent() {

	int num = 0;
	for (int i = 0; i < mResources.size(); i++)
	{
		if (mResources[i]->Loaded())
		{
			num++;
		}
	}

	float ln, tn;

	ln = (float)num;
	tn = (float)mResources.size();

	return ln / tn;

}