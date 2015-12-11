/**
 * @file
 * co.parabox.list.average node implementation.
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
					 "title" : "Average List",
					 "keywords" : [ "rolling", "mean" ],
					 "version" : "1.0.0",
					 "description": "Maintains a rolling average of a list.",
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
		max = MAX(max, instance->samples[i].size);

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

	// @todo realloc or just alloc a bigger space to avoid constant free/malloc
	if( instance->samples[index].samples != NULL )
	{
		free(instance->samples[index].samples);
	}

	instance->samples[index].samples = (VuoGenericType1*) malloc(sizeof(VuoGenericType1) * len);

	instance->samples[index].size = len;
	instance->availableSamples++;

	instance->availableSamples = MIN(instance->availableSamples, instance->max);

	for(int i = 0; i < len; i++)
	{
		instance->samples[index].samples[i] = VuoListGetValue_VuoGenericType1(list, i+1);
	}

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
		{
			VuoListAppendValue_VuoGenericType1(list, VuoGenericType1_multiply(avg[i], (float)instance->availableSamples));
		}
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
	{
		resetSamples( *instance, sampleSize );
	}

	addSamples( *instance, list );

	*averagedList = getAverage(*instance);
}

void nodeInstanceFini(VuoInstanceData(struct nodeInstanceData *) instance)
{
	freeSamples(*instance);
}
