/**
 * @file
 * co.parabox.mesh.make.triangle node implementation.
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
					 "title" : "Make Triangle Image",
					 "keywords" : [ "portrait", "triangulate" ],
					 "version" : "1.0.0",
					 "description": "",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

static inline int clampi(int value, int min, int max)
{
	return value < min ? min : (value > max ? max : value);
}

#define UINT_TO_FLOAT_COLOR .0039215686;

static void colorAtCoordinate(const unsigned char* pixels,
						const unsigned int width,
						const unsigned int height,
						const unsigned int x,
						const unsigned int y,
						float* r,
						float* g,
						float* b,
						float* a)
{
	const unsigned char* position = &pixels[((4 * width * y)) + (x * 4)];

	*r = ((unsigned int)position[0]) * UINT_TO_FLOAT_COLOR;
	*g = ((unsigned int)position[1]) * UINT_TO_FLOAT_COLOR;
	*b = ((unsigned int)position[2]) * UINT_TO_FLOAT_COLOR;
	*a = ((unsigned int)position[3]) * UINT_TO_FLOAT_COLOR;
}

#undef UINT_FLOAT_TO_COLOR

static VuoPoint4d colorAtPointWithRadius(const unsigned char* pixels,
								const VuoImage image,
								VuoPoint2d samplerCoordinate,
								unsigned int pixelRadius
								)
{
	unsigned int w = image->pixelsWide;
	unsigned int h = image->pixelsHigh;

	float aspect = h/(float)w;
	float 	r, g, b, a, rf = 0.f, gf = 0.f, bf = 0.f, af = 0.f;

	VuoPoint2d pixelCoord = samplerCoordinate;
	pixelCoord.x *= w;
	pixelCoord.y *= h;

	unsigned int sampleCount = 0;

	for(int y = clampi(pixelCoord.y-pixelRadius, 0, h); y <= clampi(pixelCoord.y+pixelRadius, 0, h); y++)
	{
		for(int x = clampi(pixelCoord.x-pixelRadius, 0, w); x <= clampi(pixelCoord.x+pixelRadius, 0, w); x++)
		{
			sampleCount++;

			colorAtCoordinate(pixels, w, h, x, y, &r, &g, &b, &a);

			rf += r;
			gf += g;
			bf += b;
			af += a;
		}
	}

	VuoPoint4d color = VuoPoint4d_make(	rf/(float)sampleCount,
										gf/(float)sampleCount,
										bf/(float)sampleCount,
										af/(float)sampleCount);

	return color;
}

void nodeEvent
(
		VuoInputData(VuoImage) image,
		VuoInputData(VuoInteger, { "default":32 }) subdivisions,
		VuoInputData(VuoInteger, {"default":2, "suggestedMin":0, "suggestedMax":32}) pixelRadius,
		VuoInputData(VuoPoint2d, {"default":{"x":640, "y":480} }) imageSize,
		VuoOutputData(VuoMesh) mesh
)
{
	// copy image pixels to buf
	unsigned char* pixels = VuoImage_copyBuffer(image, GL_RGBA);

	float w = imageSize.x, h = imageSize.y;
	float xsegs = subdivisions;
	float ysegs = (int)((h/w) * xsegs);

	unsigned int i = 0;

	unsigned int vertexCount = xsegs * ysegs * 6;
	VuoPoint4d* v = (VuoPoint4d*)malloc(sizeof(VuoPoint4d) * vertexCount);
	VuoPoint4d* c = (VuoPoint4d*)malloc(sizeof(VuoPoint4d) * vertexCount);	// store color data in tangents channel
	unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * vertexCount);

	for(int y = 0; y < ysegs; y++)
	{
		for(int x = 0; x < xsegs; x++)
		{
			VuoPoint4d v0 = (VuoPoint4d) { (x/xsegs) * w, (y/ysegs) * h, 0, 1 };
			VuoPoint4d v1 = (VuoPoint4d) { ((x+1)/xsegs) * w, (y/ysegs) * h, 0, 1 };
			VuoPoint4d v2 = (VuoPoint4d) { (x/xsegs) * w, ((y+1)/ysegs) * h, 0, 1 };
			VuoPoint4d v3 = (VuoPoint4d) { ((x+1)/xsegs) * w, ((y+1)/ysegs) * h, 0, 1 };

			v[i+0] = v0;
			v[i+1] = v1;
			v[i+2] = v2;

			v[i+3] = v1;
			v[i+4] = v3;
			v[i+5] = v2;

			VuoPoint2d cola_pos = VuoPoint2d_make(
						(v0.x + v1.x + v2.x) * .3333,
						(v0.y + v1.y + v2.y) * .3333 );

			VuoPoint2d colb_pos = VuoPoint2d_make(
						(v3.x + v1.x + v2.x) * .3333,
						(v3.y + v1.y + v2.y) * .3333 );

			VuoPoint4d cola = colorAtPointWithRadius(pixels, image, cola_pos, pixelRadius);
			VuoPoint4d colb = colorAtPointWithRadius(pixels, image, colb_pos, pixelRadius);

			c[i+0] = cola;
			c[i+1] = cola;
			c[i+2] = cola;

			c[i+3] = colb;
			c[i+4] = colb;
			c[i+5] = colb;


			i += 6;
		}
	}

	for(int i = 0; i < vertexCount; i++)
		indices[i] = i;

	free (pixels);

	VuoSubmesh submesh;

	submesh.vertexCount = vertexCount;
	submesh.positions = v;
	submesh.normals = NULL;
	submesh.tangents = c;
	submesh.bitangents = NULL;
	submesh.textureCoordinates = NULL;
	submesh.faceCullingMode = GL_BACK;
	submesh.elementCount			= vertexCount;
	submesh.elements				= indices;
	submesh.elementAssemblyMethod 	= VuoMesh_IndividualTriangles;

	*mesh = VuoMesh_makeFromSingleSubmesh(submesh);
}
