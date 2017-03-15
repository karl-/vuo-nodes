/**
 * @file
 * co.parabox.shader.make.uniform node implementation.
 *
 */


#include "node.h"
#include "PbxShaderUniform.h"

VuoModuleMetadata({
					 "title" : "Make Shader Uniform",
					 "keywords" : [ "opengl", "scenegraph", "graphics", "glsl", "attribute", "fragment", "shader", "pipeline" ],
					 "version" : "1.0.0",
					 "description": "Creates a key value <Text, Value> object that may be passed to a GLSL shader.",
					 "genericTypes" : {
						"VuoGenericType1" : { "compatibleTypes" : [
							 "VuoReal",
							 "VuoInteger",
							 "VuoImage",
							 "VuoPoint2d",
							 "VuoPoint3d",
							 "VuoPoint4d",
							 "VuoColor"
							]
						}
					 },
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
	VuoInputData(VuoText, {"default":"name"}) name,
	VuoInputData(VuoGenericType1) value,
	VuoOutputData(PbxShaderUniform) uniform
)
{
	*uniform = PbxShaderUniform_make_VuoGenericType1(name, value);
}
