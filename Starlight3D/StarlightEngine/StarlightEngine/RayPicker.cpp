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

	auto result = RayPickNode(ray, entity);

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
	return result;
}

PickResult RayPicker::RayPick(rpRay ray)
{

	PickResult result = RayPickNode(ray, mGraph->GetRoot());

	return result;

}

PickResult RayPicker::RayToTri(rpRay ray, float3 v0, float3 v1, float3 v2)
{

	PickResult result;
	result.hit = false;

	/*
	bool RayIntersectsTriangle(Vector3D rayOrigin,
		Vector3D rayVector,
		Triangle * inTriangle,
		Vector3D & outIntersectionPoint)
	*/

		const float EPSILON = 0.0000001;
		
		float3 vertex0 = v0;
		float3 vertex1 = v1;
		float3 vertex2 = v2;

		float3 edge1, edge2, h, s, q;
		float a, f, u, v;
		edge1 = vertex1 - vertex0;
		edge2 = vertex2 - vertex0;
		h = Diligent::cross(ray.dir,edge2);
		a = Diligent::dot(edge1,h);
		if (a > -EPSILON && a < EPSILON)
			return result;    // This ray is parallel to this triangle.
		f = 1.0 / a;
		s = ray.pos - vertex0;
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

PickResult RayPicker::RayPickMesh(rpRay ray, Mesh3D* mesh) {

	int a = 5;

	auto tris = mesh->GetTris();
	auto verts = mesh->GetVertices();

	auto transform = mesh->GetOwner()->GetWorldMatrix();

	PickResult close_result;

	close_result.hit = false;

	for (int i = 0; i < tris.size(); i++) {

		auto tri = tris[i];

		auto v0 = verts[tri.v0];
		auto v1 = verts[tri.v1];
		auto v2 = verts[tri.v2];

		auto pos1 = v0.position;
		auto pos2 = v1.position;
		auto pos3 = v2.position;

		auto t_pos1 = pos1 * transform;
		auto t_pos2 = pos2 * transform;
		auto t_pos3 = pos3 * transform;


		auto result = RayToTri(ray, t_pos1, t_pos2, t_pos3);

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

	return close_result;
}

PickResult RayPicker::RayPickNode(rpRay ray, Node3D* node) {

	PickResult close_result;
	close_result.hit = false;

	if (node == mIgnore) {

		int aa = 5;

	}
	else {

		if (node->GetType() == NodeType::Entity) {

			auto entity = (NodeEntity*)node;

			for (int i = 0; i < entity->GetMeshes().size(); i++) {

				auto result = RayPickMesh(ray, entity->GetMesh(i));

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

}

void RayPicker::SetGraph(SceneGraph* graph) {

	mGraph = graph;

}