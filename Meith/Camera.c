#include "Camera.h"

#include <stdlib.h>
#include <string.h>

Camera *Camera_Init()
{
	Camera *camera = (Camera *)malloc(sizeof(Camera));

	camera->transform = *Transform_Init();
	camera->transform.position[1] = 20.0f;
	camera->transform.position[2] = 20.0f;

	vec3 cam_axis = { -1.0f, 0.0f, 0.0f };
	float angle = 45.0f;
	Transform_RotateAxis(&camera->transform, cam_axis, angle);

	camera->fov = 0.785398f;
	camera->aspect_ratio = 1.333333f;
	camera->near = 0.1f;
	camera->far = 100.0f;

	Camera_GetViewMat(camera);
	Camera_GetProjMat(camera);
	
	return camera;
}

void Camera_GetViewMat(Camera *camera)
{
	mat4x4 R;
	mat4x4_Rot(R, camera->transform.quaternion);
	
	vec3 forward = { 0.0f, 0.0f, -1.0f };
	vec3 cam_forward;
	mat3x3_MulVec3(cam_forward, R, forward);

	vec3 right = { 1.0f, 0.0f, 0.0f };
	vec3 cam_right;
	mat3x3_MulVec3(cam_right, R, right);

	vec3 cam_up;
	vec3_CrossProd(cam_up, cam_right, cam_forward);
	
	vec3 target;
	vec3_Add(target, cam_forward, camera->transform.position);
	
	vec3 n;
	vec3_Sub(n, camera->transform.position, target);
	vec3_Norm(n, n);
	
	vec3 u;
	vec3_CrossProd(u, cam_up, n);
	vec3_Norm(u, u);
	
	vec3 v;
	vec3_CrossProd(v, n, u);
	
	int i = 0;
	for(i = 0; i < 3; ++i)
		camera->view_matrix[0][i] = u[i];
	
	for(i = 0; i < 3; ++i)
		camera->view_matrix[1][i] = v[i];
		
	for(i = 0; i < 3; ++i)
		camera->view_matrix[2][i] = n[i];
		
	for(i = 0; i < 3; ++i)
		camera->view_matrix[3][i] = 0.0f;
		
	vec3_Negate(u, u);
	vec3_Negate(v, v);
	vec3_Negate(n, n);
	
	camera->view_matrix[0][3] = vec3_DotProd(u, camera->transform.position);
	camera->view_matrix[1][3] = vec3_DotProd(v, camera->transform.position);
	camera->view_matrix[2][3] = vec3_DotProd(n, camera->transform.position);
	camera->view_matrix[3][3] = 1.0f;
}

void Camera_GetProjMat(Camera *camera)
{
	float const a = 1.0f / (float)tan(camera->fov / 2.0f);
	
	camera->projection_matrix[0][0] = a / camera->aspect_ratio;
	camera->projection_matrix[0][1] = 0.0f;
	camera->projection_matrix[0][2] = 0.0f;
	camera->projection_matrix[0][3] = 0.0f;
	
	camera->projection_matrix[1][0] = 0.0f;
	camera->projection_matrix[1][1] = a;
	camera->projection_matrix[1][2] = 0.0f;
	camera->projection_matrix[1][3] = 0.0f;
	
	camera->projection_matrix[2][0] = 0.0f;
	camera->projection_matrix[2][1] = 0.0f;
	camera->projection_matrix[2][2] = -(camera->far + camera->near) / (camera->far - camera->near);
	camera->projection_matrix[2][3] = -2 * camera->far * camera->near / (camera->far - camera->near);
	
	camera->projection_matrix[3][0] = 0.0f;
	camera->projection_matrix[3][1] = 0.0f;
	camera->projection_matrix[3][2] = -1.0f;
	camera->projection_matrix[3][3] = 0.0f;
}

void Camera_Render(Camera *camera, Shader *shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "u_proj"), 1, GL_TRUE, &camera->projection_matrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "u_view"), 1, GL_TRUE, &camera->view_matrix[0][0]);
}

void Camera_Destroy(Camera *camera)
{
	free(camera);
}
