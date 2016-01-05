/**
 * @file
 * co.parabox.scene.get.transform node implementation.
 *
 * @copyright
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Get 3d Object Transform",
					 "keywords" : [ "world", "model", "space", "local" ],
					 "version" : "1.0.0",
					 "description": "Get a scene object's local transform.  Note that this is not affected by parent transforms.",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });


void nodeEvent
(
		VuoInputData(VuoSceneObject) sceneObject,
		VuoOutputData(VuoTransform) transform
)
{
	*transform = sceneObject.transform;
}
