/**
 * @file
 * co.parabox.mesh.make.normals node implementation.
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
					 "title" : "Make Normals Line Segments",
					 "keywords" : [ "opengl", "scenegraph", "graphics", "vertices", "vertex", "triangles", "winding", "break" ],
					 "version" : "2.0.0",
					 "description": "Given the vertex and normal arrays of a mesh, this produces a 2d point list of the texture coordinates in a line segment format.",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
	VuoInputData(VuoList_VuoPoint3d) positions,
	VuoInputData(VuoList_VuoPoint3d) normals,
	VuoInputData(VuoReal, {"default":0.1, "suggestedMin":0.01, "suggestedMax":1}) length,
	VuoOutputData(VuoList_VuoPoint3d) lineSegments
)
{
	unsigned int vertexCount = VuoListGetCount_VuoPoint3d(positions);
	unsigned int normalCount = VuoListGetCount_VuoPoint3d(normals);

	if(vertexCount < 1 || vertexCount != normalCount)
		return;

	///@todo use VuoListCreateWithCount when available
	*lineSegments = VuoListCreate_VuoPoint3d();

	for(unsigned int i = 0; i < VuoListGetCount_VuoPoint3d(positions); i++)
	{
		VuoPoint3d v = VuoListGetValue_VuoPoint3d(positions, i+1);
		VuoPoint3d n = VuoListGetValue_VuoPoint3d(normals, i+1);

		VuoListAppendValue_VuoPoint3d(*lineSegments, v);
		VuoListAppendValue_VuoPoint3d(*lineSegments, VuoPoint3d_add(v, VuoPoint3d_multiply(n, length)));
	}
}
