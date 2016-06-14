#include "node.h"
#include "VuoLayer.h"

VuoModuleMetadata({
					 "title" : "Assign Name to Layer",
					 "keywords" : [ ],
					 "version" : "1.0.0"
				 });

void nodeEvent
(
		VuoInputData(VuoText) layerName,
		VuoInputData(VuoLayer) layer,
		VuoOutputData(VuoLayer) renamedLayer
)
{
	renamedLayer->sceneObject = VuoSceneObject_copy(layer.sceneObject);
	if(renamedLayer->sceneObject.name != NULL)
		VuoRelease(renamedLayer->sceneObject.name);
	renamedLayer->sceneObject.name = VuoText_make(layerName);
}
