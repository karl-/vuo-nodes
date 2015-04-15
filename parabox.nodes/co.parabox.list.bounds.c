/**
 * @file
 * co.parabox.list.bounds node implementation.
 *
 * @copyright Copyright © 2012–2014 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Get List Bounds",
					 "keywords" : [ "min", "max", "range" ],
					 "version" : "1.0.0",
					  "genericTypes" : {
						 "VuoGenericType1" : {
							 "compatibleTypes" : [ "VuoReal", "VuoInteger" ]
						 }
					 },
					 "description": "Get the minimum and maximum values in a list.."
				 });

static VuoGenericType1 VuoGenericType1_maximum(VuoGenericType1 a, VuoGenericType1 b)
{
	return a > b ? a : b;
}

static VuoGenericType1 VuoGenericType1_minimum(VuoGenericType1 a, VuoGenericType1 b)
{
	return a < b ? a : b;
}

void nodeEvent
(
	VuoInputData(VuoList_VuoGenericType1) list,
	VuoOutputData(VuoGenericType1) minimum,
	VuoOutputData(VuoGenericType1) maximum
)
{
	VuoGenericType1 min = VuoListGetValue_VuoGenericType1(list, 1), max = min;

	for(int i = 1; i < VuoListGetCount_VuoGenericType1(list); i++)
	{
		min = VuoGenericType1_minimum(VuoListGetValue_VuoGenericType1(list, i+1), min);
		max = VuoGenericType1_maximum(VuoListGetValue_VuoGenericType1(list, i+1), max);
	}

	*minimum = min;
	*maximum = max;
}
