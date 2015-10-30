/**
 * @file
 * co.parabox.utility.fps node implementation.
 *
 * Karl wrote this.  Do whatever you want with it.
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

	for(unsigned int i = 1; i < instance->size; i++)
		avg += (instance->history[i] - instance->history[i-1]);

	return avg / (double) (instance->size - 1);
}

struct nodeInstanceData * nodeInstanceInit(
	VuoInputData(VuoInteger) samples
)
{
	struct nodeInstanceData * instance = (struct nodeInstanceData *)malloc(sizeof(struct nodeInstanceData));
	VuoRegister(instance, free);

	instance->history = (VuoReal*) malloc(sizeof(VuoReal) * samples);
	instance->size = 0;
	instance->index = 0;
	instance->capacity = samples;

	return instance;
}

void nodeInstanceEvent(
	VuoInstanceData(struct nodeInstanceData*) instance,
	VuoInputEvent({"eventBlocking":"none"}) addTick,
	VuoInputData(VuoInteger, {"default":15, "suggestedMin":2, "suggestedMax":60}) samples,
	VuoOutputData(VuoReal) fps
)
{
	unsigned int cap = MAX(MIN(samples, 999), 2);

	if(cap != (*instance)->capacity)
		resizeHistory(*instance, cap);

	if(addTick)
	{
		unsigned int index = (*instance)->index % (*instance)->capacity;
		(*instance)->history[index] = VuoLogGetTime();
		(*instance)->index++;
		(*instance)->size = MAX((*instance)->size, index);
	}

	*fps = averageHistory(*instance);
}


void nodeInstanceFini(VuoInstanceData(struct nodeInstanceData *) instance)
{
	free((*instance)->history);
}
