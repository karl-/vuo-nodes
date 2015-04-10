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

void nodeEvent
(
		VuoInputData(VuoSceneObject) object,
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

//	for(unsigned int i = 0; i < VuoListGetCount_VuoSceneObject(object.childObjects); i++)
//	{
		VuoSceneObject obj = object;//VuoListGetValue_VuoSceneObject(object.childObjects, i+1);

		VuoMesh mesh = obj.mesh;

		for(unsigned int CurSubmesh = 0; CurSubmesh < mesh->submeshCount; CurSubmesh++)
		{
			VuoSubmesh* submesh = &mesh->submeshes[CurSubmesh];

			for(unsigned int CurIndex = 0; CurIndex < submesh->vertexCount; CurIndex++)
			{
				VuoPoint4d v = submesh->positions[CurIndex];
				VuoListAppendValue_VuoPoint3d(*positions, (VuoPoint3d) {v.x, v.y, v.z} );

				v = submesh->normals[CurIndex];
				VuoListAppendValue_VuoPoint3d(*normals, (VuoPoint3d) {v.x, v.y, v.z});

				v = submesh->tangents[CurIndex];
				VuoListAppendValue_VuoPoint3d(*tangents, (VuoPoint3d) {v.x, v.y, v.z});

				v = submesh->bitangents[CurIndex];
				VuoListAppendValue_VuoPoint3d(*bitangents, (VuoPoint3d) {v.x, v.y, v.z});

				v = submesh->textureCoordinates[CurIndex];
				VuoListAppendValue_VuoPoint2d(*textures, (VuoPoint2d) {v.x, v.y});
			}

			for(unsigned int CurElement = 0; CurElement < submesh->elementCount; CurElement++)
			{
				VuoListAppendValue_VuoInteger(*elements, submesh->elements[CurElement]);
			}
		}
//	}
}
