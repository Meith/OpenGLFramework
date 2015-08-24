#include "RigidBody.h"

#include <stdlib.h>

void State_Recalculate(State* state)
{
	vec3_Scale(state->velocity, state->momentum, state->inverse_mass);
	vec3_Scale(state->angular_velocity, state->angular_momentum, state->inverse_inertia_tensor);
	quat_Norm(state->transform.quaternion, state->transform.quaternion);

	quat angular_velocity;
	angular_velocity[0] = state->angular_velocity[0];
	angular_velocity[1] = state->angular_velocity[1];
	angular_velocity[2] = state->angular_velocity[2];
	angular_velocity[3] = 0.0f;
	quat_Mul(state->spin, angular_velocity, state->transform.quaternion);
	vec4_Scale(state->spin, state->spin, 0.5f);
	Transform_GetWorldMat(&state->transform);
}

RigidBody *RigidBody_Init(Transform transform)
{
	RigidBody *rigid_body = (RigidBody *)malloc(sizeof(RigidBody));

	rigid_body->current.transform = transform;
	rigid_body->current.size = 0.5f;
	rigid_body->current.mass = 1.0f;
	rigid_body->current.inverse_mass = 1.0f / rigid_body->current.mass;
	vec3_Zero(rigid_body->current.momentum);
	vec3_Zero(rigid_body->current.angular_momentum);
	rigid_body->current.inertia_tensor = rigid_body->current.mass * rigid_body->current.size * rigid_body->current.size * 2.0f / 5.0f;
	rigid_body->current.inverse_inertia_tensor = 1.0f / rigid_body->current.inertia_tensor;
	vec3_Zero(rigid_body->current.direction);
	rigid_body->current.speed = 0.0f;
	State_Recalculate(&rigid_body->current); 
	rigid_body->previous = rigid_body->current;

	return rigid_body;
}

void RigidBody_Update(RigidBody *rigid_body, Transform *transform, float t, const float dt)
{
	rigid_body->current.transform = *transform;
	rigid_body->previous = rigid_body->current;
	Derivative_Integrate(&rigid_body->current, t, dt);
}

void RigidBody_UpdateTransform(RigidBody *rigidbody, Transform *transform)
{
	*transform = rigidbody->current.transform;
}

void RigidBody_Destroy(RigidBody *rigid_body)
{
	free(rigid_body);
}