#pragma once
#include "SceneGraph.h"

struct PickResult {

	float3 hit_point;
	float3 hit_normal;
	float3 hit_uv;
	float hit_distance;
	Node3D* hit_node;
	NodeEntity* hit_entity;
	NodeActor* hit_actor;
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

class RayPicker
{
public:

	RayPicker(SceneGraph* graph);

	PickResult RayPick(rpRay ray);
	PickResult MousePick(int x, int y, int w, int h, NodeCamera* cam);
	PickResult RayToTri(rpRay ray, float3 v0, float3 v1, float3 v2);
	PickResult MousePickNode(int x, int y, int w, int h, NodeEntity* entity, NodeCamera* cam);


private:

	PickResult RayPickNode(rpRay ray, Node3D* node);
	PickResult RayPickMesh(rpRay ray, Mesh3D* mesh);

	SceneGraph* mGraph = nullptr;

};

