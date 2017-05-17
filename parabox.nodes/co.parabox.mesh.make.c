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
	unsigned int m_VertexCount = VuoListGetCount_VuoPoint3d(positions);
	unsigned int m_ElementCount = VuoListGetCount_VuoInteger(elements);

	if(m_VertexCount < 3 || m_ElementCount < 3)
		return;

	VuoPoint4d 	*m_Positions = NULL,
				*m_Normals = NULL,
				*m_Textures = NULL;

	unsigned int* m_Elements = (unsigned int*)malloc(sizeof(unsigned int) * m_ElementCount);

	// Populate positions
	m_Positions = (VuoPoint4d*)malloc(sizeof(VuoPoint4d) * m_VertexCount);

	for(int i = 0; i < m_VertexCount; i++)
	{
		VuoPoint3d v = VuoListGetValue_VuoPoint3d(positions, i+1);
		m_Positions[i] = (VuoPoint4d) {v.x, v.y, v.z, 1.};
	}

	// Elements
	for(int i = 0; i < m_ElementCount; i++)
	{
		m_Elements[i] = VuoListGetValue_VuoInteger(elements, i+1);
	}

	// Optional:

	// Normals
	if( VuoListGetCount_VuoPoint3d(normals) == m_VertexCount )
	{
		m_Normals = (VuoPoint4d*)malloc(sizeof(VuoPoint4d) * m_VertexCount);

		for(int i = 0; i < m_VertexCount; i++)
		{
			VuoPoint3d v = VuoListGetValue_VuoPoint3d(normals, i+1);
			m_Normals[i] = (VuoPoint4d) {v.x, v.y, v.z, 1.};
		}
	}

	if( VuoListGetCount_VuoPoint2d(textures) == m_VertexCount )
	{
		m_Textures = (VuoPoint4d*)malloc(sizeof(VuoPoint4d) * m_VertexCount);

		for(int i = 0; i < m_VertexCount; i++)
		{
			VuoPoint2d v = VuoListGetValue_VuoPoint2d(textures, i+1);
			m_Textures[i] = (VuoPoint4d) {v.x, v.y, 0., 1.};
		}
	}

	VuoSubmesh submesh = VuoSubmesh_makeFromBuffers(m_VertexCount,
													m_Positions,
													m_Normals,
													NULL,
													NULL,
													m_Textures,
													m_ElementCount,
													m_Elements,
													VuoMesh_IndividualTriangles);

	*mesh = VuoMesh_makeFromSingleSubmesh(submesh);
}
