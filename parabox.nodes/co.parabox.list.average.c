/**
 * @file
 * co.parabox.list.average node implementation.
 *
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Rolling Average List",
					 "keywords" : [ "rolling", "mean" ],
					 "version" : "1.0.0",
					 "description": "Maintains a rolling average of each item in a list.",
					 "genericTypes" : {
						"VuoGenericType1" : {
							"compatibleTypes" : [ "VuoReal", "VuoInteger", "VuoPoint2d", "VuoPoint3d", "VuoPoint4d" ]
						}
					},
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

struct ArraySample
{
	VuoGenericType1* samples;
	VuoInteger size;
};

struct nodeInstanceData
{
	struct ArraySample* samples;
	VuoInteger index;				// the index of the next array index to be replaced with new data
	VuoInteger availableSamples;	// the number of lists currently stored
	VuoInteger max;					// the number of lists to keep around for averaging
};

static int maxSamples(struct nodeInstanceData* instance)
{
	int max = instance->availableSamples > 0 ? instance->samples[0].size : 0;

	for(int i = 1; i < instance->availableSamples; i++)
		max = MIN(max, instance->samples[i].size);

	return max;
}

static void freeSamples(struct nodeInstanceData* instance)
{
	if(instance->samples != NULL)
	{
		for(int i = 0; i < instance->availableSamples; i++)
		{
			if( instance->samples[i].samples )
			{
				free(instance->samples[i].samples);
			}
		}

		free(instance->samples);
	}

	instance->index = 0;
	instance->availableSamples = 0;
}

static void resetSamples(struct nodeInstanceData* instance, VuoInteger sampleSize)
{
	freeSamples(instance);
	instance->max = sampleSize;

	instance->samples = (struct ArraySample*)malloc(sizeof(struct ArraySample) * sampleSize);
	instance->index = 0;
	instance->availableSamples = 0;

	for(int i = 0; i < sampleSize; i++)
	{
		instance->samples[i].samples = NULL;
		instance->samples[i].size = 0;
	}
}

/**
 * appends list to the current samples array.
 */
static void addSamples(struct nodeInstanceData* instance, VuoList_VuoGenericType1 list)
{
	unsigned int index = instance->index;
	unsigned int len = VuoListGetCount_VuoGenericType1(list);

	if( instance->samples[index].samples != NULL || instance->samples[index].size != len)
	{
		free(instance->samples[index].samples);
		instance->samples[index].samples = NULL;
	}

	if(instance->samples[index].samples == NULL)
		instance->samples[index].samples = (VuoGenericType1*) malloc(sizeof(VuoGenericType1) * len);

	instance->samples[index].size = len;
	instance->availableSamples = MIN(++instance->availableSamples, instance->max);

	for(int i = 0; i < len; i++)
		instance->samples[index].samples[i] = VuoListGetValue_VuoGenericType1(list, i+1);

	instance->index++;

	if(instance->index > instance->max-1)
		instance->index = 0;
}

static VuoList_VuoGenericType1 getAverage(struct nodeInstanceData* instance)
{
	VuoList_VuoGenericType1 list = VuoListCreate_VuoGenericType1();

	if(instance->availableSamples > 0)
	{
		int max = maxSamples(instance);

		VuoGenericType1* avg = (VuoGenericType1*)malloc(sizeof(VuoGenericType1) * max);

		for(unsigned int i = 0; i < max; i++)
		{
			avg[i] = instance->samples[0].samples[i];
		}

		for(unsigned int n = 1; n < instance->availableSamples; n++)
		{
			for(unsigned int i = 0; i < max; i++)
			{
				avg[i] = VuoGenericType1_add(avg[i], instance->samples[n].samples[i]);
			}
		}

		for(unsigned int i = 0; i < max; i++)
			VuoListAppendValue_VuoGenericType1(list, VuoGenericType1_multiply(avg[i], 1.f / (float) MAX(1, instance->availableSamples)));
	}

	return list;
}

struct nodeInstanceData * nodeInstanceInit(
	VuoInputData(VuoInteger) sampleSize
	)
{
	struct nodeInstanceData * instance = (struct nodeInstanceData *)malloc(sizeof(struct nodeInstanceData));
	VuoRegister(instance, free);

	instance->samples = NULL;
	instance->availableSamples = 0;
	resetSamples(instance, sampleSize);

	return instance;
}

void nodeInstanceEvent
(
		VuoInstanceData(struct nodeInstanceData *) instance,
		VuoInputData(VuoList_VuoGenericType1) list,
		VuoInputData(VuoInteger, {"default":5, "suggestedMin":2, "suggestedMax":20}) sampleSize,
		VuoOutputData(VuoList_VuoGenericType1) averagedList
)
{
	if( sampleSize != (*instance)->max )
		resetSamples( *instance, sampleSize );

	addSamples( *instance, list );

	*averagedList = getAverage(*instance);
}

void nodeInstanceFini(VuoInstanceData(struct nodeInstanceData *) instance)
{
	freeSamples(*instance);
}
