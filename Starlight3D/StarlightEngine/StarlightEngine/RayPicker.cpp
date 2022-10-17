#include "pch.h"

#include "RayPicker.h"
RayPicker::RayPicker(SceneGraph* graph) {

	mGraph = graph;

}

PickResult RayPicker::MousePickNode(int x, int y, int w, int h, NodeEntity* entity, NodeCamera* cam) {

	float mx = -1 + (float)(x) / (float)(w) * 2;
	float my = 1 - (float)(y) / (float)(h) * 2;

	float3 origin = float3(mx, my, 0);
	float3 dest = float3(mx, my, 1);

	float4x4 viewProj = cam->GetWorldMatrix().Inverse() * cam->GetProjectionMatrix();

	float4x4 inverseProj = viewProj.Inverse();

	float3 rayorigin = origin * inverseProj;
	float3 rayend = dest * inverseProj;

	float3 rayDir = normalize(rayend - rayorigin);


	rpRay ray;

	ray.pos = cam->GetPosition();
	ray.dir = rayDir;

	meshes.clear();
	for (int i = 0; i < entity->GetMeshes().size(); i++)
	{
		meshes.push_back(entity->GetMesh(i));
	}
	auto result = RayPickNoChange(ray);

	return result;

}

PickResult RayPicker::MousePick(int x, int y, int w, int h, NodeCamera* cam) {

	float mx = -1 + (float)(x) / (float)(w) * 2;
	float my = 1 - (float)(y) / (float)(h) * 2;

	float3 origin = float3(mx, my, 0);
	float3 dest = float3(mx, my, 1);

	float4x4 viewProj = cam->GetWorldMatrix().Inverse() * cam->GetProjectionMatrix();

	float4x4 inverseProj = viewProj.Inverse();

	float3 rayorigin = origin * inverseProj;
	float3 rayend = dest * inverseProj;

	float3 rayDir = normalize(rayend - rayorigin);




	rpRay ray;

	ray.pos = cam->GetPosition();
	ray.dir = rayDir;

	auto result = RayPick(ray);

	return result;

}

PickResult RayPicker::RayPick(rpRay ray,Node3D* node)
{
	int start = clock();
	mIgnore = node;

	auto result =  RayPick(ray);

	if (result.hit) {

		float xd, yd, zd;

		xd = result.hit_point.x - ray.pos.x;
		yd = result.hit_point.y - ray.pos.y;
		zd = result.hit_point.z - ray.pos.z;
		float dist = sqrt(xd * xd + yd * yd + zd * zd);
		float dir_dist = sqrt(ray.dir.x * ray.dir.x + ray.dir.y * ray.dir.y + ray.dir.z * ray.dir.z);
		if (dist > dir_dist) {
			result.hit = false;
		}


	

	}
	int end = clock();
//	printf("RP took:%d ms\n", end - start);
	return result;
}


void RayPicker::addMeshes(Node3D* e) {
	if (e == mIgnore) {
		return;
	}
	if (e->GetType() == NodeType::Entity) {
		NodeEntity* ee = (NodeEntity*)e;
		for (int i = 0; i < ee->GetMeshes().size(); i++)
		{
			meshes.push_back(ee->GetMesh(i));
		}
	}
	for (int i = 0; i < e->ChildrenCount(); i++) {
		addMeshes(e->GetChild(i));
	}

}
PickResult RayPicker::RayPickNoChange(rpRay ray) {
	return RayPickMesh(ray);
}
PickResult RayPicker::RayPick(rpRay ray)
{

	//PickResult result = RayPickNode(ray, mGraph->GetRoot());
	meshes.clear();

	addMeshes(mGraph->GetRoot());

	return RayPickMesh(ray);

}
const float EPSILON = 0.0000001;
float3 edge1, edge2, h, s, q;
float a, f, u, v;
inline PickResult RayPicker::RayToTri(rpRay& ray,rpTri& tri)// float3& vertex0, float3& vertex1, float3& vertex2)
{

	PickResult result;
	result.hit = false;

	/*
	bool RayIntersectsTriangle(Vector3D rayOrigin,
		Vector3D rayVector,
		Triangle * inTriangle,
		Vector3D & outIntersectionPoint)
	*/

	
		
		

	
	edge1 = tri.v1 - tri.v0;// vertex1 - vertex0;
		edge2 = tri.v2 - tri.v0;
		h = Diligent::cross(ray.dir,edge2);
		a = Diligent::dot(edge1,h);
		if (a > -EPSILON && a < EPSILON)
			return result;    // This ray is parallel to this triangle.
		f = 1.0 / a;
		s = ray.pos - tri.v0;
		u = f * Diligent::dot(s,h);
		if (u < 0.0 || u > 1.0)
			return result;
		q = Diligent::cross(s,edge1);
		v = f * Diligent::dot(ray.dir,q);
		if (v < 0.0 || u + v > 1.0)
			return result;
		// At this stage we can compute t to find out where the intersection point is on the line.
		float t = f * Diligent::dot(edge2,q);
		
		if (t > EPSILON) // ray intersection
		{
			//outIntersectionPoint = rayOrigin + rayVector * t;
			result.hit = true;
			result.hit_point = ray.pos + ray.dir * t;

			

			return result;
		}
		else // This means that there is a line intersection but not a ray intersection.
			return result;
	
		result.hit = false;
	return result;


}

float NOHIT = -255;

inline float rayBoxIntersect(rpRay& ray,float3& vmin, float3& vmax)
{
	float t[10];
	t[1] = (vmin.x - ray.pos.x) / ray.dir.x;
	t[2] = (vmax.x - ray.pos.x) / ray.dir.x;
	t[3] = (vmin.y - ray.pos.y) / ray.dir.y;
	t[4] = (vmax.y - ray.pos.y) / ray.dir.y;
	t[5] = (vmin.z - ray.pos.z) / ray.dir.z;
	t[6] = (vmax.z - ray.pos.z) / ray.dir.z;
	t[7] = fmax(fmax(fmin(t[1], t[2]), fmin(t[3], t[4])), fmin(t[5], t[6]));
	t[8] = fmin(fmin(fmax(t[1], t[2]), fmax(t[3], t[4])), fmax(t[5], t[6]));
	t[9] = (t[8] < 0 || t[7] > t[8]) ? NOHIT : t[7];
	return t[9];
}

inline PickResult RayPicker::RayPickMesh(rpRay& ray) {

	int start = clock();
	PickResult close_result;

	close_result.hit = false;
	for (int i = 0; i < meshes.size(); i++)
	{

		auto mesh = meshes[i];
	

		int a = 5;
		PickCache cache;

		//restart:
	restart:
		if (caches.count(mesh) == 0)
		{
		
			//printf("!!!!!!");
			auto tris = mesh->GetTris();
			auto verts = mesh->GetVertices();

			auto transform = mesh->GetOwner()->GetWorldMatrix();
			float3 min = float3(99999, 99999, 99999);
			float3 max = float3(-99999, -99999, -99999);
			
			cache.tris.reserve(tris.size());

			int ts = tris.size();

			for (int i = 0; i < ts; i++)
			{

				auto tri = tris[i];
				auto v0 = verts[tri.v0];
				auto v1 = verts[tri.v1];
				auto v2 = verts[tri.v2];



				auto t_pos1 = v0.position * transform;
				auto t_pos2 = v1.position * transform;
				auto t_pos3 = v2.position * transform;

				min.x = fmin(t_pos1.x, min.x);
				min.y = fmin(t_pos1.y, min.y);
				min.z = fmin(t_pos1.z, min.z);

				min.x = fmin(t_pos2.x, min.x);
				min.y = fmin(t_pos2.y, min.y);
				min.z = fmin(t_pos2.z, min.z);

				min.x = fmin(t_pos3.x, min.x);
				min.y = fmin(t_pos3.y, min.y);
				min.z = fmin(t_pos3.z, min.z);


				max.x = fmax(t_pos1.x, max.x);
				max.y = fmax(t_pos1.y, max.y);
				max.z = fmax(t_pos1.z, max.z);

				max.x = fmax(t_pos2.x, max.x);
				max.y = fmax(t_pos2.y, max.y);
				max.z = fmax(t_pos2.z, max.z);

				max.x = fmax(t_pos3.x, max.x);
				max.y = fmax(t_pos3.y, max.y);
				max.z = fmax(t_pos3.z, max.z);

				rpTri ctri;
				ctri.v0 = t_pos1;
				ctri.v1 = t_pos2;
				ctri.v2 = t_pos3;

				cache.tris.push_back(ctri);

			}
			cache.bmin = min;
			cache.bmax = max;
			cache.node = mesh->GetOwner();
				caches[mesh] = cache;


		}

		else {
		
			cache = caches[mesh];
			if (mesh->Changed())
			{
				auto ee = caches.find(mesh);
				cache.tris.clear();
				caches.erase(ee);
				int bb = 0;
				goto restart;
			}

		}

	

		if (rayBoxIntersect(ray, cache.bmin, cache.bmax)!=NOHIT)
		{


			for (int i = 0; i < cache.tris.size(); i++) {

				auto result = RayToTri(ray, cache.tris[i]);

				if (result.hit)
				{
//					printf("HIT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
						if (close_result.hit == false) {
							float xd, yd, zd;
							xd = result.hit_point.x - ray.pos.x;
							yd = result.hit_point.y - ray.pos.y;
							zd = result.hit_point.z - ray.pos.z;
							float dist = sqrt(xd * xd + yd * yd + zd * zd);
							close_result = result;
							close_result.hit = true;
							close_result.hit_distance = dist;
							close_result.hit_point = result.hit_point;
							close_result.hit_node = mesh->GetOwner();
							close_result.hit_entity = (NodeEntity*)mesh->GetOwner();
							close_result.hit_mesh = mesh;


							close_result.hit_distance = dist;
						}
						else {

							float xd, yd, zd;

							xd = result.hit_point.x - ray.pos.x;
							yd = result.hit_point.y - ray.pos.y;
							zd = result.hit_point.z - ray.pos.z;
							float dist = sqrt(xd * xd + yd * yd + zd * zd);
							if (dist < close_result.hit_distance)
							{
								close_result.hit_distance = dist;
								close_result.hit_point = result.hit_point;
								close_result.hit_node = mesh->GetOwner();
								close_result.hit_mesh = mesh;
								close_result.hit_entity = (NodeEntity*)mesh->GetOwner();
							}

							//float dist1 =  



						}
				}

			}
		}
		
	}

	int end = clock();

	//printf("MS====%d\n", end - start);

	return close_result;
}

inline PickResult RayPicker::RayPickNode(rpRay& ray, Node3D* node) {

	/*
	exit(0);
	PickResult close_result;
	close_result.hit = false;

	if (node == mIgnore) {

		int aa = 5;

	}
	else {

		if (node->GetType() == NodeType::Entity) {

			auto entity = (NodeEntity*)node;

			for (int i = 0; i < entity->GetMeshes().size(); i++) {

				//auto result = RayPickMesh(ray, entity->GetMesh(i));

				if (result.hit)
				{
					if (close_result.hit == false) {
						float xd, yd, zd;
						xd = result.hit_point.x - ray.pos.x;
						yd = result.hit_point.y - ray.pos.y;
						zd = result.hit_point.z - ray.pos.z;
						float dist = sqrt(xd * xd + yd * yd + zd * zd);
						close_result = result;
						close_result.hit = true;
						close_result.hit_distance = dist;
						close_result.hit_point = result.hit_point;
						close_result.hit_node = node;
						close_result.hit_entity = (NodeEntity*)node;
						close_result.hit_mesh = entity->GetMesh(i);



						close_result.hit_distance = dist;
					}
					else {

						float xd, yd, zd;

						xd = result.hit_point.x - ray.pos.x;
						yd = result.hit_point.y - ray.pos.y;
						zd = result.hit_point.z - ray.pos.z;
						float dist = sqrt(xd * xd + yd * yd + zd * zd);
						if (dist < close_result.hit_distance)
						{
							close_result.hit_distance = dist;
							close_result.hit_point = result.hit_point;
							close_result.hit_node = node;
							close_result.hit_entity = (NodeEntity*)node;
							close_result.hit_mesh = entity->GetMesh(i);
						}

						//float dist1 =  



					}
				}

			}



		}
	}

	for (int i = 0;i < node->ChildrenCount();i++) {

		auto result = RayPickNode(ray, node->GetChild(i));

		if (result.hit)
		{
			if (close_result.hit == false) {
				float xd, yd, zd;
				xd = result.hit_point.x - ray.pos.x;
				yd = result.hit_point.y - ray.pos.y;
				zd = result.hit_point.z - ray.pos.z;
				float dist = sqrt(xd * xd + yd * yd + zd * zd);
				close_result = result;
				close_result.hit = true;
				close_result.hit_distance = dist;
				close_result.hit_point = result.hit_point;
				close_result.hit_node = node->GetChild(i);
				close_result.hit_entity = (NodeEntity*)node->GetChild(i);
				close_result.hit_mesh = result.hit_mesh;



				close_result.hit_distance = dist;
			}
			else {

				float xd, yd, zd;

				xd = result.hit_point.x - ray.pos.x;
				yd = result.hit_point.y - ray.pos.y;
				zd = result.hit_point.z - ray.pos.z;
				float dist = sqrt(xd * xd + yd * yd + zd * zd);
				if (dist < close_result.hit_distance)
				{
					close_result.hit_distance = dist;
					close_result.hit_point = result.hit_point;
					close_result.hit_node = node->GetChild(i);
					close_result.hit_entity = (NodeEntity*)node->GetChild(i);
					close_result.hit_mesh = result.hit_mesh;

				}

				//float dist1 =  



			}
		}

	

	}

	return close_result;
	*/
return PickResult();
}

void RayPicker::SetGraph(SceneGraph* graph) {

	mGraph = graph;

}