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
					 "title" : "Make Normals Line Mesh",
					 "keywords" : [ "opengl", "scenegraph", "graphics", "vertices", "vertex", "triangles", "winding", "break" ],
					 "version" : "2.0.0",
					 "description": "Given the vertex and normal arrays of a mesh, this produces a 2d point list of the texture coordinates in a line segment format.  The normals are also set.",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

static inline VuoPoint4d addAndMultiply(VuoPoint3d v, VuoPoint3d n, VuoReal d)
{
	return VuoPoint4d_make(	v.x + n.x * d,
							v.y + n.y * d,
							v.z + n.z * d,
							1);
}

void nodeEvent
(
	VuoInputData(VuoList_VuoPoint3d) positions,
	VuoInputData(VuoList_VuoPoint3d) normals,
	VuoInputData(VuoReal, {"default":0.1, "suggestedMin":0.01, "suggestedMax":1}) length,
	VuoOutputData(VuoMesh) lineMesh
)
{
	unsigned int vertexCount = VuoListGetCount_VuoPoint3d(positions);
	unsigned int normalCount = VuoListGetCount_VuoPoint3d(normals);

	if(vertexCount < 1 || vertexCount != normalCount)
		return;

	VuoPoint4d* m_positions = (VuoPoint4d*) malloc(sizeof(VuoPoint4d) * vertexCount * 2);
	VuoPoint4d* m_normals  = (VuoPoint4d*) malloc(sizeof(VuoPoint4d) * vertexCount * 2);
	unsigned int* m_elements = (unsigned int*) malloc(sizeof(unsigned int) * vertexCount * 2);

	for(unsigned int i = 0; i < vertexCount; i++)
	{
		VuoPoint3d v = VuoListGetValue_VuoPoint3d(positions, i+1);
		VuoPoint3d n = VuoListGetValue_VuoPoint3d(normals, i+1);

		m_positions[i*2] = VuoPoint4d_make(v.x, v.y, v.z, 1);
		m_positions[i*2+1] =  addAndMultiply(v, n, length);

		m_normals[i*2] = (VuoPoint4d) { n.x, n.y, n.z, 1 };
		m_normals[i*2+1] =  (VuoPoint4d) { n.x, n.y, n.z, 1 };

		m_elements[i*2] = i*2;
		m_elements[i*2+1] = i*2+1;
	}

	// build the mesh!
	VuoSubmesh submesh;

	submesh.vertexCount = vertexCount * 2;
	submesh.positions = m_positions;
	submesh.elements = m_elements;
	submesh.normals = m_normals;
	submesh.tangents = NULL;
	submesh.bitangents = NULL;
	submesh.textureCoordinates = NULL;
	submesh.elementCount = vertexCount * 2;
	submesh.elementAssemblyMethod = VuoMesh_IndividualLines;
	submesh.faceCullingMode = GL_NONE;

	*lineMesh = VuoMesh_makeFromSingleSubmesh(submesh);

}
