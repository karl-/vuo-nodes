/**
 * @file
 * co.parabox.convert.euler node implementation.
 */

#include "node.h"
#include "VuoTransform.h"
#include "VuoPoint3d.h"
#include "VuoPoint4d.h"

VuoModuleMetadata({	"title" : "Convert Euler",
					"keywords" : [ "math", "rotation", "rotate", "quaternion", "matrix" ],
					"version" : "1.0.0",
					"description":"Convert an Euler rotation to matrix and quaternion representations."
				 });

void nodeEvent
(
	VuoInputData(VuoPoint3d, { "default": {"x":0, "y":0, "z":0}}) euler,
	VuoOutputData(VuoPoint4d) quaternion,
	VuoOutputData(VuoList_VuoReal) matrix
)
{
	VuoPoint3d rad = VuoPoint3d_multiply(euler, 0.0174533);

	*quaternion = VuoTransform_quaternionFromEuler(rad);

	VuoList_VuoReal rotation = VuoListCreate_VuoReal();

	float m[9];

	VuoTransform_rotationMatrixFromEuler(rad, m);

	for(int i = 0; i < 9; i++)
		VuoListAppendValue_VuoReal(rotation, m[i]);

	*matrix = rotation;
}
