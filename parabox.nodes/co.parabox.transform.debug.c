/**
 * @file
 * co.parabox.transform.debug
 */

#include "node.h"

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
		VuoOutputData(VuoPoint3d) scale,
		VuoOutputData(VuoText) summary
)
{
	*translation = transform.translation;
	*isRotationEuler = transform.type == VuoTransformTypeEuler;
	*rotationEuler = VuoTransform_getEuler(transform);
	*rotationQuaternion = VuoTransform_getQuaternion(transform);
	*scale = transform.scale;
	VuoText sum = VuoText_format("type: %s\ntrans: %.2f, %.2f, %.2f\neuler: %.2f, %.2f, %.2f\nquat : %.2f, %.2f, %.2f, %.2f\nscale: %.2f, %.2f, %.2f",
		transform.type == VuoTransformTypeEuler ? "Euler" : (transform.type == VuoTransformTypeQuaternion ? "Quaternion" : "Angle Axis"),
		transform.translation.x, transform.translation.y, transform.translation.z,
		(*rotationEuler).x, (*rotationEuler).y, (*rotationEuler).z,
		(*rotationQuaternion).x, (*rotationQuaternion).y, (*rotationQuaternion).z, (*rotationQuaternion).w,
		transform.scale.x, transform.scale.y, transform.scale.z
		);
	VuoRegister(sum, free);
	*summary = sum;
}
