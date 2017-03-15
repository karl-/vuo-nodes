/**
 * @file
 * co.parabox.image.get.id
 *
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>

VuoModuleMetadata({
					 "title" : "Get Image OpenGL ID",
					 "keywords" : [
						"texture",
					 ],
					 "version" : "1.0.0",
					 "dependencies" : [
					 	"VuoGlContext"
					 ],
					 "node": {
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoImage) image,
		VuoOutputData(VuoInteger) id
)
{
	*id = image->glTextureName;
}
