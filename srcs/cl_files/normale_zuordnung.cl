#include "options.cl"

static float3			interpolate_color_as_vector(float4 color)
{
	float3				new_color;

	color.xz = color.zx;
	new_color.x = color.x * 2.f - 1.f;
	new_color.y = color.y * 2.f - 1.f;
	new_color.z = color.z * 2.f - 1.f;
	return (new_color);
}

static float3			plane_normal_map(t_obj *object, t_intersection *intersection, float3 normal, float2 *coord, float3 inter_vector)
{
	float3				end_vector;

	end_vector = inter_vector.x * normal + object->basis[0] * inter_vector.z + object->basis[1] * inter_vector.y;
	return (end_vector);
}

static float3			cylinder_normal_map(t_obj *object, t_intersection *intersection, float3 normal, float2 *coord, float3 inter_vector)
{
	float3				end_vector;
	float3				third_basis;

	third_basis = cross(object->v, normal);
	end_vector = inter_vector.x * normal + third_basis * inter_vector.z + object->v * inter_vector.y;
	return (end_vector);
}

static float3			sphere_normal_map(t_obj *object, t_intersection *intersection, float3 normal, float2 *coord, float3 inter_vector)
{
	float3				third_basis;
	float3				end_vector;

	third_basis = cross(object->v, normal);
	end_vector = inter_vector.x * normal + object->basis[0] * inter_vector.z + object->basis[1] * inter_vector.y;
	return (end_vector);
}

float3					normal_map(t_obj *object, t_intersection *intersection, float3 normal, float2 *coord, __read_only image2d_array_t normals)
{
	float3				normal_via_map;
	float4				buf;

	buf = (float4)(coord->x, coord->y, (float)(object->normal - 1), 0.f);
	normal_via_map = interpolate_color_as_vector(read_imagef(normals, text_samp, buf));
	if (object->type == PLANE)
	 	normal = plane_normal_map(object, intersection, normal, coord, normal_via_map);
	else if (object->type == CYLINDER)
	 	normal = cylinder_normal_map(object, intersection, normal, coord, normal_via_map);
	// else if (object->type == CONE)
	//  	normal = get_cone_normal_map(object, intersection, normal, coord);
	// else if (object->type == TRIANGLE)
	// 	normal = object->v;
	else
		normal = sphere_normal_map(object, intersection, normal, coord, normal_via_map);
	return (normalize(normal));
}