#include "node.h"
#include "VuoImageRenderer.h"
#include <OpenGL/CGLMacro.h>

VuoModuleMetadata({
					  "title" : "Make Graph",
					  "keywords" : [ "waveform", "audio", "clipart" ],
					  "version" : "1.0.0",
					  "description": "Creates a graph image from a list of real values.\n\nIf maxValue is greater than 0, the the Y axis of the graph is plotted from 0 to maxValue, otherwise the largest point in the list is used.",
					  "node": {
					  }
				 });

static const char *vertexShader = VUOSHADER_GLSL_SOURCE(120,
	uniform mat4 projectionMatrix;
	uniform mat4 modelviewMatrix;
	attribute vec4 position;

	void main()
	{
		gl_Position = projectionMatrix * modelviewMatrix * position;
	}
);

static const char * barGraphFragmentShader = VUOSHADER_GLSL_SOURCE(120,

	varying vec4 fragmentTextureCoordinate;
	uniform vec4 color;
	uniform sampler2D data;

	void main(void)
	{
		vec2 pos = fragmentTextureCoordinate.xy;
		vec4 amplitude = texture2D(data, vec2(pos.x, .5));
		gl_FragColor = abs(pos.y) < amplitude.a ? color : vec4(0.);
	}
);

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

	instance->glContext = VuoGlContext_use();

	instance->imageRenderer = VuoImageRenderer_make(instance->glContext);
	VuoRetain(instance->imageRenderer);
	
	instance->shader = VuoShader_make("Bar Graph Shader");
	VuoShader_addSource(instance->shader, VuoMesh_IndividualTriangles, NULL, NULL, barGraphFragmentShader);
	VuoRetain(instance->shader);

	return instance;
}

void nodeInstanceEvent
(
		VuoInstanceData(struct nodeInstanceData *) instance,
		VuoInputData(VuoList_VuoReal) samples,
		VuoInputData(VuoColor, {"default":{"r":0.2, "g":0.2, "b":0.9, "a":1}}) color,
		VuoInputData(VuoInteger, {"default":0, "suggestedMin":0, "suggestedMax":1}) chartType,
		VuoInputData(VuoReal, {"default":1}) maxValue,
		VuoInputData(VuoInteger, {"default":640, "suggestedMin":1, "suggestedStep":32}) width,
		VuoInputData(VuoInteger, {"default":480, "suggestedMin":1, "suggestedStep":32}) height,
		VuoOutputData(VuoImage) image
)
{
	if(samples == NULL || VuoListGetCount_VuoReal(samples) < 1)
		return;

	int len = VuoListGetCount_VuoReal(samples);
	unsigned char* pixels = (unsigned char*)malloc(sizeof(char)*len*4);

	float largest = maxValue;

	if(largest < 0)
	{
		VuoListGetValue_VuoReal(samples, 1);
		for(int i = 2; i <= len; i++) largest = MAX(largest, VuoListGetValue_VuoReal(samples, i));
	}

	int n = 0;
	for(int i = 0; i < len; i++)
	{
		float amp = VuoListGetValue_VuoReal(samples, i+1);

		pixels[n++] = (unsigned int)255;
		pixels[n++] = (unsigned int)255;
		pixels[n++] = (unsigned int)255;
		pixels[n++] = (unsigned int)((amp/largest)*255);
	}

	VuoImage gradientStrip = VuoImage_makeFromBuffer(pixels, GL_RGBA, len, 1, VuoImageColorDepth_8);

	if(chartType == 0)
	{
		CGLContextObj cgl_ctx = (CGLContextObj)VuoGlContext_use();
		glBindTexture(GL_TEXTURE_2D, gradientStrip->glTextureName);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		VuoGlContext_disuse(cgl_ctx);
	}

	VuoShader_setUniform_VuoColor((*instance)->shader, "color", color);
	VuoShader_setUniform_VuoImage((*instance)->shader, "data", gradientStrip);

	// Render.
	*image = VuoImageRenderer_draw((*instance)->imageRenderer, (*instance)->shader, width, height, VuoImageColorDepth_8);
}

void nodeInstanceFini(VuoInstanceData(struct nodeInstanceData *) instance)
{
	VuoRelease((*instance)->shader);
	VuoRelease((*instance)->imageRenderer);
	VuoGlContext_disuse((*instance)->glContext);
}
