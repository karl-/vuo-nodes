/**
 * @file
 * co.parabox.mesh.get node implementation.
 *
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Make UV Line Segments",
					 "keywords" : [ "opengl", "scenegraph", "graphics", "vertices", "vertex", "triangles", "winding", "break" ],
					 "version" : "2.0.0",
					 "description": "Given the texture and element arrays of a mesh, this produces a 2d point list of the texture coordinates in a line segment format.",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
	VuoInputData(VuoList_VuoPoint2d) textures,
	VuoInputData(VuoList_VuoInteger) elements,
	VuoOutputData(VuoList_VuoPoint2d) lineSegments
)
{
	unsigned int vertexCount = VuoListGetCount_VuoPoint2d(textures);

	if(vertexCount < 1)
		return;

	///@todo use VuoListCreateWithCount when available
	*lineSegments = VuoListCreate_VuoPoint2d();

	// first index and last index only get one entry
	VuoListAppendValue_VuoPoint2d(*lineSegments, VuoListGetValue_VuoPoint2d(textures, VuoListGetValue_VuoInteger(elements, 1)));

	for(unsigned int i = 2; i < VuoListGetCount_VuoInteger(elements); i++)
	{
		int index = VuoListGetValue_VuoInteger(elements, i);
		VuoPoint2d coord = VuoListGetValue_VuoPoint2d(textures, index);

		VuoListAppendValue_VuoPoint2d(*lineSegments, coord);
		VuoListAppendValue_VuoPoint2d(*lineSegments, coord);
	}

	VuoListAppendValue_VuoPoint2d(*lineSegments, VuoListGetValue_VuoPoint2d(textures, VuoListGetValue_VuoInteger(elements, VuoListGetCount_VuoInteger(elements))));
}
