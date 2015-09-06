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
