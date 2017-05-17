/**
 * @file
 * co.parabox.convert.quaternion node implementation.
 */

#include "node.h"
#include "VuoTransform.h"
#include "VuoPoint3d.h"
#include "VuoPoint4d.h"

VuoModuleMetadata({	"title" : "Convert Quaternion",
					"keywords" : [ "math", "rotation", "rotate", "euler", "matrix" ],
					"version" : "1.0.0",
					"description":"Convert quaternion rotation to matrix and euler representations."
				 });

void nodeEvent
(
	VuoInputData(VuoPoint4d, { "default": {"x":0, "y":0, "z":0, "w":1}}) quaternion,
	VuoOutputData(VuoPoint3d) euler,
	VuoOutputData(VuoList_VuoReal) matrix
)
{
	*euler = VuoPoint3d_multiply(VuoTransform_eulerFromQuaternion(quaternion), 57.2958);

	*matrix = VuoListCreate_VuoReal();

	float m[9];
	VuoTransform_rotationMatrixFromQuaternion(quaternion, m);

	for(int i = 0; i < 9; i++)
		VuoListAppendValue_VuoReal(*matrix, m[i]);
}
