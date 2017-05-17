/**
 * @file
 * co.parabox.convert.euler node implementation.
 */

#include "node.h"
#include "VuoTransform.h"
#include "VuoPoint3d.h"
#include "VuoPoint4d.h"

VuoModuleMetadata({	"title" : "Convert Rotation",
					"keywords" : [ "math", "rotation", "rotate", "quaternion", "matrix" ],
					"version" : "1.0.0",
					"description":"Convert a rotation matrix (3x3) to Euler and Quaternion representations."
				 });

void nodeEvent
(
	VuoInputData(VuoList_VuoReal) matrix,
	VuoOutputData(VuoPoint3d) euler,
	VuoOutputData(VuoPoint4d) quaternion
)
{
	double* m = VuoListGetData_VuoReal(matrix);

	float mf[9];

	for(int i = 0; i < 9; i++)
		mf[i] = m[i];

	*euler = VuoTransform_eulerFromMatrix(mf);
}
