/**
 * @file
 * simulateProcessing node implementation.
 */

#include "node.h"
#include "VuoImageRenderer.h"

VuoModuleMetadata({
					  "title" : "Simulate Lengthy Processing",
					  "keywords" : [ "" ],
					  "version" : "1.0.0",
					  "node": { },
					  "dependencies" : []
				 });

struct nodeInstanceData
{
	VuoShader shader;
	VuoGlContext glContext;
	VuoImageRenderer imageRenderer;
};

struct nodeInstanceData * nodeInstanceInit(void)
{
	struct nodeInstanceData * instance = (struct nodeInstanceData *)malloc(sizeof(struct nodeInstanceData));
	VuoRegister(instance, free);

	instance->shader = VuoShader_makeUnlitImageShader(NULL, 1);
	VuoRetain(instance->shader);

	instance->glContext = VuoGlContext_use();

	instance->imageRenderer = VuoImageRenderer_make(instance->glContext);
	VuoRetain(instance->imageRenderer);

	return instance;
}

void nodeInstanceEvent
(
	VuoInstanceData(struct nodeInstanceData*) instance,
	VuoInputData(VuoImage) image,
	VuoInputData(VuoInteger, { "default":100, "suggestedMin":0, "suggestedMax":1000, "suggestedStep":1 }) millisecondsToSleep,
	VuoOutputData(VuoImage) aCopyOfTheImage
)
{
	usleep(millisecondsToSleep * 1000);

	if (!image)
	{
		*aCopyOfTheImage = NULL;
		return;
	}

	VuoShader_setUniform_VuoImage((*instance)->shader, "texture", image);

	*aCopyOfTheImage = VuoImageRenderer_draw((*instance)->imageRenderer, (*instance)->shader, image->pixelsWide, image->pixelsHigh, VuoImage_getColorDepth(image));
}

void nodeInstanceFini(VuoInstanceData(struct nodeInstanceData *) instance)
{
	VuoRelease((*instance)->shader);
	VuoRelease((*instance)->imageRenderer);
	VuoGlContext_disuse((*instance)->glContext);
}
