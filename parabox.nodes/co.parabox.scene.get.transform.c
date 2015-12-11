/**
 * @file
 * co.parabox.scene.get.transform node implementation.
 *
 * @copyright
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Get 3d Object Local Transform",
					 "keywords" : [ "world", "model", "space" ],
					 "version" : "1.0.0",
					 "description": "",
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
