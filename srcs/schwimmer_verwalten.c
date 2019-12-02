/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schwimmer_verwalten.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lminta <lminta@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 17:03:05 by sbrella           #+#    #+#             */
/*   Updated: 2019/12/01 19:14:55 by lminta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3	cross(cl_float3 one, cl_float3 two)
{
	cl_float3	ret;

	ret.s[0] = one.s[1] * two.s[2] - one.s[2] * two.s[1];
	ret.s[1] = one.s[2] * two.s[0] - one.s[0] * two.s[2];
	ret.s[2] = one.s[0] * two.s[1] - one.s[1] * two.s[0];
	return (ret);
}

cl_float3	vector_diff(cl_float3 one, cl_float3 two)
{
	cl_float3	res;

	res.s[0] = one.s[0] - two.s[0];
	res.s[1] = one.s[1] - two.s[1];
	res.s[2] = one.s[2] - two.s[2];
	return (res);
}

cl_float3	create_cfloat3(float x, float y, float z)
{
	cl_float3	re;

	re.v4[0] = x;
	re.v4[1] = y;
	re.v4[2] = z;
	return (re);
}

cl_float3	sum_cfloat3(cl_float3 one, cl_float3 two)
{
	cl_float3	res;

	res.s[0] = one.s[0] + two.s[0];
	res.s[1] = one.s[1] + two.s[1];
	res.s[2] = one.s[2] + two.s[2];
	return (res);
}

cl_float3	mult_cfloat3(cl_float3 one, float f)
{
	cl_float3	res;

	res.s[0] = one.s[0] * f;
	res.s[1] = one.s[1] * f;
	res.s[2] = one.s[2] * f;
	return (res);
}

float		vec_len(cl_float3 vec)
{
	if (vec.s[0] == INFINITY || vec.s[1] == INFINITY || vec.s[2] == INFINITY)
		return (INFINITY);
	return (sqrt(vec.s[0] * vec.s[0] + vec.s[1] * vec.s[1] + vec.s[2] * vec.s[2]));
}
