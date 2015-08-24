#ifndef __Transform__
#define __Transform__

#include "Vector.h"

typedef struct Transform
{	
	vec3 position;
	quat quaternion;
	vec3 scale;
	
	mat4x4 world_matrix;
} Transform;

Transform *Transform_Init();

static _inline void Transform_MoveDirection(Transform *transform, vec3 direction, float disp)
{
	vec3 move;
	mat3x3_MulVec3(move, transform->world_matrix, direction);
	vec3_Scale(move, move, disp);
	vec3_Add(transform->position, transform->position, move);
}

static _inline void Transform_RotateAxis(Transform *transform, vec3 axis, float angle)
{
	vec4 current_quat;
	vec4_Copy(current_quat, transform->quaternion);
	vec4 new_quat;
	quat_FromAxisAngle(new_quat, axis, angle);
	quat_Mul(transform->quaternion, current_quat, new_quat);
}

static _inline void Transform_Scale(Transform *transform, vec3 scale)
{
	vec3_Mul(transform->scale, transform->scale, scale);
}

void Transform_GetWorldMat(Transform *transform);
void Transform_Destroy(Transform *transform);

#endif
