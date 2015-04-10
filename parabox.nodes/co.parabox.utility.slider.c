/**
 * @file
 * co.parabox.file.text node implementation.
 *
 * @copyright Copyright © 2012–2014 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Generic Slider",
					 "keywords" : [ "i/o", "input", "data" ],
					 "version" : "1.0.0",
					 "description": "Slide!.",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
	VuoInputData(VuoInteger, {"default":440, "suggestedMin":0, "suggestedMax":12000, "suggestedStep":10}) value,
	VuoOutputData(VuoInteger) val
)
{
	*val = value;
}
