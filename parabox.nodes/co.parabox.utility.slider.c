/**
 * @file
 * co.parabox.utility.slider node implementation.
 *
 */

#include "node.h"
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Generic Slider",
					 "keywords" : [ "normalize", "gizmo", "widget", "ui" ],
					 "version" : "1.0.0",
					 "description": "A slider input from 0-1 that outputs a scaled value.",
					  "genericTypes" : {
						 "VuoGenericType1" : {
							 "compatibleTypes" : [ "VuoReal", "VuoInteger" ]
						 }
					 },
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
	VuoInputData(VuoReal, {"default":1, "suggestedMin":0, "suggestedMax":1}) value,
	VuoInputData(VuoGenericType1) minimum,
	VuoInputData(VuoGenericType1) maximum,
	VuoOutputData(VuoGenericType1) output
)
{
	float clamp = fmin(fmax(value, 0), 1);
	*output = (clamp * (maximum-minimum)) + minimum;
}
