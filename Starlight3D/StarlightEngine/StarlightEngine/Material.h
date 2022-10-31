#pragma once

#include "Texture2D.h"
#include "TextureCube.h"
#include <vector>



	/// <summary>
	/// Determines how the material renders.
	/// </summary>
	enum MaterialType {

		PBR,Translucent,Reflecxtion,Refraction,FullBright,AnimPBR,Terrain,Phong,PBR2

	};

	/// <summary>
	/// A material defines how a mesh is rendered.
	/// </summary>
	class Material
	{
	public:

		/// <summary>
		/// Constructs an empty material.
		/// </summary>
		Material();

		/// <summary>
		/// Sets the rendering type of the material.
		/// </summary>
		/// <param name="type"></param>
		void SetType(MaterialType type);

		/// <summary>
		/// Bind a material for rendering.
		/// </summary>
		void Bind();
		/// <summary>
		/// Releases a material from rendering.
		/// </summary>
		void Release();

		void BindTextures();
		void ReleaseTextures();

		Texture2D* GetColorMap() {
			return mColorMap;
		}

		Texture2D* GetNormalMap() {
			return mNormalMap;
		}
		
		Texture2D* GetSpecularMap() {
			return mSpecularMap;
		}

		TextureCube* GetEnvMap() {

			return mEnvMap;

		}

		void SetColorMap(Texture2D* map);
		void SetNormalMap(Texture2D* map);
		void SetSpecularMap(Texture2D* map);
		void SetEnvMap(TextureCube* map);



		void SetTerrainTexture(Texture2D* color_map,Texture2D* norm_map, int index) {

			mAuxMap[index] = color_map;
			mAuxMap[index + 1] = norm_map;

		}

		void SetDisplacementMap(Texture2D* displace) {

			mDisplaceMap = displace;

		}


		void SetSpecularStrength(float strength) {

			mSpecularStrength = strength;

		}

		void ClearColorMap();
		void ClearNormalMap();
		void ClearSpecularMap();

		//static Kinetic::FX::Effect* FXPbr;
		//static Kinetic::FX::Effect* FXFullBright;
		//static Kinetic::FX::Effect* FXAnimPbr;
	//	static Kinetic::FX::Effect* FXPbrTerrain;
		//static Kinetic::FX::Effect* FXPhong;


		static std::vector<float4x4> m_FinalBoneMatrices;

		static Texture2D* BlankColor;
		static Texture2D* BlankNormal;
		static Texture2D* BlankSpecular;
		void SetDiffuse(float3 dif);
		void SetSpecular(float3 spec);
		float3 GetDiffuse();
		float3 GetSpecular();

	private:

		//Type
		MaterialType mType = MaterialType::PBR2;

		//colors
		float3 mDiffuse;
		float3 mSpecular;
		float3 mAmbient;
		float mSpecularStrength = 0.3;



		//Surface Maps
		Texture2D* mColorMap = nullptr;
		Texture2D* mAuxMap[8];
		Texture2D* mNormalMap = nullptr;
		Texture2D* mSpecularMap = nullptr;
		TextureCube* mEnvMap = nullptr;
		
		Texture2D* mDisplaceMap = nullptr;
		bool RefractEnv = true;

	};



