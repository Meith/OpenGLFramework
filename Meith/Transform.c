#include "Transform.h"

#include <stdlib.h>
#include <string.h>

Transform *Transform_Init()
{
	Transform *transform = (Transform *)malloc(sizeof(Transform));
	
	vec3_Zero(transform->position);
	quat_Identity(transform->quaternion);

	vec3 scale = { 1.0f, 1.0f, 1.0f };
	vec3_Copy(transform->scale, scale);

	Transform_GetWorldMat(transform);
		
	return transform;
}

void Transform_GetWorldMat(Transform *transform)
{
	mat4x4 T;
	mat4x4_Trans(T, transform->position);

	mat4x4 R;
	quat_Norm(transform->quaternion, transform->quaternion);
	mat4x4_Rot(R, transform->quaternion);

	mat4x4 S;
	mat4x4_Scale(S, transform->scale);

	mat4x4 temp_mat;
	mat4x4_Mul(temp_mat, T, R);

	mat4x4_Mul(transform->world_matrix, temp_mat, S);
}

void Transform_Destroy(Transform *transform)
{
	free(transform);
}
