#include "pch.h"
#include "Importer.h"
#include <map>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>
#include "Material.h"
#include "VString.h"

//#include "Animation.h"
//#include "Animator.h"
#include "NodeAnimator.h"


#include "VFile.h"
#include "MeshActor.h"
#include "Common/interface/BasicMath.hpp"


using namespace Diligent;
NodeEntity* cur = NULL;

std::vector<Mesh3D*> meshes;
std::vector<MeshActor*> meshes_actor;
std::vector<Material*> materials;

const char* mpath = "";

const C_STRUCT aiScene* scene = NULL;

const char* GetPath(const char* path) {

	VString base_path = VString(path);

	int bl = base_path.Len();

	int fs = 0;
	int i = 0;
	while (true) {

		if (path[i] == "/"[0] || path[i] == "\\"[0])
		{
			fs = i;
		}
		i++;
		if (i == bl) {
			break;
		}
	}

	VString real_path = base_path.SubString(0, fs + 1);

	return real_path.GetConst();

}

const char* GetTexturePath(const char* path, const char* tex) {

	VString base_path = VString(path);

	int bl = base_path.Len();

	int fs = 0;
	int i = 0;
	while (true) {

		if (path[i] == "/"[0] || path[i]=="\\"[0])
		{
			fs = i;
		}
		i++;
		if (i == bl) {
			break;
		}
	}

	VString real_path = base_path.SubString(0, fs+1);

	real_path.Add(VString(tex));



	return real_path.GetConst();
}

const char* GetNormalTexturePath(const char* path, const char* tex) {

	VString base_path = VString(path);

	int bl = base_path.Len();

	int fs = 0;
	int i = 0;
	while (true) {

		if (path[i] == "/"[0])
		{
			fs = i;
		}
		i++;
		if (i == bl) {
			break;
		}
	}

	VString real_path = base_path.SubString(0, fs + 1);

	real_path.Add("norm_");
	real_path.Add(VString(tex));



	return real_path.GetConst();
}


const char* GetSpecularTexturePath(const char* path, const char* tex) {

	VString base_path = VString(path);

	int bl = base_path.Len();

	int fs = 0;
	int i = 0;
	while (true) {

		if (path[i] == "/"[0])
		{
			fs = i;
		}
		i++;
		if (i == bl) {
			break;
		}
	}

	VString real_path = base_path.SubString(0, fs + 1);

	real_path.Add("spec_");
	real_path.Add(VString(tex));



	return real_path.GetConst();
}


/*
inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
	glm::mat4 to;


	to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
	to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
	to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
	to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

	return to;
}
*/


NodeEntity* importNode(const C_STRUCT aiScene* sc, const C_STRUCT aiNode* nd)
{

	//printf("Importing Node: Meshes:%d\n", nd->mNumMeshes);

	C_STRUCT aiMatrix4x4 m = nd->mTransformation;

	//float4x4 vm = aiMatrix4x4ToGlm(&m);
	//float4 pos = vm


	//vm[3] = glm::vec4(0, 0, 0, 1);



	//vm[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	NodeEntity* new_entity = new NodeEntity;

	//vent->SetRotation(vm);
	//vent->SetPosition(Vect3(pos.x, pos.y, pos.z));

	//VString nn = VString(nd->mName.C_Str());
	//vent->SetName(nn.GetConst());




	for (int i = 0; i < nd->mNumMeshes; i++) {

		Mesh3D*mesh = meshes[nd->mMeshes[i]];
		//printf("Importing Mesh: Verts:%d Tris:%d\n", mesh->mNumVertices, mesh->mNumFaces);
		//vent->AddMesh(vmesh);
		new_entity->AddMesh(mesh);


	}

	/*
	for (; n < nd->mNumMeshes; ++n) {
		const C_STRUCT aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		*/
		//printf("Importing Children. Count:%d\n", nd->mNumChildren);
	for (int i = 0; i < nd->mNumChildren; i++) {

		auto new_entity2 = importNode(sc, nd->mChildren[i]);
		new_entity->AddNode(new_entity2);
		//vent->Add(nent);

	}
	return new_entity;

}




	NodeEntity* Importer::ImportAI(const char* path, bool optimize)
	{

		meshes.resize(0);
		materials.resize(0);


		NodeEntity* root = new NodeEntity;
		printf("Importing scene:");
		printf(path);
		printf("|\n");
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace | aiProcess_FixInfacingNormals | aiProcess_FindDegenerates | aiProcess_FindInvalidData |  aiProcess_Triangulate | aiProcess_ValidateDataStructure);

		if (scene == nullptr)
		{

			printf("Failed to import\n");
			while (true) {

			}

		}
		
		for (int i = 0; i < scene->mNumMaterials; i++) {

			Material* new_material = new Material;

			new_material->SetType(MaterialType::PBR);

			auto aiMaterial = scene->mMaterials[i];

			char* mat_name = (char*)aiMaterial->GetName().C_Str();

			int base_tex_count = aiMaterial->GetTextureCount(aiTextureType::aiTextureType_BASE_COLOR);
			int diff_tex_count = aiMaterial->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);
			int norm_tex_count = aiMaterial->GetTextureCount(aiTextureType::aiTextureType_NORMALS);

			aiString base_path;
			aiString diff_path;
			aiString norm_path;

			if (base_tex_count > 0)
			{
				aiMaterial->GetTexture(aiTextureType_BASE_COLOR, 0, &base_path);
			}
			if (diff_tex_count > 0) {
				aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diff_path);

				//char*)diff_path.C_Str();
				if (VFile::Exists(diff_path.C_Str())) {

					new_material->SetColorMap(new Texture2D(diff_path.C_Str()));

				}
				else {
					char* tex_path = (char*)GetTexturePath(path, diff_path.C_Str());
					if (VFile::Exists(tex_path)) {
						new_material->SetColorMap(new Texture2D(tex_path));
					}
				}


				//int b = 5;
				//new_material->SetColorMap(new Vivid::Textures::Texture2D(tex_path, true));

			}
			if (norm_tex_count > 0) {

				aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &norm_path);

				char* tex_path = (char*)GetTexturePath(path, norm_path.C_Str());

				int b = 5;
				new_material->SetNormalMap(new Texture2D(tex_path));

			}
			else {

				if (diff_tex_count > 200) {

					
					char* tex_path = (char*)GetNormalTexturePath(path, diff_path.C_Str());
					char* spec_path = (char*)GetSpecularTexturePath(path, diff_path.C_Str());
		

					int b = 0;

					new_material->SetNormalMap(new Texture2D(tex_path));
					new_material->SetSpecularMap(new Texture2D(spec_path));


 					int a = 0;

				}

			}

			int a = 5;
			materials.push_back(new_material);

		}

		printf("Imported");


		for (int m = 0; m < scene->mNumMeshes; m++) {

			const C_STRUCT aiMesh* mesh = scene->mMeshes[m];

			Mesh3D* new_mesh = new Mesh3D;

			new_mesh->SetMaterial(materials[mesh->mMaterialIndex]);


			meshes.push_back(new_mesh);

			printf("Mesh Verts:%d Faces:%d\n",(int)mesh->mNumVertices, (int)mesh->mNumFaces);

			for (int index = 0; index < mesh->mNumVertices; index++) {

				auto vertex = mesh->mVertices[index];
				auto normal = mesh->mNormals[index];
				aiVector3D bi_normal;

				if (mesh->mBitangents != NULL) {
					bi_normal = mesh->mBitangents[index];
				}

				aiVector3D tangent;

				if (mesh->mTangents != NULL) {
					tangent = mesh->mTangents[index];
				}



				auto uv = mesh->mTextureCoords[0][index];
				aiColor4D color;
				color.r = 1;
				color.g = 1;
				color.b = 1;
				color.a = 1;
				if (mesh->HasVertexColors(0)) {
					color = mesh->mColors[0][index];
				}

				Vertex new_vertex;
				new_vertex.position = float3(vertex.x, vertex.z, vertex.y);
				new_vertex.normal = float3(normal.x, normal.z, normal .y);
				new_vertex.bi_normal = float3(bi_normal.x, bi_normal.z, bi_normal.y);

				new_vertex.tangent = float3(tangent.x, tangent.z, tangent.y);

				new_vertex.texture_coord = float3(uv.x, uv.y, uv.z);

				new_vertex.color = float4(color.r, color.g, color.b, color.a);

				new_mesh->AddVertex(new_vertex,true);

				//vmesh->SetVertex(v, nvert);

				//printf("X:%f Y:%f Z:%f \n", vert.x, vert.y, vert.z);
			}


			for (int tri = 0; tri < mesh->mNumFaces; tri++) {

				auto face = mesh->mFaces[tri];

				if (face.mNumIndices == 3) {

					Tri new_tri;

					new_tri.v0 = face.mIndices[0];
					new_tri.v1 = face.mIndices[1];
					new_tri.v2 = face.mIndices[2];

					new_mesh->AddTri(new_tri);

					//vmesh->SetTri(t, vtri);
				}
			}
			//new_mesh->GenerateNormals();
			new_mesh->MakeDoubleSided();
			new_mesh->CreateBuffers();


		}




		/*
		for (int m = 0; m < scene->mNumMeshes; m++) {

			const C_STRUCT aiMesh* mesh = scene->mMeshes[m];


			auto vmat = mats[mesh->mMaterialIndex];



			Vivid::Meshes::Mesh3D* new_mesh = new Vivid::Meshes::Mesh3D;

			//vmesh->SetMaterial(vmat);

			meshes.push_back(new_mesh);

			for (int v = 0; v < mesh->mNumVertices; v++) {

				auto vert = mesh->mVertices[v];
				auto norm = mesh->mNormals[v];
				aiVector3D binorm;

				if (mesh->mBitangents != NULL) {
					binorm = mesh->mBitangents[v];
				}

				aiVector3D tan;

				if (mesh->mTangents != NULL) {
					tan = mesh->mTangents[v];
				}



				auto uv = mesh->mTextureCoords[0][v];
				aiColor4D col;
				col.r = 1;
				col.g = 1;
				col.b = 1;
				col.a = 1;
				if (mesh->HasVertexColors(0)) {
					col = mesh->mColors[0][v];
				}

				Vertex nvert;
				nvert.Pos = Vect3(vert.x, vert.z, vert.y);
				nvert.Norm = Vect3(norm.x, norm.z, norm.y);
				nvert.BiNorm = Vect3(binorm.x, binorm.z, binorm.y);
				nvert.Tan = Vect3(tan.x, tan.z, tan.y);
				nvert.UV = Vect3(uv.x, uv.y, uv.z);
				nvert.Col = Vect4(col.r, col.g, col.b, col.a);

				vmesh->SetVertex(v, nvert);
				//printf("X:%f Y:%f Z:%f \n", vert.x, vert.y, vert.z);
			}


			for (int t = 0; t < mesh->mNumFaces; t++) {

				auto face = mesh->mFaces[t];

				if (face.mNumIndices == 3) {

					Tri vtri;
					vtri.V0 = face.mIndices[0];
					vtri.V1 = face.mIndices[1];
					vtri.V2 = face.mIndices[2];

					vmesh->SetTri(t, vtri);
				}
			}

			vmesh->CreateVBO();

		}
		*/
		root = importNode(scene, scene->mRootNode);

		//printf("Import complete.\n");

		aiReleaseImport(scene);


		return root;
		return nullptr;
	}

	

	NodeActor* Importer::ImportActor(const char* path) {

		meshes.resize(0);
		materials.resize(0);


		NodeActor* root = new NodeActor;
		printf("Importing scene:");
		printf(path);
		printf("|\n");
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace | aiProcess_FixInfacingNormals | aiProcess_FindDegenerates | aiProcess_FindInvalidData | aiProcess_Triangulate | aiProcess_ValidateDataStructure);

		if (scene == nullptr)
		{

			printf("Failed to import\n");
			while (true) {

			}

		}

		for (int i = 0; i < scene->mNumMaterials; i++) {

			Material* new_material = new Material;

			new_material->SetType(MaterialType::AnimPBR);

			auto aiMaterial = scene->mMaterials[i];

			char* mat_name = (char*)aiMaterial->GetName().C_Str();

			int base_tex_count = aiMaterial->GetTextureCount(aiTextureType::aiTextureType_BASE_COLOR);
			int diff_tex_count = aiMaterial->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);
			int norm_tex_count = aiMaterial->GetTextureCount(aiTextureType::aiTextureType_NORMALS);

			aiString base_path;
			aiString diff_path;
			aiString norm_path;

			if (base_tex_count > 0)
			{
				aiMaterial->GetTexture(aiTextureType_BASE_COLOR, 0, &base_path);
			}
			if (diff_tex_count > 0) {
				aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diff_path);

				char* tex_path = (char*)GetTexturePath(path, diff_path.C_Str());

				int b = 5;
				new_material->SetColorMap(new Texture2D(tex_path));

			}
			if (norm_tex_count > 0) {

				aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &norm_path);

				char* tex_path = (char*)GetTexturePath(path, norm_path.C_Str());

				int b = 5;
				new_material->SetNormalMap(new Texture2D(tex_path));

			}
			else {

				if (diff_tex_count > 0) {


					char* tex_path = (char*)GetNormalTexturePath(path, diff_path.C_Str());



					int b = 0;

					new_material->SetNormalMap(new Texture2D(tex_path));

					int a = 0;

				}

			}

			int a = 5;
			materials.push_back(new_material);

		}

		printf("Imported");


		for (int m = 0; m < scene->mNumMeshes; m++) {

			const C_STRUCT aiMesh* mesh = scene->mMeshes[m];

			MeshActor* new_mesh = new MeshActor;

			new_mesh->SetMaterial(materials[mesh->mMaterialIndex]);


			meshes_actor.push_back(new_mesh);

			printf("Mesh Verts:%d Faces:%d\n", (int)mesh->mNumVertices, (int)mesh->mNumFaces);

			for (int index = 0; index < mesh->mNumVertices; index++) {

				auto vertex = mesh->mVertices[index];
				auto normal = mesh->mNormals[index];
				aiVector3D bi_normal;

				if (mesh->mBitangents != NULL) {
					bi_normal = mesh->mBitangents[index];
				}

				aiVector3D tangent;

				if (mesh->mTangents != NULL) {
					tangent = mesh->mTangents[index];
				}



				auto uv = mesh->mTextureCoords[0][index];
				aiColor4D color;
				color.r = 1;
				color.g = 1;
				color.b = 1;
				color.a = 1;
				if (mesh->HasVertexColors(0)) {
					color = mesh->mColors[0][index];
				}

				VertexActor new_vertex;
				new_vertex.position = float3(vertex.x, vertex.y,vertex.z);
				new_vertex.normal = float3(normal.x, normal.y, normal.z);
				new_vertex.bi_normal = float3(bi_normal.x, bi_normal.y, bi_normal.z);

				new_vertex.tangent = float3(tangent.x, tangent.y, tangent.z);

				new_vertex.texture_coord = float3(uv.x, uv.y, uv.z);									 

				new_vertex.color = float4(color.r, color.g, color.b, color.a);

				new_mesh->AddVertex(new_vertex, true);

				//vmesh->SetVertex(v, nvert);

				//printf("X:%f Y:%f Z:%f \n", vert.x, vert.y, vert.z);
			}


			std::map<std::string, BoneInfo> m_BoneInfoMap;
			int m_BoneCounter = 0;

			int max_verts = new_mesh->GetVertices().size();

			for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
			{

				int boneID = -1;
				std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
				if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end()) {

					int v = 0;
					BoneInfo newBoneInfo;
					newBoneInfo.id = m_BoneCounter;
					newBoneInfo.offset = aiMatrix4x4ToGlm(&mesh->mBones[boneIndex]->mOffsetMatrix);
					m_BoneInfoMap[boneName] = newBoneInfo;
					boneID = m_BoneCounter;
					m_BoneCounter++;

				}
				else {
					boneID = m_BoneInfoMap[boneName].id;
				}
				assert(boneID != -1);

				auto weights = mesh->mBones[boneIndex]->mWeights;
				int numWeights = mesh->mBones[boneIndex]->mNumWeights;

				for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
				{
					int vertexId = weights[weightIndex].mVertexId;
					float weight = weights[weightIndex].mWeight;
					assert(vertexId <= max_verts);


					new_mesh->SetBoneData(vertexId, boneID, weight);
				}


			}

			root->SetBoneInfoMap(m_BoneInfoMap, m_BoneCounter);



			for (int tri = 0; tri < mesh->mNumFaces; tri++) {

				auto face = mesh->mFaces[tri];

				if (face.mNumIndices == 3) {

					Tri new_tri;

					new_tri.v0 = face.mIndices[0];
					new_tri.v1 = face.mIndices[1];
					new_tri.v2 = face.mIndices[2];

					new_mesh->AddTri(new_tri);

					//vmesh->SetTri(t, vtri);
				}
			}

			new_mesh->CreateBuffers();
			root->SetMeshActor(new_mesh);

		}

		NodeAnimator* node_anim = new NodeAnimator;


		Animation* anim = new Animation((aiScene*)scene, root);
		Animator* animer = new Animator(anim);



		//node_anim->mAnimation = anim;
		//node_anim->mAnimtor = animer;

		root->SetAnimator(animer);



		aiReleaseImport(scene);


		return root;



	}

