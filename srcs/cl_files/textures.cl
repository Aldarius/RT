#include "kernel.hl"

float3					get_color(t_obj *object, float3 hitpoint, t_scene *scene, float2 *coord)
{
	float4				buf;

	if (object->texture > 0)
	{
		buf = (float4)(coord->x, coord->y, (float)(object->texture + 1), 0.f);
		return(cl_float4_to_float3(read_imagef(scene->textures, text_samp, buf)));
	}
	else
		return (object->color);
}

float3					global_texture(t_ray *ray, t_scene *scene)
{
	float3				vect;
	float				u;
	float				v;
	float4				buf;

	vect = ray->dir;
	u = 0.5 + (atan2(vect[2], vect[0])) / (2 * PI);
	v = 0.5 - (asin(vect[1])) / PI;
	buf = (float4)(u, v, 1.f, 0.f);
	return(cl_float4_to_float3(read_imagef(scene->textures, text_samp, buf)));
}