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
					 "title" : "Make Mesh with Values",
					 "keywords" : [ "opengl", "scenegraph", "graphics", "vertices", "vertex", "triangles", "winding", "break" ],
					 "version" : "2.0.0",
					 "description": "Create a `VuoMesh` from some combination of vertex positions, triangles, and normals.  Positions and Elements are always required, but normals and textures (UV) may be left empty.",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoList_VuoPoint3d, {"default": [
				{"x":-0.5, "y":-0.5, "z":0},
				{"x": 0.5, "y":-0.5, "z":0},
				{"x": 0.0, "y": 0.5, "z":0},
			] }) positions,
		VuoInputData(VuoList_VuoInteger, {"default": [ 0, 1, 2 ]}) elements,
		VuoInputData(VuoList_VuoPoint3d, {"default": [] }) normals,
		VuoInputData(VuoList_VuoPoint2d, {"default": [
				{"x": 0.0, "y": 0.0},
				{"x": 1.0, "y": 0.0},
				{"x": 0.5, "y": 0.8},
			] }) textures,
		VuoOutputData(VuoMesh) mesh
)
{
	VuoSubmesh submesh;

	unsigned int vertexCount = VuoListGetCount_VuoPoint3d(positions);
	unsigned int elementCount = VuoListGetCount_VuoInteger(elements);

	if(vertexCount < 3 || elementCount < 3) return;

	// These two must always be present
	VuoPoint4d* m_positions = (VuoPoint4d*)malloc(sizeof(VuoPoint4d) * vertexCount);
	unsigned int* m_elements = (unsigned int*)malloc(sizeof(unsigned int) * elementCount);

	// Populate positions
	for(int i = 0; i < vertexCount; i++)
	{
		VuoPoint3d v = VuoListGetValue_VuoPoint3d(positions, i+1);
		m_positions[i] = (VuoPoint4d) {v.x, v.y, v.z, 1.};
	}

	for(int i = 0; i < elementCount; i++)
	{
		m_elements[i] = VuoListGetValue_VuoInteger(elements, i+1);
	}

	submesh.vertexCount = vertexCount;
	submesh.positions = m_positions;

	submesh.elementCount			= elementCount;
	submesh.elements				= m_elements;
	submesh.elementAssemblyMethod 	= VuoMesh_IndividualTriangles;

	// Now the optional stuff:

	if( VuoListGetCount_VuoPoint3d(normals) == vertexCount )
	{
		VuoPoint4d* m_normals = (VuoPoint4d*)malloc(sizeof(VuoPoint4d) * vertexCount);

		for(int i = 0; i < vertexCount; i++)
		{
			VuoPoint3d v = VuoListGetValue_VuoPoint3d(normals, i+1);
			m_normals[i] = (VuoPoint4d) {v.x, v.y, v.z, 1.};
		}

		submesh.normals = m_normals;
	}
	else
	{
		submesh.normals = NULL;
	}

	if( VuoListGetCount_VuoPoint2d(textures) == vertexCount )
	{
		VuoPoint4d* m_textures = (VuoPoint4d*)malloc(sizeof(VuoPoint4d) * vertexCount);

		for(int i = 0; i < vertexCount; i++)
		{
			VuoPoint2d v = VuoListGetValue_VuoPoint2d(textures, i+1);
			m_textures[i] = (VuoPoint4d) {v.x, v.y, 0., 1.};
		}

		submesh.textureCoordinates = m_textures;
	}
	else
	{
		submesh.textureCoordinates	= NULL;
	}


	submesh.tangents			= NULL; // tangents are probably not going to be commonly user set
	submesh.bitangents			= NULL; // ditto;

	submesh.faceCullingMode = GL_BACK;

	*mesh = VuoMesh_makeFromSingleSubmesh(submesh);
}
