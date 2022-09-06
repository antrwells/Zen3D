#include "pch.h"
#include "Material.h"

#include <string>

	
	Texture2D* Material::BlankColor = nullptr;
	Texture2D* Material::BlankNormal = nullptr;
	Texture2D* Material::BlankSpecular = nullptr;

	std::vector<float4x4> Material::m_FinalBoneMatrices;


	Material::Material() {

		mType = MaterialType::PBR;

		if (BlankColor == nullptr) {

			BlankColor = new Texture2D("engine/maps/blankcolor.png");
			BlankNormal = new Texture2D("engine/maps/blanknormal.png");


		}

	
		
		mNormalMap = BlankNormal;
		mColorMap = BlankColor;
		mSpecularMap = BlankColor;

		mDiffuse = float3(1, 1, 1);
		mSpecular = float3(1, 1, 1);
		mAmbient = float3(0.2, 0.2, 0.2);

	}

	void Material::SetType(MaterialType type) {

		mType = type;

	}

	void Material::BindTextures() {
		if (mColorMap != nullptr) {

			//mColorMap->Bind(0);



		}

		if (mNormalMap != nullptr) {
			//mNormalMap->Bind(1);


		}

		if (mSpecularMap != nullptr) {
			//mSpecularMap->Bind(3);
		}

	}

	void Material::ReleaseTextures() {

		if (mColorMap != nullptr) {

		//	mColorMap->Release(0);

		}
		if (mNormalMap != nullptr) {
			//mNormalMap->Release(1);
		}
		if (mSpecularMap != nullptr) {
			//mSpecularMap->Release(3);
		}
	}

	void Material::Bind() {

		
	}

	void Material::Release() {
	
	}

	void Material::SetColorMap(Texture2D* map) {

		mColorMap = map;

	}

	void Material::SetNormalMap(Texture2D* map)
	{

		mNormalMap = map;

	}

	void Material::SetSpecularMap(Texture2D* map) {

		mSpecularMap = map;

	}

	/*
	void Material::SetEnvironmentMap(Kinetic::Textures::TextureCube* map) {

		mEnvironmentMap = map;

	}
	*/

