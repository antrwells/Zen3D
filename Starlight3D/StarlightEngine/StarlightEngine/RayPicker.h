#pragma once

#include "SceneGraph.h"
#include <vector>
#include <map>

struct PickResult {

	float3 hit_point;
	float3 hit_normal;
	float3 hit_uv;
	float hit_distance;
	Node3D* hit_node = nullptr;
	NodeEntity* hit_entity = nullptr;
	NodeActor* hit_actor = nullptr;
	Mesh3D* hit_mesh;
	bool hit = false;

};

struct rpVertex {
	float3 pos;
	float3 norm;
	float3 uv;
};

struct rpRay
{
	float3 pos;
	float3 dir;
};

struct rpTri {

	float3 v0, v1, v2;

};

struct PickCache {

	std::vector<rpTri> tris;
	float3 bmin;
	float3 bmax;
	Node3D* node = nullptr;

};

class RayPicker
{
public:

	RayPicker(SceneGraph* graph);

	PickResult RayPick(rpRay ray);
	PickResult RayPickNoChange(rpRay ray);
	PickResult RayPick(rpRay ray, Node3D* ignore);
	PickResult MousePick(int x, int y, int w, int h, NodeCamera* cam);
	PickResult RayToTri(rpRay& ray,rpTri& tri);
	PickResult MousePickNode(int x, int y, int w, int h, NodeEntity* entity, NodeCamera* cam);
	void SetGraph(SceneGraph* graph);
	void addMeshes(Node3D* e);

private:

	Node3D* mIgnore;
	PickResult RayPickNode(rpRay& ray, Node3D* node);
	PickResult RayPickMesh(rpRay& ray);

	SceneGraph* mGraph = nullptr;
	std::map<Mesh3D*, PickCache> caches;
	std::vector<Mesh3D*> meshes;

};

