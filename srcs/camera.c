/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 18:50:13 by lminta            #+#    #+#             */
/*   Updated: 2019/11/18 10:30:30 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	reconfigure_camera(t_cam *camera)
{
	float		x_fov;
	float		y_fov;

	x_fov = (float)WIN_W / (float)WIN_H > 1 ? camera->fov / 2 :
	camera->fov / 2 * (float)WIN_W / (float)WIN_H;
	y_fov = (float)WIN_H / (float)WIN_W > 1 ? (camera->fov / 2) :
	(camera->fov / 2) * ((float)WIN_H / (float)WIN_W);
	camera->border_y = vector_diff(
	rotate(camera->normal, camera->direction, x_fov),
	rotate(camera->normal, camera->direction, -x_fov));
	camera->border_x = vector_diff(
	rotate(cross(camera->direction, camera->normal), camera->direction, y_fov),
	rotate(cross(camera->direction, camera->normal),
	camera->direction, -y_fov));
}

void		pos_check(t_game *game, t_gui *gui)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	SDL_GetMouseState(&x, &y);
	gui->over_gui = 0;
	if (game->keys.show_gui && gui->s_s.show)
		if (x > gui->s_s.frect.x && x < gui->s_s.frect.x + gui->s_s.frect.w)
			if (y > gui->s_s.frect.y && y < gui->s_s.frect.y + gui->s_s.frect.h)
				gui->over_gui = 1;
	if (game->keys.show_gui && gui->o_s.show)
		if (x > gui->o_s.frect.x && x < gui->o_s.frect.x + gui->o_s.frect.w)
			if (y > gui->o_s.frect.y && y < gui->o_s.frect.y + gui->o_s.frect.h)
				gui->over_gui = 1;
	if (game->keys.show_gui && gui->g_b.show)
		if (x > gui->g_b.frect.x && x < gui->g_b.frect.x + gui->g_b.frect.w)
			if (y > gui->g_b.frect.y && y < gui->g_b.frect.y + gui->g_b.frect.h)
				gui->over_gui = 1;
	if (game->keys.show_gui && gui->over_gui)
		SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
	else
		SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR));
}

static void	mouse_mov(t_game *game, t_gui *gui)
{
	if (game->flag)
	{
		game->cl_info->ret =
		cl_write(game->cl_info, game->cl_info->progs[0].krls[0].args[2], sizeof(cl_float3) *
		(unsigned)WIN_H * (unsigned)WIN_W, game->gpu.vec_temp);
		game->gpu.samples = 0;
		reconfigure_camera(&game->gpu.camera[game->cam_num]);
	}
	else if (gui->flag)
	{
		game->cl_info->ret =
		cl_write(game->cl_info, game->cl_info->progs[0].krls[0].args[2], sizeof(cl_float3) *
		(unsigned)WIN_H * (unsigned)WIN_W, game->gpu.vec_temp);
		game->cl_info->ret = cl_write(game->cl_info, game->cl_info->progs[0].krls[0].args[1],
		sizeof(t_obj) * game->obj_quantity, game->gpu.objects);
		game->gpu.samples = 0;
		game->flag = 1;
	}
	else if (game->keys.space || game->keys.r)
		game->flag = 1;
}

static void	c_r(t_game *game, t_gui *gui)
{
	if (game->keys.z)
	{
		game->gpu.camera[game->cam_num].position =
		sum_cfloat3(game->gpu.camera[game->cam_num].position,
		mult_cfloat3(game->gpu.camera[game->cam_num].normal, 0.1));
		game->flag = 1;
	}
	if (game->keys.x)
	{
		game->gpu.camera[game->cam_num].position =
		sum_cfloat3(game->gpu.camera[game->cam_num].position,
		mult_cfloat3(game->gpu.camera[game->cam_num].normal, -0.1));
		game->flag = 1;
	}
	if (game->mouse.rmb && game->mouse.mm && !gui->over_gui)
	{
		rotate_horizontal(&(game->gpu.camera[game->cam_num]),
		-game->gpu.camera[game->cam_num].fov / WIN_W * game->keys.xrel);
		rotate_vertical(&(game->gpu.camera[game->cam_num]),
		-game->gpu.camera[game->cam_num].fov / WIN_H * -game->keys.yrel);
		game->flag = 1;
	}
	mouse_mov(game, gui);
}

static void	cam_rep(t_game *game, t_gui *gui)
{
	if (game->keys.d)
	{
		game->gpu.camera[game->cam_num].position =
		sum_cfloat3(game->gpu.camera[game->cam_num].position,
		mult_cfloat3(normalize(cross(game->gpu.camera[game->cam_num].normal,
		game->gpu.camera[game->cam_num].direction)), -0.1));
		game->flag = 1;
	}
	if (game->keys.q)
	{
		game->gpu.camera[game->cam_num].direction =
		rotate(game->gpu.camera[game->cam_num].normal,
		game->gpu.camera[game->cam_num].direction, M_PI / 60);
		game->flag = 1;
	}
	if (game->keys.e)
	{
		game->gpu.camera[game->cam_num].direction =
		rotate(game->gpu.camera[game->cam_num].normal,
		game->gpu.camera[game->cam_num].direction, -M_PI / 60);
		game->flag = 1;
	}
	c_r(game, gui);
}

void		camera_reposition(t_game *game, t_gui *gui)
{
	if (game->keys.w || (game->mouse.lmb && game->mouse.rmb))
	{
		game->gpu.camera[game->cam_num].position =
		sum_cfloat3(game->gpu.camera[game->cam_num].position,
		mult_cfloat3(game->gpu.camera[game->cam_num].direction, 0.1));
		game->flag = 1;
	}
	if (game->keys.s)
	{
		game->gpu.camera[game->cam_num].position =
		sum_cfloat3(game->gpu.camera[game->cam_num].position,
		mult_cfloat3(game->gpu.camera[game->cam_num].direction, -0.1));
		game->flag = 1;
	}
	if (game->keys.a)
	{
		game->gpu.camera[game->cam_num].position =
		sum_cfloat3(game->gpu.camera[game->cam_num].position,
		mult_cfloat3(normalize(cross(game->gpu.camera[game->cam_num].normal,
		game->gpu.camera[game->cam_num].direction)), 0.1));
		game->flag = 1;
	}
	cam_rep(game, gui);
}
