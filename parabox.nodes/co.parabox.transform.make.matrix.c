/**
 * @file
 * co.parabox.transform.debug
 */

#include "node.h"
#include "VuoTransform.h"

VuoModuleMetadata({
					 "title" : "Make Transform With Matrix",
					 "keywords" : [
						"transform", "rotation", "euler", "quaternion", "quat", "glm"
					 ],
					 "version" : "1.0.0",
					 "dependencies" : [
					 ],
					 "node": {
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoList_VuoReal) matrix,
		VuoOutputData(VuoTransform) transform
)
{
	bool is4x4 = VuoListGetCount_VuoReal(matrix) == 16;
	bool is3x3 = VuoListGetCount_VuoReal(matrix) == 9;

	if(!is3x3 && !is4x4)
		return;

	VuoReal* m = VuoListGetData_VuoReal(matrix);

	float values[16];

	if(is4x4)
	{
		for(int i = 0; i < 16; i++)
			values[i] = (float) m[i];
	}
	else
	{
		values[ 0] = VuoListGetValue_VuoReal(matrix, 1);
		values[ 1] = VuoListGetValue_VuoReal(matrix, 2);
		values[ 2] = VuoListGetValue_VuoReal(matrix, 3);
		values[ 3] = 0;
		values[ 4] = VuoListGetValue_VuoReal(matrix, 4);
		values[ 5] = VuoListGetValue_VuoReal(matrix, 5);
		values[ 6] = VuoListGetValue_VuoReal(matrix, 6);
		values[ 7] = 0;
		values[ 8] = VuoListGetValue_VuoReal(matrix, 7);
		values[ 9] = VuoListGetValue_VuoReal(matrix, 8);
		values[10] = VuoListGetValue_VuoReal(matrix, 9);
		values[11] = 0;
		values[12] = 0;
		values[13] = 0;
		values[14] = 0;
		values[15] = 1;
	}

	*transform = VuoTransform_makeFromMatrix4x4(values);
}
