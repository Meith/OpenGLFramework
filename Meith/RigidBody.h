#ifndef __RigidBody__
#define __RigidBody__

#include "Vector.h"
#include "Transform.h"

#include <math.h>

typedef struct State
{
	// primary physics state
	Transform transform;
	vec3 momentum;
	vec3 angular_momentum;

	// secondary physics state
	vec3 velocity;
	quat spin;
	vec3 angular_velocity;

	// my stuff
	vec3 direction;
	float speed;

	// constant state
	float size;
	float mass;
	float inverse_mass;
	float inertia_tensor;
	float inverse_inertia_tensor;
} State;

void State_Recalculate(State *state);

static _inline void State_AddForce(State *state, float t, vec3 force, vec3 torque)
{
	vec3_Zero(force);
	vec3_Zero(torque);

	vec3_Scale(force, state->direction, state->speed);
	vec3 r = { 0.0f, -state->size, 0.0f };
	vec3_CrossProd(torque, force, r);

	// damping torque angular drag of 0.05f
	vec3_Scale(state->angular_velocity, state->angular_velocity, 0.015f);
	vec3_Scale(state->velocity, state->velocity, 0.15f);
	vec3_Sub(torque, torque, state->angular_velocity);
	vec3_Sub(force, force, state->velocity);
}

typedef struct RigidBody
{
	State previous;
	State current;
} RigidBody;

RigidBody *RigidBody_Init(Transform transform);
void RigidBody_Update(RigidBody *rigid_body, Transform *transform, float t, const float dt);
void RigidBody_UpdateTransform(RigidBody *rigidbody, Transform *transform);
void RigidBody_Destroy(RigidBody *rigid_body);

typedef struct Derivative
{
	vec3 velocity;
	vec3 force;
	quat spin;
	vec3 torque;
} Derivative;

static _inline Derivative Derivative_EvaluateFirst(State *state, float t)
{
	Derivative output;
	vec3_Copy(output.velocity, state->velocity);
	vec4_Copy(output.spin, state->spin);
	State_AddForce(state, t, output.force, output.torque);
	return output;
}

static _inline void Derivative_AddVec3Mag(vec3 res, vec3 v, const float dt)
{
	vec3 mag;
	vec3_Scale(mag, v, dt);
	vec3_Add(res, res, mag);
}

static _inline void Derivative_AddVec4Mag(vec4 res, vec4 v, const float dt)
{
	vec4 mag;
	vec4_Scale(mag, v, dt);
	vec4_Add(res, res, mag);
}

static _inline Derivative Derivative_Evaluate(State state, float t, const float dt, Derivative *derivative)
{
	Derivative_AddVec3Mag(state.transform.position, derivative->velocity, dt);

	Derivative_AddVec3Mag(state.momentum, derivative->force, dt);

	Derivative_AddVec4Mag(state.transform.quaternion, derivative->spin, dt);

	Derivative_AddVec3Mag(state.angular_momentum, derivative->torque, dt);

	State_Recalculate(&state);

	Derivative output;
	vec3_Copy(output.velocity, state.velocity);
	vec4_Copy(output.spin, state.spin);
	State_AddForce(&state, t + dt, output.force, output.torque);
	return output;
}

static _inline void Derivative_AddVec3Step(vec3 res, vec3 a, vec3 b, vec3 c, vec3 d, const float dt)
{
	vec3 bc;
	vec3_Add(bc, b, c);
	vec3_Scale(bc, bc, 2.0f);

	vec3 abcd;
	vec3_Add(abcd, a, bc);
	vec3_Add(abcd, abcd, d);
	vec3_Scale(abcd, abcd, 2.0f / 5.0f * dt); // 2 / 5 is sphere related const. Take care later.
	vec3_Add(res, res, abcd);
}

static _inline void Derivative_AddVec4Step(vec4 res, vec4 a, vec4 b, vec4 c, vec4 d, const float dt)
{
	vec4 bc;
	vec4_Add(bc, b, c);
	vec4_Scale(bc, bc, 2.0f);

	vec4 abcd;
	vec4_Add(abcd, a, bc);
	vec4_Add(abcd, abcd, d);
	vec4_Scale(abcd, abcd, 2.0f / 5.0f * dt);
	vec4_Add(res, res, abcd);
}

static _inline void Derivative_Integrate(State *state, float t, const float dt)
{
	Derivative a = Derivative_EvaluateFirst(state, t);
	Derivative b = Derivative_Evaluate(*state, t, dt * 0.5f, &a);
	Derivative c = Derivative_Evaluate(*state, t, dt * 0.5f, &b);
	Derivative d = Derivative_Evaluate(*state, t, dt, &c);
	
	Derivative_AddVec3Step(state->transform.position, a.velocity, b.velocity, c.velocity, d.velocity, dt);
	Derivative_AddVec3Step(state->momentum, a.force, b.force, c.force, d.force, dt);
	Derivative_AddVec4Step(state->transform.quaternion, a.spin, b.spin, c.spin, d.spin, dt);
	Derivative_AddVec3Step(state->angular_momentum, a.torque, b.torque, c.torque, d.torque, dt);

	State_Recalculate(state);
}

#endif