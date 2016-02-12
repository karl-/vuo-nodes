/**
 * @file
 * co.parabox.layer.make.material node implementation.
 */

#include "node.h"
#include "VuoLayer.h"
#include "VuoMesh.h"

VuoModuleMetadata({	"title" : "Make Layer with Material",
					"keywords" : [ "quad", "sprite", "billboard", "plane", "material" ],
					"version" : "1.0.0",
					"genericTypes" :
					{
						"VuoGenericType1" : {
							"compatibleTypes" :
							[
								"VuoShader",
								"VuoImage",
								"VuoColor"
							]
						}
					},
					"description":"Make a Layer using a material (shader, color, or texture)."
				 });

void nodeEvent
(
	VuoInputData(VuoGenericType1, {"defaults":{"VuoColor":{"r":1,"g":1,"b":1,"a":1}}}) material,
	VuoInputData(VuoTransform2d) transform,
	VuoOutputData(VuoLayer) quad
)
{

	VuoSceneObject so = VuoSceneObject_make(
		VuoMesh_makeQuadWithoutNormals(),
		VuoShader_make_VuoGenericType1(material),
		VuoTransform_makeEuler(
			VuoPoint3d_make(transform.translation.x, transform.translation.y, 0.f),
			VuoPoint3d_make(0.f, transform.rotation, 0.f),
			VuoPoint3d_make(transform.scale.x, transform.scale.y, 1.f)),
		NULL );

	quad->sceneObject = so;
}
