/**
 * @file
 * co.parabox.shader.make.glsl node implementation.
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
					 "title" : "Make Shader with GLSL",
					 "keywords" : [ "opengl", "scenegraph", "graphics", "fragment", "vertex" ],
					 "version" : "2.0.0",
					 "description": "Creates a Vuo Shader from GLSL source code. \n \nThe following `attribute` variables are provided by the scene renderer to the vertex shader: \n \n- `attribute vec4 position;` — Vertex position \n- `attribute vec4 textureCoordinate;` — UV position \n- `attribute vec4 normal;` — Vertex normal \n- `attribute vec4 tangent;` — Vertex tangent \n- `attribute vec4 bitangent;` — Vertex bitangent \n \nImage and float values may be passed to the fragment shader via the `images` and `floats` list ports.  To access these values in GLSL, use `imageX` and `floatX`, where X is the zero based index. \n \nAs an example, to access the first image in the `images` list in GLSL: \n \n\tuniform sampler2D image0; \n \n The projection and model view matrices are defined as: \n \n- `uniform mat4 projectionMatrix;` \n- `uniform mat4 modelviewMatrix;`",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

struct nodeInstanceData
{
	VuoShader shader;
};

struct nodeInstanceData * nodeInstanceInit(
	VuoInputData(VuoText) vertexShader,
	VuoInputData(VuoText) fragmentShader
	)
{
	struct nodeInstanceData * instance = (struct nodeInstanceData *)malloc(sizeof(struct nodeInstanceData));
	VuoRegister(instance, free);

	instance->shader = VuoShader_make("GLSL Shader");
	VuoRetain(instance->shader);

	VuoShader_addSource(instance->shader, VuoMesh_IndividualTriangles, vertexShader, NULL, fragmentShader);

	return instance;
}

const char* ImageUniform = "image";

void nodeInstanceEvent
(
		VuoInstanceData(struct nodeInstanceData *) instance,
		VuoInputData(VuoText, { "default":
				"#version 120\n\n														\
				uniform mat4 projectionMatrix;\n										\
				uniform mat4 modelviewMatrix;\n											\
				attribute vec4 position;			// vertex position\n				\
				attribute vec4 normal;				// vertex normal\n					\
				attribute vec4 textureCoordinate;	// uv position\n					\
				\n																		\
				varying vec4 vertNormal;\n												\
				varying vec4 texCoord;				// uv coordinate\n					\
				\n																		\
				void main(void)\n														\
				{\n																		\
					texCoord = textureCoordinate;\n										\
					vertNormal = normalize(modelviewMatrix * normal);\n					\
					gl_Position = projectionMatrix * modelviewMatrix * position;\n		\
				}"
			 }) vertexShader,
		VuoInputData(VuoText, {"default":
				"#version 120\n\n																\
				uniform float time;\n															\
				varying vec4 vertNormal;\n														\
				varying vec4 texCoord;\n														\
				void main(void)\n																\
				{\n																				\
					vec3 nrm = abs(vertNormal).xyz;\n											\
					float dp = dot(vec3(1., 0., 0.), nrm);\n									\
					gl_FragColor = vec4(texCoord.xy, (sin(time) * dp) * .5 + .5 , 1.);\n		\
				}"
			}) fragmentShader,
		VuoInputEvent({"eventBlocking":"none","data":"vertexShader"}) vertexShaderEvent,
		VuoInputEvent({"eventBlocking":"none","data":"fragmentShader"}) fragmentShaderEvent,
		VuoInputData(VuoList_VuoImage) images,
		VuoInputData(VuoList_VuoReal) floats,
		VuoInputEvent({"eventBlocking":"none","data":"images"}) imagesEvent,
		VuoInputEvent({"eventBlocking":"none","data":"floats"}) floatsEvent,
		VuoInputData(VuoReal, {"default":0.0}) time,
		VuoOutputData(VuoShader) shader
)
{
	/**
	 *	If the vertex or fragment shader text has changed, reload the shader.
	 *	@todo Maybe verify that the shader has successfully compiled before
	 *	pushing it out?
	 */
	if(fragmentShaderEvent || vertexShaderEvent)
	{
		if((*instance)->shader)
		{
			VuoRelease((*instance)->shader);
		}

		(*instance)->shader = VuoShader_make("GLSL Shader");
		VuoRetain((*instance)->shader);

		VuoShader_addSource((*instance)->shader, VuoMesh_IndividualTriangles, vertexShader, NULL, fragmentShader);
	}

	/**
	 *	Time
	 */
	VuoShader_setUniform_VuoReal((*instance)->shader, "time", time);

	/**
	 *	Load Images
	 */
	if(imagesEvent)
	{
		for(int CurImageIndex = 0; CurImageIndex < VuoListGetCount_VuoImage(images); CurImageIndex++)
		{		
			char imageUniform [8];
			int n = sprintf (imageUniform, "image%u", CurImageIndex);

			VuoShader_setUniform_VuoImage((*instance)->shader, imageUniform, VuoListGetValue_VuoImage(images, CurImageIndex));
		}
	}

	/**
	 *	Set float values
	 */
	if(floatsEvent)
	{
		for(int CurFloatIndex = 0; CurFloatIndex < VuoListGetCount_VuoReal(floats); CurFloatIndex++)
		{		
			char floatUniform [8];
			int n = sprintf (floatUniform, "float%u", CurFloatIndex);

			VuoShader_setUniform_VuoImage((*instance)->shader, floatUniform, VuoListGetValue_VuoImage(images, CurFloatIndex));
		}
	}

	*shader = (*instance)->shader;
}

void nodeInstanceFini(VuoInstanceData(struct nodeInstanceData *) instance)
{
	VuoRelease((*instance)->shader);
}
