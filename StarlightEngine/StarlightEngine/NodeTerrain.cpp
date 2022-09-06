#include "pch.h"
#include "NodeTerrain.h"
#include <vector>
//#include "PBTerrain.h"
//#include "PerlinNoise.hpp"


/*
namespace Kinetic::Graph::Nodes::Terrain {

	int GetV(int x, int y, int w)
	{

		return y * w + x;

	}
	int grid[21024][21024];
	NodeTerrain::NodeTerrain(Kinetic::Textures::Texture2D* map, float x_segs, float y_segs, float x_Size, float y_size, float y_scale) {


		

		for (int x = 0; x < 21024; x++) {
			for (int y = 0; y < 21024; y++) {
				grid[x][y] = -1;
			}
		}


		float x_move = x_Size / x_segs;
		float y_move = y_size / y_segs;

		Kinetic::Meshes::Mesh3D* geo_mesh = new Kinetic::Meshes::Mesh3D();

		int tx, ty;

		tx = 0;
		ty = 0;
		int vn = 0;

		for (float y = 0; y < y_size+1; y += y_move) {
			tx = 0;
			for (float x = 0; x < x_Size+1; x += x_move) {
				
				float ax = x / x_Size;
				float ay = y / y_size;

				float aw = map->GetWidth();
				float ah = map->GetHeight();

				unsigned char* da = map->GetData();

				float xrv = aw * ax;
				float yrv = ah * ay;
				yrv = yrv - 1;
				if (yrv < 0) yrv = 0;

				int loc = (yrv * map->GetWidth() * 3) + (xrv * 3);

				float yp = da[loc];

				yp = yp / 255.0f;

				yp = yp * y_scale;


				float x_point = -x_Size / 2.0 + (x_Size * ax);
				float y_point = -y_size / 2.0 + (y_size * ay);
				grid[tx][ty] = vn;
 

				//printf("AX:%f AY:%f\n", ax, ay);
				//printf("PX:%f PY:%f\n", x_point, y_point);

				Kinetic::DataTypes::Vertex* vert = new Kinetic::DataTypes::Vertex;

				float tex_u = ay;
				float tex_v = ay;


				vert->position = glm::vec3(x_point, yp, y_point);
				vert->texture_coord = glm::vec3(tex_u, tex_v, 0);
				vert->color = glm::vec4(0, 1, 0, 1);
				vert->normal = glm::vec3(0, 1, 0);
				//printf("X:%f Y:%f Z:%f\n", x_point,y_point, z_point);
				geo_mesh->AddVertex(*vert,false);
				vn++;
				//int a = 0;

				tx++;
			}
			ty++;
		}

		int tris = 0;

//		tx = 0;
	//	ty = 0;
		for (int y = 0; y < ty - 1; y++) {
			for (int x = 0; x < tx - 1; x++) {

				if (grid[x + 1][y + 1] != -1)
				{

					Kinetic::DataTypes::Tri* tri = new Kinetic::DataTypes::Tri;
					tri->v0 = grid[x][y];
					tri->v1 = grid[x + 1][y];
					tri->v2 = grid[x + 1][y + 1];

					geo_mesh->AddTri(*tri);

					tri = new Kinetic::DataTypes::Tri;

					tri->v0 = grid[x + 1][y + 1];
					tri->v1 = grid[x][y + 1];
					tri->v2 = grid[x][y];
					geo_mesh->AddTri(*tri);

					geo_mesh->AddTri(*tri);
					//tris++;

				}

			}
		}

		printf("Tris:%d\n", tris);
		geo_mesh->GenerateNormals();
		geo_mesh->CreateVBO();
		AddMesh(geo_mesh);

		int b = 0;

	}


    NodeTerrain::NodeTerrain(Kinetic::Textures::Texture2D* map,float width,float height,float yScale) {

		int map_width, map_height;

		map_width = map->GetWidth();
		map_height = map->GetHeight();
		unsigned char* data = map->GetData();

		Kinetic::Meshes::Mesh3D* geo_mesh = new Kinetic::Meshes::Mesh3D();

		std::vector<std::vector<Kinetic::DataTypes::Vertex*>> points;

		float rW = map_width;
		float rH = map_height;

		for (int y = 0; y < map_height; y++) {
			std::vector<Kinetic::DataTypes::Vertex* > point_line;
			for (int x = 0; x < map_width; x++) {

				int loc = (y * map_width * 3);
				loc = loc + (x * 3);

				unsigned int tex_height = (unsigned int)data[loc];
				//	printf("Height:%d\n", tex_height);

				Kinetic::DataTypes::Vertex* vert = new Kinetic::DataTypes::Vertex;

				float rX = x;
				float rY = y;

				float x_point = rX / rW;
				float z_point = rY / rH;

				float tex_u = x_point * 15;
				float tex_v = z_point * 15;

				x_point = (-width / 2.0f) + (width * x_point);
				z_point = (-height / 2.0f) + (height * z_point);

				float rTex = (float)tex_height;

				rTex = rTex / 255.0f;

				float y_point = yScale * rTex;


				vert->position = glm::vec3(x_point, y_point, z_point);
				vert->texture_coord = glm::vec3(tex_u, tex_v, 0);
				vert->color = glm::vec4(0, 1, 0, 1);
				vert->normal = glm::vec3(0, 1, 0);
				rTex = rTex * 2.0;
				if (rTex > 1.0) {
					rTex = 1.0;
				}
				vert->m_Weights[0] = rTex;
				//printf("X:%f Y:%f Z:%f\n", x_point,y_point, z_point);
				geo_mesh->AddVertex(*vert, false);
				point_line.push_back(vert);
			}
			points.push_back(point_line);

		}

		int a = 0;

		Kinetic::DataTypes::Vertex* v1, * v2, * v3, * v4;



		for (int y = 0; y < map_height-1; y++) {
			for (int x = 0; x < map_width-1; x++) {

			
				Kinetic::DataTypes::Tri* tri = new Kinetic::DataTypes::Tri;
				tri->v0 = GetV(x, y, map_width);
				tri->v1 = GetV(x + 1, y, map_width);
				tri->v2 = GetV(x + 1, y + 1, map_width);

				geo_mesh->AddTri(*tri);

				tri = new Kinetic::DataTypes::Tri;

				tri->v0 = GetV(x + 1, y + 1, map_width);
				tri->v1 = GetV(x, y + 1, map_width);
				tri->v2 = GetV(x, y, map_width);

				geo_mesh->AddTri(*tri);


			}
		}
		geo_mesh->GenerateNormals();
		geo_mesh->CreateVBO();
		AddMesh(geo_mesh);
		geo_mesh->GetMaterial()->SetType(Kinetic::Material::MaterialType::Terrain);

	}

	void NodeTerrain::SetPhysicsTerrain() {

		std::vector<float> heights;



		mBody = new PBTerrain(heights, 1, 1);

	}

	NodeTerrain::NodeTerrain(int seedval,  float width, float height, float yScale)
	{
		int map_width, map_height;

		//map_width = map->GetWidth();
		//map_height = map->GetHeight();
		map_width = 1024;
		map_height = 1024;

		const siv::PerlinNoise::seed_type seed = 123456u;

		const siv::PerlinNoise perlin{ seed };


	//	unsigned char* data = map->GetData();

		Kinetic::Meshes::Mesh3D* geo_mesh = new Kinetic::Meshes::Mesh3D();

		std::vector<std::vector<Kinetic::DataTypes::Vertex*>> points;

		float rW = map_width;
		float rH = map_height;

		for (int y = 0; y < map_height; y++) {
			std::vector<Kinetic::DataTypes::Vertex* > point_line;
			for (int x = 0; x < map_width; x++) {

				int loc = (y * map_width * 3);
				loc = loc + (x * 3);

				//unsigned int tex_height = (unsigned int)data[loc];

				const double noise = perlin.octave2D_01((x * 0.005), (y * 0.005), 2);
			//	printf("noise:%f\n",(float)noise);

				unsigned int tex_height = (unsigned int)(255.0f * (float)noise);

				//	printf("Height:%d\n", tex_height);

				Kinetic::DataTypes::Vertex* vert = new Kinetic::DataTypes::Vertex;

				float rX = x;
				float rY = y;

				float x_point = rX / rW;
				float z_point = rY / rH;

				float tex_u = x_point * 15;
				float tex_v = z_point * 15;

				x_point = (-width / 2.0f) + (width * x_point);
				z_point = (-height / 2.0f) + (height * z_point);

				float rTex = (float)tex_height;

				rTex = rTex / 255.0f;

				float y_point = yScale * rTex;


				vert->position = glm::vec3(x_point, y_point, z_point);
				vert->texture_coord = glm::vec3(tex_u, tex_v, 0);
				vert->color = glm::vec4(0, 1, 0, 1);
				vert->normal = glm::vec3(0, 1, 0);
				rTex = rTex * 2.0;
				if (rTex > 1.0) {
					rTex = 1.0;
				}
				vert->m_Weights[0] = rTex;
				//printf("X:%f Y:%f Z:%f\n", x_point,y_point, z_point);
				geo_mesh->AddVertex(*vert, false);
				point_line.push_back(vert);
			}
			points.push_back(point_line);

		}

		int a = 0;

		Kinetic::DataTypes::Vertex* v1, * v2, * v3, * v4;



		for (int y = 0; y < map_height - 1; y++) {
			for (int x = 0; x < map_width - 1; x++) {


				Kinetic::DataTypes::Tri* tri = new Kinetic::DataTypes::Tri;
				tri->v0 = GetV(x, y, map_width);
				tri->v1 = GetV(x + 1, y, map_width);
				tri->v2 = GetV(x + 1, y + 1, map_width);

				geo_mesh->AddTri(*tri);

				tri = new Kinetic::DataTypes::Tri;

				tri->v0 = GetV(x + 1, y + 1, map_width);
				tri->v1 = GetV(x, y + 1, map_width);
				tri->v2 = GetV(x, y, map_width);

				geo_mesh->AddTri(*tri);


			}
		}
		geo_mesh->GenerateNormals();
		geo_mesh->CreateVBO();
		AddMesh(geo_mesh);
		geo_mesh->GetMaterial()->SetType(Kinetic::Material::MaterialType::Terrain);
	}

}

*/