/**
 * @file
 * PbxShaderUniform implementation.
 *
 * @copyright Copyright © 2012–2014 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "PbxShaderUniform.h"
#include "VuoText.h"

VuoModuleMetadata({
					 "title" : "Shader Uniform",
					 "description" : "A key value pair that can be used to pass data to a GLSL uniform.  See also `Make Shader with GLSL` node.",
					 "keywords" : [ ],
					 "version" : "1.0.0",
					 "dependencies" : [
						 "c"
					 ]
				 });

PbxShaderUniform PbxShaderUniform_makeFromJson(json_object * js)
{
	PbxShaderUniform kvp = { "", PbxShaderUniformType_VuoReal, 0 };
	json_object *o = NULL;

	if (json_object_object_get_ex(js, "name", &o))
		kvp.name = VuoText_makeFromJson(o);

	if (json_object_object_get_ex(js, "realValue", &o))
	{
		kvp.type = PbxShaderUniformType_VuoReal;
		kvp.value.realValue = VuoReal_makeFromJson(o);
	} 
	else if(json_object_object_get_ex(js, "intValue", &o))
	{
		kvp.type = PbxShaderUniformType_VuoInteger;
		kvp.value.intValue = VuoInteger_makeFromJson(o);
	}
	else if(json_object_object_get_ex(js, "imageValue", &o))
	{
		kvp.type = PbxShaderUniformType_VuoImage;
		kvp.value.imageValue = VuoImage_makeFromJson(o);
	}
	else if(json_object_object_get_ex(js, "point2dvalue", &o))
	{
		kvp.type = PbxShaderUniformType_VuoPoint2d;
		kvp.value.point2dvalue = VuoPoint2d_makeFromJson(o);
	}
	else if(json_object_object_get_ex(js, "point3dvalue", &o))
	{
		kvp.type = PbxShaderUniformType_VuoPoint3d;
		kvp.value.point3dvalue = VuoPoint3d_makeFromJson(o);
	}
	else if(json_object_object_get_ex(js, "point4dvalue", &o))
	{
		kvp.type = PbxShaderUniformType_VuoPoint4d;
		kvp.value.point4dvalue = VuoPoint4d_makeFromJson(o);
	}

	return kvp;
}

json_object * PbxShaderUniform_getJson(const PbxShaderUniform kvp)
{
	json_object *js = json_object_new_object();

	json_object *nameObject = VuoText_getJson(kvp.name);
	json_object_object_add(js, "name", nameObject);

	switch(kvp.type)
	{
		case PbxShaderUniformType_VuoReal:
		{
			json_object *valueObject = VuoReal_getJson(kvp.value.realValue);
			json_object_object_add(js, "realValue", valueObject);
			break;
		}

		case PbxShaderUniformType_VuoInteger:
		{
			json_object *valueObject = VuoInteger_getJson(kvp.value.intValue);
			json_object_object_add(js, "intValue", valueObject);
			break;
		}
		
		case PbxShaderUniformType_VuoImage:
		{
			json_object *valueObject = VuoImage_getJson(kvp.value.imageValue);
			json_object_object_add(js, "imageValue", valueObject);
			break;
		}
		
		case PbxShaderUniformType_VuoPoint2d:
		{
			json_object *valueObject = VuoPoint2d_getJson(kvp.value.point2dvalue);
			json_object_object_add(js, "point2dvalue", valueObject);
			break;
		}
		
		case PbxShaderUniformType_VuoPoint3d:
		{
			json_object *valueObject = VuoPoint3d_getJson(kvp.value.point3dvalue);
			json_object_object_add(js, "point3dvalue", valueObject);
			break;
		}

		case PbxShaderUniformType_VuoPoint4d:
		{
			json_object *valueObject = VuoPoint4d_getJson(kvp.value.point4dvalue);
			json_object_object_add(js, "point4dvalue", valueObject);
			break;
		}
	}

	return js;
}

char * PbxShaderUniform_getSummary(const PbxShaderUniform kvp)
{
	switch(kvp.type)
	{
		case PbxShaderUniformType_VuoReal:
			return VuoText_format("%s: %g", kvp.name, kvp.value.realValue);

		case PbxShaderUniformType_VuoInteger:
			return VuoText_format("%s: %lli", kvp.name, kvp.value.intValue);
			
		case PbxShaderUniformType_VuoImage:
			return VuoText_format("%s: %u", kvp.name, kvp.value.imageValue->glTextureName);
			
		case PbxShaderUniformType_VuoPoint2d:
			return VuoText_format("%s: {%f, %f}", kvp.name, kvp.value.point2dvalue.x, kvp.value.point2dvalue.y );
			
		case PbxShaderUniformType_VuoPoint3d:
			return VuoText_format("%s: {%f, %f, %f}", kvp.name, kvp.value.point3dvalue.x, kvp.value.point3dvalue.y, kvp.value.point3dvalue.z);

		case PbxShaderUniformType_VuoPoint4d:
			return VuoText_format("%s: {%f, %f, %f, %f}", kvp.name, kvp.value.point4dvalue.x, kvp.value.point4dvalue.y, kvp.value.point4dvalue.z, kvp.value.point4dvalue.w);
	}
}
