/**
 * @file
 * co.parabox.coordinate.pixels.vuo node implementation.
 *
 */

#include "node.h"
#include <stdio.h>
#include <time.h>

VuoModuleMetadata({
"title" : "Convert Pixels to Vuo Coordinates",
"keywords" : [ "gl", "point", "translate" ],
"version" : "1.0.0",
"genericTypes" : {
	"VuoGenericType1" : {
		"compatibleTypes" : [ "VuoReal", "VuoPoint2d" ]
	}
},
"description": "Convert pixels to Vuo coordinates.",
"node" : {}
});

static VuoReal VuoReal_convertToVuoCoords(int w, int h, VuoReal val)
{

}

void nodeEvent
(
	VuoInputData(VuoWindowReference) window,
	VuoInputData(VuoGenericType1) pixels,
	VuoOutputData(VuoGenericType1) vuoCoordinates
)
{
	*vuoCoordinates = VuoGenericType1_convertToVuoCoords(pixels);
}
