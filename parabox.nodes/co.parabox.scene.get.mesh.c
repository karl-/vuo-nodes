/**
 * @file
 * co.parabox.scene.get.mesh node implementation.
 *
 * @copyright
 */

#include "node.h"
#include "VuoMesh.h"
#include "VuoSceneObject.h"
#include "VuoList_VuoMesh.h"

VuoModuleMetadata({
					 "title" : "Get Meshes in Scene",
					 "keywords" : [ "world", "model", "space" ],
					 "version" : "1.0.0",
					 "description": "Recursively retreive meshes in a SceneObject.",
					 "dependencies" : [
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void GetMeshesRecursive(const VuoSceneObject* object, VuoList_VuoMesh list)
{
	if(object != NULL && object->mesh != NULL)
	{
		if(object->mesh != NULL)
			VuoListAppendValue_VuoMesh(list, object->mesh);
	}

	int children = VuoListGetCount_VuoSceneObject(object->childObjects);

	for(int i = 0; i < children; i++)
	{
		VuoSceneObject child = VuoListGetValue_VuoSceneObject(object->childObjects, i+1);
		GetMeshesRecursive(&child, list);
	}
}

void nodeEvent
(
		VuoInputData(VuoSceneObject) sceneObject,
		VuoOutputData(VuoList_VuoMesh) meshes
)
{
	VuoList_VuoMesh list = VuoListCreate_VuoMesh();

	GetMeshesRecursive(&sceneObject, *meshes);
}
