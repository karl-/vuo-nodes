/**
 * @file
 * PbxShaderUniform C type definition.
 *
 */

#ifndef PBXSHADERUNIFORM_H
#define PBXSHADERUNIFORM_H

#include "VuoText.h"
#include "VuoInteger.h"
#include "VuoReal.h"
#include "VuoShader.h"
#include "VuoImage.h"
#include "VuoPoint2d.h"
#include "VuoPoint3d.h"
#include "VuoPoint4d.h"
#include "VuoColor.h"

enum PbxShaderUniformType
{
	PbxShaderUniformType_VuoReal,
	PbxShaderUniformType_VuoInteger,
	PbxShaderUniformType_VuoImage,
	PbxShaderUniformType_VuoPoint2d,
	PbxShaderUniformType_VuoPoint3d,
	PbxShaderUniformType_VuoPoint4d
};

typedef struct
{
	VuoText name;

	enum PbxShaderUniformType type;

	union
	{
		VuoReal realValue;
		VuoInteger intValue;
		VuoImage imageValue;
		VuoPoint2d point2dvalue;
		VuoPoint3d point3dvalue;
		VuoPoint4d point4dvalue;
	} value;
} PbxShaderUniform;

static inline void PbxShaderUniform_setUniform(VuoShader shader, PbxShaderUniform uniform)
{
	if(uniform.name != NULL)
	{
		switch(uniform.type)
		{
			case PbxShaderUniformType_VuoReal:
				VuoShader_setUniform_VuoReal(shader, uniform.name, uniform.value.realValue);
				break;

			case PbxShaderUniformType_VuoInteger:
				VuoShader_setUniform_VuoInteger(shader, uniform.name, uniform.value.intValue);
				break;

			case PbxShaderUniformType_VuoImage:
				VuoShader_setUniform_VuoImage(shader, uniform.name, uniform.value.imageValue);
				break;

			case PbxShaderUniformType_VuoPoint2d:
				VuoShader_setUniform_VuoPoint2d(shader, uniform.name, uniform.value.point2dvalue);
				break;

			case PbxShaderUniformType_VuoPoint3d:
				VuoShader_setUniform_VuoPoint3d(shader, uniform.name, uniform.value.point3dvalue);
				break;

			case PbxShaderUniformType_VuoPoint4d:
				VuoShader_setUniform_VuoPoint4d(shader, uniform.name, uniform.value.point4dvalue);
				break;
		}
	}
}

PbxShaderUniform PbxShaderUniform_makeFromJson(struct json_object * js);
struct json_object * PbxShaderUniform_getJson(const PbxShaderUniform value);
char * PbxShaderUniform_getSummary(const PbxShaderUniform value);

PbxShaderUniform PbxShaderUniform_makeFromString(const char *str);
char * PbxShaderUniform_getString(const PbxShaderUniform value);

static inline PbxShaderUniform PbxShaderUniform_make_VuoReal(VuoText name, VuoReal value) __attribute__((const));
static inline PbxShaderUniform PbxShaderUniform_make_VuoReal(VuoText name, VuoReal value)
{
	PbxShaderUniform p;
	p.name = name;
	p.type = PbxShaderUniformType_VuoReal,
	p.value.realValue = value;

	return p;
}

static inline PbxShaderUniform PbxShaderUniform_make_VuoInteger(VuoText name, VuoInteger value) __attribute__((const));
static inline PbxShaderUniform PbxShaderUniform_make_VuoInteger(VuoText name, VuoInteger value)
{
	PbxShaderUniform p;
	p.name = name;
	p.type = PbxShaderUniformType_VuoInteger,
	p.value.intValue = value;

	return p;
}

static inline PbxShaderUniform PbxShaderUniform_make_VuoImage(VuoText name, VuoImage value) __attribute__((const));
static inline PbxShaderUniform PbxShaderUniform_make_VuoImage(VuoText name, VuoImage value)
{
	PbxShaderUniform p;
	p.name = name;
	p.type = PbxShaderUniformType_VuoImage;
	p.value.imageValue = value;
	return p;
}

static inline PbxShaderUniform PbxShaderUniform_make_VuoPoint2d(VuoText name, VuoPoint2d value) __attribute__((const));
static inline PbxShaderUniform PbxShaderUniform_make_VuoPoint2d(VuoText name, VuoPoint2d value)
{
	PbxShaderUniform p;
	p.name = name;
	p.type = PbxShaderUniformType_VuoPoint2d;
	p.value.point2dvalue = value;
	return p;
}

static inline PbxShaderUniform PbxShaderUniform_make_VuoPoint3d(VuoText name, VuoPoint3d value) __attribute__((const));
static inline PbxShaderUniform PbxShaderUniform_make_VuoPoint3d(VuoText name, VuoPoint3d value)
{
	PbxShaderUniform p;
	p.name = name;
	p.type = PbxShaderUniformType_VuoPoint3d;
	p.value.point3dvalue = value;
	return p;
}

static inline PbxShaderUniform PbxShaderUniform_make_VuoPoint4d(VuoText name, VuoPoint4d value) __attribute__((const));
static inline PbxShaderUniform PbxShaderUniform_make_VuoPoint4d(VuoText name, VuoPoint4d value)
{
	PbxShaderUniform p;
	p.name = name;
	p.type = PbxShaderUniformType_VuoPoint4d;
	p.value.point4dvalue = value;
	return p;
}

static inline PbxShaderUniform PbxShaderUniform_make_VuoColor(VuoText name, VuoColor value) __attribute__((const));
static inline PbxShaderUniform PbxShaderUniform_make_VuoColor(VuoText name, VuoColor value)
{
	PbxShaderUniform p;
	p.name = name;
	p.type = PbxShaderUniformType_VuoPoint4d;
	p.value.point4dvalue = VuoPoint4d_make(value.r, value.g, value.b, value.a);
	return p;
}

/// @{
/**
 * Automatically generated function.
 */
PbxShaderUniform PbxShaderUniform_makeFromString(const char *str);
char * PbxShaderUniform_getString(const PbxShaderUniform value);
void PbxShaderUniform_retain(PbxShaderUniform value);
void PbxShaderUniform_release(PbxShaderUniform value);
/// @}

#endif
