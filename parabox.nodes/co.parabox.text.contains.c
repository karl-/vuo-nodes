/**
 * @file
 * co.parabox.event.drop node implementation.
 *
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Text Contains",
					 "keywords" : [ "has", "search", "str", "string" ],
					 "version" : "1.0.0",
					 "description": "Returns true if `text` contains `value`, false otherwise.",
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoText) text,
		VuoInputData(VuoText) value,
		VuoOutputData(VuoBoolean) contains
)
{
	if(text == NULL || value == NULL)
		*contains = false;
	else
		*contains = strstr(text, value) != NULL;
}
