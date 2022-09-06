#pragma once
#include <vector>
#include "DataTypes.h"

//#include "glad/glad.h"
#include "Material.h"

	/// <summary>
	/// A mesh3D is used to define a three dimensional mesh, often used by NodeEntity's to create the overall 3D scene.
	/// </summary>
	class Mesh3D
	{
	public:

		/// <summary>
		/// Creates an empty mesh. Meshes are used as part of a scenegraph/nodes to achieve a real time 3D scene, with lighting and shadows.
		/// </summary>
		Mesh3D();

		void MakeDoubleSided() {

			std::vector<Tri> new_tris;

			for (int i = 0; i < mTris.size(); i++) {

				auto old_tri = mTris[i];

				Tri new_tri;

				new_tri.v0 = old_tri.v0;
				new_tri.v1 = old_tri.v2;
				new_tri.v2 = old_tri.v1;

				new_tris.push_back(new_tri);
				

			}

			for (int i = 0; i < new_tris.size(); i++) {
				mTris.push_back(new_tris[i]);
			}

		}

		Mesh3D* Unwind() {

			int v = 0;

			std::vector<Vertex> verts;
			std::vector<Tri> tris;

			for (int i = 0; i < mTris.size(); i++) {

				Tri tri;

				tri = mTris[i];

				Tri new_tri;


				verts.push_back(mVertices[tri.v0]);
				verts.push_back(mVertices[tri.v1]);
				verts.push_back(mVertices[tri.v2]);
				new_tri.v0 = v;
				new_tri.v1 = v + 1;
				new_tri.v2 = v + 2;
				tris.push_back(new_tri);
				v = v + 3;


			}

			Mesh3D* new_mesh = new Mesh3D;
			new_mesh->SetVertices(verts);
			new_mesh->SetTris(tris);
			new_mesh->SetMaterial(mMaterial);
			new_mesh->GenerateNormals();
			new_mesh->CreateVBO();
			return new_mesh;


		}

		void AddVertex(Vertex vertex,bool reset);
		std::vector<Vertex> GetVertices();

		void AddTri(Tri tri);
		void SetTris(std::vector<Tri> tris) {
			mTris = tris;
		}

		std::vector<Tri> GetTris();

		void SetBoneData(int index, int boneID, float weight) {

			int MAX_BONE_WEIGHTS = 4;

			Vertex vertex = mVertices[index];
			for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
			{
				if (vertex.m_BoneIDS[i] < 0)
				{
					vertex.m_Weights[i] = weight;
					vertex.m_BoneIDS[i] = boneID;
					break;
				}
			}
			mVertices[index] = vertex;

		}

		//VertexBuffer
		void CreateVBO();
		void BindVBO();
		void ReleaseVBO();
		void DrawVBO();
		void UpdateVBO();

		//normals
		void GenerateNormals();
		void GenerateTangents();

		Tri GetTri(int id) {
			return mTris[id];
		}

		int NumTris() {
			return mTris.size();
		}

		int NumVertices() {
			return mVertices.size();
		}

		Vertex GetVertex(int id) {
			return mVertices[id];
		}

		void SetMaterial(Material* material);

		//Material
		void BindMaterial();
		void ReleaseMaterial();

		Material* GetMaterial() {
			return mMaterial;
		}

		void SetVertices(std::vector<Vertex> vertices) {

			mVertices = vertices;

		}

		int GetVertexArray() {
			return 0;
			//return VertexArray;
		}

		int GetVertexBuffer() {
			return 0;
			//	return VertexBuffer;
		}

		void SetVB(int va, int vb) {
			//VertexArray = va;
			//VertexBuffer = vb;
		}

		void CreateIndexBuffer() {

		//	if (IndexBuffer == -1) {
	//			glGenBuffers(1, &IndexBuffer);
		//	}
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
		//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Kinetic::DataTypes::Tri) * mTris.size(), mTris.data(), GL_STATIC_DRAW);


		}

	private:
		
		/// <summary>
		/// Data
		/// </summary>
		std::vector<Vertex> mVertices;
		std::vector<Tri> mTris;


		//Material
		Material* mMaterial;

		//OpenGL
		//GLuint VertexArray = -1;
		//GLuint VertexBuffer = -1;
		//GLuint IndexBuffer = -1;


	};



