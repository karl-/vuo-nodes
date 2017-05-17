/**
 * @file
 * co.parabox.transform.debug
 */

#include "node.h"
#include "VuoTransform.h"

VuoModuleMetadata({
					 "title" : "VuoTransform Debug",
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
		VuoInputData(VuoTransform) transform,
		VuoOutputData(VuoPoint3d) translation,
		VuoOutputData(VuoBoolean) isRotationEuler,
		VuoOutputData(VuoPoint3d) rotationEuler,
		VuoOutputData(VuoPoint4d) rotationQuaternion,
		VuoOutputData(VuoList_VuoReal) quaternionMatrix,
		VuoOutputData(VuoList_VuoReal) eulerMatrix,
		VuoOutputData(VuoText) quaternionMatrixSummary,
		VuoOutputData(VuoText) eulerMatrixSummary,
		VuoOutputData(VuoText) storedMatrixSummary,
		VuoOutputData(VuoPoint3d) scale,
		VuoOutputData(VuoText) summary
)
{
	*translation = transform.translation;
	*isRotationEuler = transform.type == VuoTransformTypeEuler;
	*rotationEuler = VuoTransform_getEuler(transform);
	*rotationQuaternion = VuoTransform_getQuaternion(transform);
	*scale = transform.scale;

	VuoPoint3d eulerDegrees = VuoPoint3d_multiply(*rotationEuler, 57.2958);

	VuoText sum = VuoText_format("type: %s\ntrans: %.2f, %.2f, %.2f\neuler: %.2f, %.2f, %.2f\nquat : %.2f, %.2f, %.2f, %.2f\nscale: %.2f, %.2f, %.2f",
		transform.type == VuoTransformTypeEuler ? "Euler" : (transform.type == VuoTransformTypeQuaternion ? "Quaternion" : "Angle Axis"),
		transform.translation.x, transform.translation.y, transform.translation.z,
		eulerDegrees.x, eulerDegrees.y, eulerDegrees.z,
		(*rotationQuaternion).x, (*rotationQuaternion).y, (*rotationQuaternion).z, (*rotationQuaternion).w,
		transform.scale.x, transform.scale.y, transform.scale.z
		);
	VuoRegister(sum, free);

	float eMatrix[9], qMatrix[9];
	const float* uMatrix = transform.rotation;
	VuoTransform_rotationMatrixFromQuaternion(*rotationQuaternion, qMatrix);
	VuoTransform_rotationMatrixFromEuler(*rotationEuler, eMatrix);

	VuoText ems = VuoText_format("%.2f, %.2f, %.2f\n%.2f, %.2f, %.2f\n%.2f, %.2f, %.2f",
		eMatrix[0], eMatrix[1], eMatrix[2],
		eMatrix[3], eMatrix[4], eMatrix[5],
		eMatrix[6], eMatrix[7], eMatrix[8] );

	VuoText qms = VuoText_format("%.2f, %.2f, %.2f\n%.2f, %.2f, %.2f\n%.2f, %.2f, %.2f",
		qMatrix[0], qMatrix[1], qMatrix[2],
		qMatrix[3], qMatrix[4], qMatrix[5],
		qMatrix[6], qMatrix[7], qMatrix[8] );

	VuoText ums = VuoText_format("%.2f, %.2f, %.2f\n%.2f, %.2f, %.2f\n%.2f, %.2f, %.2f",
		uMatrix[0], uMatrix[1], uMatrix[2],
		uMatrix[3], uMatrix[4], uMatrix[5],
		uMatrix[6], uMatrix[7], uMatrix[8] );

	VuoRegister(ems, free);
	VuoRegister(qms, free);
	VuoRegister(ums, free);

	*summary = sum;

	*quaternionMatrixSummary = qms;
	*eulerMatrixSummary = ems;
	*storedMatrixSummary = ums;

	float etm[16], qtm[16];

	VuoTransform_getMatrix(transform, qtm);

	etm[0] = eMatrix[0] * transform.scale.x;
	etm[1] = eMatrix[1] * transform.scale.x;
	etm[2] = eMatrix[2] * transform.scale.x;
	etm[3] = qtm[3];
	etm[4] = eMatrix[3] * transform.scale.y;
	etm[5] = eMatrix[4] * transform.scale.y;
	etm[6] = eMatrix[5] * transform.scale.y;
	etm[7] = qtm[7];
	etm[8] = eMatrix[6] * transform.scale.z;
	etm[9] = eMatrix[7] * transform.scale.z;
	etm[10] = eMatrix[8] * transform.scale.z;
	etm[11] = qtm[11];
	etm[12] = qtm[12];
	etm[13] = qtm[13];
	etm[14] = qtm[14];
	etm[15] = qtm[15];

	VuoList_VuoReal qm = VuoListCreate_VuoReal();
	VuoList_VuoReal em = VuoListCreate_VuoReal();

	for(int i = 0; i < 16; i++)
	{
		VuoListAppendValue_VuoReal(qm, qtm[i]);
		VuoListAppendValue_VuoReal(em, etm[i]);
	}

	*eulerMatrix = em;
	*quaternionMatrix = qm;
}
