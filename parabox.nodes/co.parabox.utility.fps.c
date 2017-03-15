/**
 * @file
 * co.parabox.utility.fps node implementation.
 *
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Frames Per Second",
					 "keywords" : [ "fps", "count", "performance" ],
					 "version" : "1.0.0",
					 "description": "Keeps a rolling average of timestamp deltas.  Use with `Requested Frame` output port or some other timestamp format.",
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

struct nodeInstanceData
{
	VuoReal* history;
	unsigned int index;
	unsigned int size;
	unsigned int capacity;

	VuoReal lastEvent;
};

void resizeHistory(struct nodeInstanceData* instance, unsigned int newCapacity)
{
	if(newCapacity > instance->capacity)
	{
		VuoReal* newHistory = (VuoReal*) malloc(sizeof(VuoReal) * newCapacity);

		for(int i = 0; i < instance->size; i++)
			newHistory[i] = instance->history[i];

		instance->capacity = newCapacity;
		instance->size = instance->size;
		instance->index = instance->size;

		free(instance->history);
		instance->history = newHistory;
	}
	else
	{
		/// discards newer data and keeps old, which is crappy, but this is easier
		instance->capacity = newCapacity;
		instance->size = MIN(newCapacity, instance->size);
		instance->index = instance->index % newCapacity;
	}
}

VuoReal averageHistory(struct nodeInstanceData* instance)
{
	VuoReal avg = 0;

	for(unsigned int i = 0; i < instance->size; i++)
		avg += instance->history[i];

	return avg / (double) MAX(instance->size, 1);
}

struct nodeInstanceData * nodeInstanceInit(
	VuoInputData(VuoInteger) samples
)
{
	struct nodeInstanceData * instance = (struct nodeInstanceData *)malloc(sizeof(struct nodeInstanceData));
	VuoRegister(instance, free);

	unsigned int initialSize = MIN(MAX(2, samples), 999);
	instance->history = (VuoReal*) malloc(sizeof(VuoReal) * initialSize);
	instance->size = 0;
	instance->index = 0;
	instance->capacity = initialSize;
	instance->lastEvent = -1;

	return instance;
}

void nodeInstanceEvent(
	VuoInstanceData(struct nodeInstanceData*) instance,
	VuoInputEvent({"eventBlocking":"none"}) addTick,
	VuoInputData(VuoInteger, {"default":15, "suggestedMin":2, "suggestedMax":60}) samples,
	VuoInputEvent({"eventBlocking":"none"}) reset,
	VuoOutputData(VuoReal) fps
)
{
	unsigned int cap = MAX(MIN(samples, 999), 2);

	if(cap != (*instance)->capacity)
		resizeHistory(*instance, cap);

	if(addTick)
	{
		VuoReal time = VuoLogGetTime();
		VuoReal delta = time - (*instance)->lastEvent;

		if((*instance)->lastEvent > 0)
		{
			unsigned int index = (*instance)->index % (*instance)->capacity;
			(*instance)->history[index] = time - (*instance)->lastEvent;
			(*instance)->index = index + 1;
			(*instance)->size = MAX((*instance)->size, index);
		}

		(*instance)->lastEvent = time;
	}

	if(reset)
	{
		(*instance)->size = 0;
		(*instance)->index = 0;
		(*instance)->lastEvent = -1;
	}

	*fps = 1. / averageHistory(*instance);
}


void nodeInstanceFini(VuoInstanceData(struct nodeInstanceData *) instance)
{
	free((*instance)->history);
}
