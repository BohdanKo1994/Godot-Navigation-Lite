#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include <Godot.hpp>
#include <Engine.hpp>
#include <Geometry.hpp>
#include <Spatial.hpp>
#include <Node.hpp>
#include <SceneTree.hpp>
#include <MeshInstance.hpp>
#include <BoxShape.hpp>
#include <OS.hpp>
#include <CapsuleShape.hpp>
#include <CylinderShape.hpp>
#include <SphereShape.hpp>
#include <ConcavePolygonShape.hpp>
#include <ConvexPolygonShape.hpp>
#include <CubeMesh.hpp>
#include <CapsuleMesh.hpp>
#include <CylinderMesh.hpp>
#include <SphereMesh.hpp>
#include <StaticBody.hpp>
#include <CollisionShape.hpp>

#include "navigation_mesh.h"
#include "tilecache_navmesh.h"
#include "Recast.h"
#include "DetourNavMesh.h"
#include "RecastAlloc.h"
#include "RecastAssert.h"
#include "helpers.h"
#include "navmesh_generator.h"
#include "tilecache_generator.h"


namespace godot {

	class DetourNavigation : public Spatial {
		GODOT_CLASS(DetourNavigation, Spatial)
		enum ParsedGeometryType {
			PARSED_GEOMETRY_STATIC_COLLIDERS = 0,
			PARSED_GEOMETRY_MESH_INSTANCES = 1
			
		};
	private:
		/* geometry_source can be 0 = static bodies, 1 = meshes */
		SETGET(collision_mask, int);
		SETGET(parsed_geometry_type, int);

		SETGET(dynamic_objects, bool);
		SETGET(dynamic_collision_mask, int);


		void collect_mesh_instances(
			Array& geometries,
			std::vector<Ref<Mesh>> *meshes, 
			std::vector<Transform> *transforms, 
			std::vector<AABB> *aabbs,
			std::vector<int64_t> *collision_ids,
			DetourNavigationMesh* navmesh
		);


		void convert_collision_shape(
			CollisionShape* collision_shape, 
			std::vector<Ref<Mesh>>* meshes, 
			std::vector<Transform>* transforms, 
			std::vector<AABB>* aabbs,
			std::vector<int64_t>* collision_ids
		);
		
		std::vector<StaticBody*> *dyn_bodies_to_add = nullptr;
		std::vector<StaticBody*>* static_bodies_to_add = nullptr;
		std::vector<CollisionShape*>* collisions_to_remove = nullptr;
		
	public:
		static void _register_methods();

		DetourNavigation();
		~DetourNavigation();

		void _exit_tree();

		void _on_tree_exiting();

		void _enter_tree();

		void _init();
		void _ready();

		void update_tilecache();

		void add_box_obstacle_to_all(int64_t instance_id, Vector3 position, Vector3 extents, float rotationY);


		void remove_obstacle(CollisionShape* collision_shape);

		void remove_static_object(CollisionShape* collision_shape);


		void _process();

		DetourNavigationMeshCached *create_cached_navmesh(Ref<CachedNavmeshParameters> np);

		DetourNavigationMesh *create_navmesh(Ref<NavmeshParameters> np);

		std::vector<DetourNavigationMesh*> navmeshes;
		std::vector<DetourNavigationMeshCached *> cached_navmeshes;


		void build_navmesh(DetourNavigationMesh *navigation);
		void build_navmesh_cached(DetourNavigationMeshCached* navmesh);
		void _notification(int p_what);

		int process_large_mesh(MeshInstance* mesh_instance, int64_t collision_id, std::vector<Ref<Mesh>>* meshes, std::vector<Transform>* transforms, std::vector<AABB>* aabbs, std::vector<int64_t>* collision_ids);

		void _on_cache_object_added(Variant node);
		void _on_cache_object_removed(Variant node);

		void _on_static_object_added(Variant node);
		void _on_static_object_removed(Variant node);

	};

}
#endif