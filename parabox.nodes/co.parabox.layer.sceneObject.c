/**
 * @file
 * co.parabox.layer.sceneObject node implementation.
 */

#include "node.h"
#include "VuoLayer.h"
#include "VuoMesh.h"

VuoModuleMetadata({	"title" : "Convert Layer to 3D Object",
					"keywords" : [ "sneaky", "scene", "2d" ],
					"version" : "1.0.0",
					"description":"Make a 3D object using from a layer."
				 });

void nodeEvent
(
	VuoInputData(VuoLayer) layer,
	VuoOutputData(VuoSceneObject) object
)
{
	*object = VuoSceneObject_copy(layer.sceneObject);
}
