/**
 * @file
 * co.parabox.sceneObject.layer node implementation.
 */

#include "node.h"
#include "VuoLayer.h"
#include "VuoMesh.h"

VuoModuleMetadata({	"title" : "Convert 3D Object to Layer",
					"keywords" : [ "sneaky", "scene", "2d" ],
					"version" : "1.0.0",
					"description":"Make a Layer using a 3d object (layers are secretely 3d objects anyways, this just puts them in the layer queue which forgoes projection and other 3d stuff)."
				 });

void nodeEvent
(
	VuoInputData(VuoSceneObject) object,
	VuoOutputData(VuoLayer) layer
)
{
	layer->sceneObject = object;
}
