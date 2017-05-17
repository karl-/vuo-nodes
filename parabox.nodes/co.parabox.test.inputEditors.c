/**
 * @file
 * co.parabox.test.inputEditors node implementation.
 *
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Input Editors!",
					 "keywords" : [ "test", "real", "vector", "point" ],
					 "version" : "1.0.0",
					 "description": "Displays a bunch of input editors.",
				 });

void nodeEvent
(
	VuoInputData(VuoInteger, {"default": 1, "suggestedMin":0, "suggestedMax":10, "suggestedStep":1, "auto":-9223372036854775807}) integer,
	VuoInputData(VuoReal, {"default": 0.5, "suggestedMin":0, "suggestedMax":1.0, "suggestedStep":0.1}) real,
	VuoInputData(VuoPoint2d, {
		"default": { "x": 0.5, "y": 0.5 },
		"suggestedMin": { "x": 0, "y": 0 },
		"suggestedMax": { "x": 1, "y": 1 },
		"suggestedStep": { "x": 0.1, "y": 0.1 }
	}) point2d,
	VuoInputData(VuoPoint3d, {
		"default": { "x": 0.5, "y": 0.5, "z": 0.5 },
		"suggestedMin": { "x": 0, "y": 0, "z": 0.0 },
		"suggestedMax": { "x": 1, "y": 1, "z": 1 },
		"suggestedStep": { "x": 0.1, "y": 0.1, "z": 0.1 }
	}) point3d,
	VuoInputData(VuoPoint4d, {
		"default": { "x": 0.5, "y": 0.5, "z": 0.5, "w": 0.5 },
		"suggestedMin": { "x": 0, "y": 0, "z": 0.0, "w": 0.0 },
		"suggestedMax": { "x": 1, "y": 1, "z": 1 , "w": 1 },
		"suggestedStep": { "x": 0.1, "y": 0.1, "z": 0.1, "w": 0.1 }
	}) point4d,
	VuoInputData(VuoTransform2d) transform2d,
	VuoInputData(VuoTransform) transform,
	VuoOutputData(VuoInteger) outInteger,
	VuoOutputData(VuoReal) outReal,
	VuoOutputData(VuoPoint2d) outPoint2d,
	VuoOutputData(VuoPoint3d) outPoint3d,
	VuoOutputData(VuoPoint4d) outPoint4d,
	VuoOutputData(VuoTransform2d) outTransform2d,
	VuoOutputData(VuoTransform) outTransform
)
{
	*outInteger			= integer;
	*outReal			= real;
	*outPoint2d			= point2d;
	*outPoint3d			= point3d;
	*outPoint4d			= point4d;
	*outTransform2d		= transform2d;
	*outTransform		= transform;
}
