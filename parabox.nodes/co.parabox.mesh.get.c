/**
 * @file
 * co.parabox.mesh.get node implementation.
 *
 * @copyright Copyright © 2012–2014 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Get Mesh Values",
					 "keywords" : [ "opengl", "scenegraph", "graphics", "vertices", "vertex", "triangles", "winding", "break" ],
					 "version" : "2.0.0",
					 "description": "Get the data that makes up this mesh.  If the object has multiple submeshes, they will be concatenated into one big ol' list of vertex soup.",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

// @todo vuo 1.3 will change this to VuoSubmesh_download
void VuoSubmeshMesh_download(VuoSubmesh* submesh) __attribute__ ((weak));

// @todo better way to allocate arrays?
void GetMeshValues(	const VuoSceneObject* object,
					unsigned int vertexOffset,
					VuoList_VuoPoint3d *positions,
					VuoList_VuoPoint3d *normals,
					VuoList_VuoPoint3d *tangents,
					VuoList_VuoPoint3d *bitangents,
					VuoList_VuoPoint2d *textures,
					VuoList_VuoInteger *elements)
{
	unsigned int curIndex = vertexOffset;

	if(object != NULL && object->mesh != NULL)
	{
		VuoMesh mesh = object->mesh;

		for(unsigned int CurSubmesh = 0; CurSubmesh < mesh->submeshCount; CurSubmesh++)
		{
			VuoSubmesh* submesh = &mesh->submeshes[CurSubmesh];

			if(submesh == NULL)
				continue;

			// if a geometry shader was used to transform the mesh positions can be null.
			if( submesh->positions == NULL )
				VuoSubmeshMesh_download(submesh);

			for(unsigned int CurIndex = 0; CurIndex < submesh->vertexCount; CurIndex++)
			{
				VuoPoint4d v = submesh->positions[CurIndex];
				VuoListAppendValue_VuoPoint3d(*positions, (VuoPoint3d) {v.x, v.y, v.z} );

				v = submesh->normals != NULL ? submesh->normals[CurIndex] : VuoPoint4d_make(0,0,0,0);
				VuoListAppendValue_VuoPoint3d(*normals, (VuoPoint3d) {v.x, v.y, v.z});

				v = submesh->tangents != NULL ? submesh->tangents[CurIndex] : VuoPoint4d_make(0,0,0,0);
				VuoListAppendValue_VuoPoint3d(*tangents, (VuoPoint3d) {v.x, v.y, v.z});

				v = submesh->bitangents != NULL ? submesh->bitangents[CurIndex] : VuoPoint4d_make(0,0,0,0);
				VuoListAppendValue_VuoPoint3d(*bitangents, (VuoPoint3d) {v.x, v.y, v.z});

				v = submesh->textureCoordinates != NULL ? submesh->textureCoordinates[CurIndex] : VuoPoint4d_make(0,0,0,0);
				VuoListAppendValue_VuoPoint2d(*textures, (VuoPoint2d) {v.x, v.y});
			}

			for(unsigned int CurElement = 0; CurElement < submesh->elementCount; CurElement++)
			{
				VuoListAppendValue_VuoInteger(*elements, curIndex + submesh->elements[CurElement]);
			}

			curIndex += submesh->vertexCount;
		}
	}

	for(unsigned int i = 0; i < VuoListGetCount_VuoSceneObject(object->childObjects); i++)
	{
		VuoSceneObject obj = VuoListGetValue_VuoSceneObject(object->childObjects, i+1);
		GetMeshValues(&obj, curIndex, positions, normals, tangents, bitangents, textures, elements);
	}
}

//void VuoSubmeshMesh_download(VuoSubmesh *submesh)
//{
//	VuoSubmesh_download(submesh);
//}

static void transformPoints(VuoList_VuoPoint3d* points, float* matrix)
{
	unsigned long len = VuoListGetCount_VuoPoint3d(*points);
	VuoPoint3d* array = VuoListGetData_VuoPoint3d(*points);

	for(unsigned long i = 0; i < len; i++)
		array[i] = VuoTransform_transformPoint(matrix, array[i]);
}

static void transformDirections(VuoList_VuoPoint3d* points, const VuoPoint4d rotation)
{
	unsigned long len = VuoListGetCount_VuoPoint3d(*points);
	VuoPoint3d* array = VuoListGetData_VuoPoint3d(*points);

	for(unsigned long i = 0; i < len; i++)
		array[i] = VuoTransform_rotateVectorWithQuaternion(array[i], rotation);
}

void nodeEvent
(
		VuoInputData(VuoSceneObject) object,
		VuoInputData(VuoBoolean, {"default":false}) applyTransform,
		VuoOutputData(VuoList_VuoPoint3d) positions,
		VuoOutputData(VuoList_VuoInteger) elements,
		VuoOutputData(VuoList_VuoPoint3d) normals,
		VuoOutputData(VuoList_VuoPoint3d) tangents,
		VuoOutputData(VuoList_VuoPoint3d) bitangents,
		VuoOutputData(VuoList_VuoPoint2d) textures
)
{
	*positions 	= VuoListCreate_VuoPoint3d();
	*normals 	= VuoListCreate_VuoPoint3d();
	*tangents 	= VuoListCreate_VuoPoint3d();
	*bitangents = VuoListCreate_VuoPoint3d();
	*textures 	= VuoListCreate_VuoPoint2d();

	GetMeshValues(&object, 0, positions, normals, tangents, bitangents, textures, elements);

	if(applyTransform)
	{
		VuoTransform transform = object.transform;

		float matrix[16];
		VuoTransform_getMatrix(transform, matrix);
		VuoPoint4d rotation = VuoTransform_getQuaternion(transform);

		transformPoints(positions, matrix);
		transformDirections(normals, rotation);
		transformDirections(tangents, rotation);
		transformDirections(bitangents, rotation);
	}
}
