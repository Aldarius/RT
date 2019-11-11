/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbrella <sbrella@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/23 14:53:01 by lminta            #+#    #+#             */
/*   Updated: 2019/11/11 21:06:49 by sbrella          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_image_desc		create_desc(int num)
{
	cl_image_desc	ret;

	ret.image_type = CL_MEM_OBJECT_IMAGE2D_ARRAY;
	ret.image_array_size = num;
	ret.image_height = TEXTURE_HEIGHT;
	ret.image_width = TEXTURE_WIDTH;
	ret.image_depth = 1;
	ret.image_row_pitch = 0;
	ret.image_slice_pitch = 0;
	ret.buffer = NULL;
	ret.num_mip_levels = 0;
	ret.num_samples = 0;
	return (ret);
}

cl_image_format		create_format()
{
	cl_image_format	ret;

	ret.image_channel_data_type = CL_UNORM_INT8;
	ret.image_channel_order = CL_RGBA;
	return (ret);
}

static void			opencl_write_args(t_game *game)
{
	cl_krl_init(&game->kernels[0], 6);
	ERROR(game->cl_info->ret );
	game->kernels[0].sizes[0] = sizeof(cl_int) * (int)WIN_H * (int)WIN_W;
	game->kernels[0].sizes[1] = sizeof(t_obj) * 3;     // fix this
	game->kernels[0].sizes[2] = sizeof(cl_float3) * (int)WIN_H * (int)WIN_W;
	game->kernels[0].sizes[3] = (int)WIN_H * (int)WIN_W * sizeof(cl_ulong);
	game->kernels[0].sizes[4] = sizeof(t_txture) * 3;   //fix this
	game->kernels[0].sizes[5] = sizeof(t_txture) * 3;
	game->cl_info->ret = krl_set_args(game->cl_info->ctxt, &game->kernels[0]);
	ERROR(game->cl_info->ret );
	game->cl_info->ret = cl_write(game->cl_info, game->kernels[0].args[0],\
	sizeof(cl_int) * (int)WIN_H * (int)WIN_W, game->sdl.surface->pixels);
	game->cl_info->ret = cl_write(game->cl_info, game->kernels[0].args[2],\
	sizeof(cl_float3) * (int)WIN_H * (int)WIN_W, game->gpu.vec_temp);
	game->cl_info->ret = cl_write(game->cl_info, game->kernels[0].args[3],\
	(int)WIN_H * (int)WIN_W * sizeof(cl_ulong), game->gpu.random);
	ERROR(game->cl_info->ret );
}

void				opencl_init(t_game *game)
{
	int fd;

	game->kernels = ft_memalloc(sizeof(t_cl_krl) * 2);
	cl_krl_init(&game->kernels[0], 6);
	game->cl_info = ft_memalloc(sizeof(t_cl_info));
	game->gpu.objects = NULL;
	game->gpu.vec_temp = ft_memalloc(sizeof(cl_float3)\
	* (int)WIN_H * (int)WIN_W);
	game->gpu.random = get_random(game->gpu.random);
	game->gpu.samples = 0;
	game->cam_num = 0;
	game->gpu.camera = NULL;
	game->cl_info->krl_names = ft_memalloc(sizeof(t_vect));
	vect_init(game->cl_info->krl_names);
	vect_str_add(game->cl_info->krl_names, "render_kernel");
	cl_init(game->cl_info);
	fd = open("srcs/cl_files/main.cl", O_RDONLY);
	game->cl_info->ret = cl_prog_build(game->cl_info, game->kernels, fd,\
	"-w -I srcs/cl_files/ -I includes/cl_headers/");
	char		len[80000];
	clGetProgramBuildInfo(game->cl_info->prog, game->cl_info->dev_id, CL_PROGRAM_BUILD_LOG, sizeof(len), len, NULL);
	printf("%s\n", len);
	game->cl_info->ret = cl_krl_build(game->cl_info,\
	game->kernels, game->cl_info->krl_names);
	// clGetProgramBuildInfo(game->cl_info->prog, game->cl_info->dev_id, CL_PROGRAM_BUILD_LOG, sizeof(len), len, NULL);
	// printf("%s\n", len);
	ERROR(game->cl_info->ret );
	// opencl_write_args(game);
	ERROR(game->cl_info->ret );
}

void				opencl(t_game *game, char *argv)
{

	game->obj_quantity = 0;
	ft_memdel((void **)&game->gpu.camera);
	read_scene(argv, game);
	game->kernels[0].sizes[1] = sizeof(t_obj) * game->obj_quantity;
	game->kernels[0].sizes[0] = sizeof(cl_int) * (int)WIN_H * (int)WIN_W;
	game->kernels[0].sizes[2] = sizeof(cl_float3) * (int)WIN_H * (int)WIN_W;
	game->kernels[0].sizes[3] = (int)WIN_H * (int)WIN_W * sizeof(cl_ulong);
	game->kernels[0].args[0] = clCreateBuffer(game->cl_info->ctxt, CL_MEM_READ_WRITE,
		game->kernels[0].sizes[0], NULL, NULL);
	game->kernels[0].args[1] = clCreateBuffer(game->cl_info->ctxt, CL_MEM_READ_WRITE,
		game->kernels[0].sizes[1], NULL, NULL);
	game->kernels[0].args[2] = clCreateBuffer(game->cl_info->ctxt, CL_MEM_READ_WRITE,
		game->kernels[0].sizes[2], NULL, NULL);
	game->kernels[0].args[3] = clCreateBuffer(game->cl_info->ctxt, CL_MEM_READ_WRITE,
		game->kernels[0].sizes[3], NULL, NULL);
	clSetKernelArg(game->kernels[0].krl, 0, sizeof(game->kernels[0].args[0]),
	(void*)&game->kernels[0].args[0]);
	clSetKernelArg(game->kernels[0].krl, 2, sizeof(game->kernels[0].args[2]),
	(void*)&game->kernels[0].args[2]);
	clSetKernelArg(game->kernels[0].krl, 3, sizeof(game->kernels[0].args[3]),
	(void*)&game->kernels[0].args[3]);
	game->cl_info->ret = cl_write(game->cl_info, game->kernels[0].args[0],\
	sizeof(cl_int) * (int)WIN_H * (int)WIN_W, game->sdl.surface->pixels);
	game->cl_info->ret = cl_write(game->cl_info, game->kernels[0].args[2],\
	sizeof(cl_float3) * (int)WIN_H * (int)WIN_W, game->gpu.vec_temp);
	game->cl_info->ret = cl_write(game->cl_info, game->kernels[0].args[3],\
	(int)WIN_H * (int)WIN_W * sizeof(cl_ulong), game->gpu.random);
	cl_image_format		format = create_format();
	cl_image_desc		desc_textures = create_desc(game->textures_num);
	cl_image_desc		desc_normals = create_desc(game->normals_num);
	cl_int				err;
	game->kernels[0].args[4] = clCreateImage(game->cl_info->ctxt, CL_MEM_READ_ONLY, &format, &desc_textures, NULL, &err);
	game->kernels[0].args[5] = clCreateImage(game->cl_info->ctxt, CL_MEM_READ_ONLY, &format, &desc_normals, NULL, &err);
	ERROR(game->cl_info->ret );
	printf("%zu %zu\n", desc_textures.image_row_pitch, desc_textures.image_slice_pitch);
	size_t		origin[3] = {0, 0, 0};
	size_t		region_textures[3] = {TEXTURE_WIDTH, TEXTURE_HEIGHT, game->textures_num};
	size_t		region_normals[3] = {TEXTURE_WIDTH, TEXTURE_HEIGHT, game->normals_num};
	game->kernels[0].args[1] = clCreateBuffer(game->cl_info->ctxt,\
	CL_MEM_READ_ONLY, game->kernels[0].sizes[1], NULL, &game->cl_info->ret);
	clSetKernelArg(game->kernels[0].krl, 1, sizeof(game->kernels[0].args[1]),
	(void*)&game->kernels[0].args[1]);
	clSetKernelArg(game->kernels[0].krl, 4, sizeof(game->kernels[0].args[4]),
	(void*)&game->kernels[0].args[4]);
	clSetKernelArg(game->kernels[0].krl, 5, sizeof(game->kernels[0].args[5]),
	(void*)&game->kernels[0].args[5]);
	game->cl_info->ret = cl_write(game->cl_info, game->kernels[0].args[1],\
	sizeof(t_obj) * game->obj_quantity, game->gpu.objects);
	game->cl_info->ret = clEnqueueWriteImage(game->cl_info->cmd_queue, game->kernels[0].args[4],
	CL_TRUE, origin, region_textures, 0, 0, game->textures, 0, NULL, NULL);
		ERROR(game->cl_info->ret );
	game->cl_info->ret = clEnqueueWriteImage(game->cl_info->cmd_queue, game->kernels[0].args[5],
	CL_TRUE, origin, region_normals, 0, 0, game->normals, 0, NULL, NULL);
	ERROR(game->cl_info->ret );
}

void				free_opencl(t_game *game)
{
	clReleaseMemObject(game->kernels[0].args[1]);
	clReleaseMemObject(game->kernels[0].args[4]);
	clReleaseMemObject(game->kernels[0].args[5]);
}

