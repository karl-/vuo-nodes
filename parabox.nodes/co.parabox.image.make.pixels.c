/**
 * @file
 * co.parabox.make.image.pixels
 *
 * pixels[,] -> image
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>

VuoModuleMetadata({
					 "title" : "Make Image from Pixels",
					 "keywords" : [
					 	"texture", "array", "channels", "rgba"
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
		VuoInputData(VuoList_VuoColor) rgba,
		VuoInputData(VuoInteger) pixelsWide,
		VuoInputData(VuoInteger) pixelsHigh,
		VuoOutputData(VuoImage) image
)
{
	unsigned long size = VuoListGetCount_VuoColor(rgba);
	VuoColor* rgba_ptr = VuoListGetData_VuoColor(rgba);
	VuoColor* pixels = (VuoColor*) malloc(sizeof(VuoColor) * size);
	memcpy(pixels, rgba_ptr, size * sizeof(VuoColor));

	*image = VuoImage_makeFromBuffer(pixels, GL_RGBA, pixelsWide, pixelsHigh, VuoImageColorDepth_32, ^(void* pixels) { free(pixels); });
}
