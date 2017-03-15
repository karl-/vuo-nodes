/**
 * @file
 * co.parabox.shader.make.glsl node implementation.
 *
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>
#include <stdio.h>
#include "PbxShaderUniform.h"
#include "VuoList_PbxShaderUniform.h"

VuoModuleMetadata({
"title" : "Make Shader with GLSL",
"keywords" : [ "opengl", "scenegraph", "graphics", "fragment", "vertex" ],
"version" : "2.1.0",
"description": "Creates a Vuo Shader from GLSL source code. \n \nThe following `attribute` variables are provided by the scene renderer to the vertex shader: \n \n- `attribute vec4 position;` — Vertex position \n- `attribute vec4 textureCoordinate;` — UV position \n- `attribute vec4 normal;` — Vertex normal \n- `attribute vec4 tangent;` — Vertex tangent \n- `attribute vec4 bitangent;` — Vertex bitangent \n \nImage and float values may be passed to the fragment shader via the `images` and `floats` list ports.  To access these values in GLSL, use `imageX` and `floatX`, where X is the zero based index. \n \nAs an example, to access the first image in the `images` list in GLSL: \n \n\tuniform sampler2D image0; \n \n The projection and model view matrices are defined as: \n \n- `uniform mat4 projectionMatrix;` \n- `uniform mat4 modelviewMatrix;`",
"dependencies" : [
	    "VuoGlContext",
	    "PbxShaderUniform"
	],
"node" : {
"exampleCompositions" : [ "" ]
	}
});

struct nodeInstanceData
{
	VuoShader shader;
	VuoText curVertexShader;
	VuoText curFragShader;
};

const char *pointGeometryShaderSource = VUOSHADER_GLSL_SOURCE(120, include(trianglePoint));
const char *lineGeometryShaderSource  = VUOSHADER_GLSL_SOURCE(120, include(triangleLine));


struct nodeInstanceData * nodeInstanceInit(
    VuoInputData(VuoText) vertexShader,
	VuoInputData(VuoText) fragmentShader
)
{
	struct nodeInstanceData * instance = (struct nodeInstanceData *)malloc(sizeof(struct nodeInstanceData));
	VuoRegister(instance, free);

	instance->shader = VuoShader_make("GLSL Shader");
	VuoRetain(instance->shader);

	instance->curVertexShader = NULL;
	instance->curFragShader = NULL;

	VuoShader_addSource(instance->shader, VuoMesh_IndividualTriangles, vertexShader, NULL, fragmentShader);
	VuoShader_addSource(instance->shader, VuoMesh_LineStrip, vertexShader, NULL, fragmentShader);
	VuoShader_addSource(instance->shader, VuoMesh_Points, vertexShader, NULL, fragmentShader);

	VuoShader_setExpectedOutputPrimitiveCount(instance->shader, VuoMesh_Points, 2);
	VuoShader_setExpectedOutputPrimitiveCount(instance->shader, VuoMesh_IndividualLines, 2);

	return instance;
}

void nodeInstanceEvent
(
    VuoInstanceData(struct nodeInstanceData *) instance,
	VuoInputData(VuoText, { "default":"\
#version 120\n\
include(VuoGlslProjection)\n\
\n\
// Built-in uniforms and attributes\n\
uniform mat4 modelviewMatrix;\n\
// uniform mat4 modelviewMatrixInverse;\n\
\n\
attribute vec4 position;\n\
// attribute vec4 normal;\n\
// attribute vec4 tangent;\n\
// attribute vec4 bitangent;\n\
attribute vec4 textureCoordinate;\n\
\n\
// Default output\n\
// varying vec4 outPosition;\n\
// varying vec4 outNormal;\n\
// varying vec4 outTangent;\n\
// varying vec4 outBitangent;\n\
 varying vec4 outTextureCoordinate;\n\
\n\
void main(void)\n\
{\n\
	outTextureCoordinate = textureCoordinate;\n \
	gl_Position = VuoGlsl_projectPosition(modelviewMatrix * position);\n\
}\n"
	, "isCodeEditor": true}) vertexShader,
    VuoInputData(VuoText, {"default":
"#version 120\n\
\n\
uniform vec4 color;\n\
uniform sampler2D texture;\n\
\n\
// varying vec4 outPosition;\n\
// varying vec4 outNormal;\n\
// varying vec4 outTangent;\n\
// varying vec4 outBitangent;\n\
varying vec4 outTextureCoordinate;\n\
\n\
void main(void)\n\
{\n\
	gl_FragColor = texture2D(texture, outTextureCoordinate.xy) * color;\n\
}",
                           "isCodeEditor": true
                          }) fragmentShader,
	VuoInputData(VuoList_PbxShaderUniform) uniforms,
    VuoOutputData(VuoShader) shader
)
{
	/**
	 *	If the vertex or fragment shader text has changed, reload the shader.
	 *	@todo Maybe verify that the shader has successfully compiled before
	 *	pushing it out?
	 */
	if ( vertexShader != (*instance)->curVertexShader || fragmentShader != (*instance)->curFragShader )
	{
		if ((*instance)->shader)
		{
			VuoRelease((*instance)->shader);
		}

		(*instance)->shader = VuoShader_make("GLSL Shader");
		VuoRetain((*instance)->shader);

		(*instance)->curVertexShader = vertexShader;
		(*instance)->curFragShader = fragmentShader;

		VuoShader_addSource((*instance)->shader, VuoMesh_IndividualTriangles, vertexShader, NULL, fragmentShader);
		VuoShader_addSource((*instance)->shader, VuoMesh_IndividualLines, vertexShader, NULL, fragmentShader);
		VuoShader_addSource((*instance)->shader, VuoMesh_Points, vertexShader, NULL, fragmentShader);

		VuoShader_setExpectedOutputPrimitiveCount((*instance)->shader, VuoMesh_Points, 2);
		VuoShader_setExpectedOutputPrimitiveCount((*instance)->shader, VuoMesh_IndividualLines, 2);
	}

	for (unsigned int i = 0; i < VuoListGetCount_PbxShaderUniform(uniforms); i++)
	{
		PbxShaderUniform_setUniform((*instance)->shader, VuoListGetValue_PbxShaderUniform(uniforms, i + 1));
	}

	*shader = (*instance)->shader;
}

void nodeInstanceFini(VuoInstanceData(struct nodeInstanceData *) instance)
{
	VuoRelease((*instance)->shader);
}
