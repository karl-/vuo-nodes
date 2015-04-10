/**
 * @file
 * co.parabox.event.drop node implementation.
 *
 * @copyright Copyright © 2012–2014 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Drop Events",
					 "keywords" : [ "the mic", "restrict", "bounce", "flow" ],
					 "version" : "1.0.0",
					 "description": "Passes a limited number events.",
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

VuoInteger * nodeInstanceInit(
	VuoInputData(VuoInteger) samplesPerEvent
	)
{
	VuoInteger* count = (VuoInteger*)malloc(sizeof(VuoInteger));
	VuoRegister(count, free);

	*count = 0;

	return count;
}

void nodeInstanceEvent
(
		VuoInstanceData(VuoInteger*) count,
		VuoInputData(VuoGenericType1, {"eventBlocking":"door", "hasPortAction":true}) value,
		VuoInputData(VuoInteger, {"default":1, "suggestedMin":1, "suggestedMax":32}) samplesPerEvent,
		VuoOutputData(VuoGenericType1) output
)
{
	(**count)++;

	if( (**count) > samplesPerEvent )
	{
		*output = value;
		(**count) = 0;
	}
}

void nodeInstanceFini(VuoInstanceData(VuoInteger*) instance)
{
}
