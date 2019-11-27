/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editboxes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lminta <lminta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 20:04:17 by lminta            #+#    #+#             */
/*   Updated: 2019/11/27 19:51:24 by lminta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		shift(t_gui *gui, t_obj *obj, int *i)
{
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	KW_CreateLabel(gui->gui, gui->c_o.frame, "Shift", gui->c_o.rects[0]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->shift.s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->shift.s[1], gui->c_o.rects[2]);
}

static void		vert(t_gui *gui, t_obj *obj, int *i)
{
	if (obj->type != TRIANGLE)
		return ;
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[0].s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[0].s[1], gui->c_o.rects[2]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[0].s[2], gui->c_o.rects[3]);
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	KW_CreateLabel(gui->gui, gui->c_o.frame, "Vertices", gui->c_o.rects[0]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[1].s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[1].s[1], gui->c_o.rects[2]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[1].s[2], gui->c_o.rects[3]);
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[2].s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[2].s[1], gui->c_o.rects[2]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->vertices[2].s[2], gui->c_o.rects[3]);
}

static void		basis(t_gui *gui, t_obj *obj, int *i)
{
	vert(gui, obj, i);
	if (obj->type == TRIANGLE)
		return ;
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[0].s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[0].s[1], gui->c_o.rects[2]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[0].s[2], gui->c_o.rects[3]);
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	KW_CreateLabel(gui->gui, gui->c_o.frame, "Basis", gui->c_o.rects[0]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[1].s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[1].s[1], gui->c_o.rects[2]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[1].s[2], gui->c_o.rects[3]);
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[2].s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[2].s[1], gui->c_o.rects[2]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->basis[2].s[2], gui->c_o.rects[3]);
}

static void	text_normal(t_gui *gui, t_obj *obj, int *i)
{
	shift(gui, obj, i);
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	KW_CreateLabel(gui->gui, gui->c_o.frame, "Textures", gui->c_o.rects[0]);
	if (obj->texture)
		gui->c_o.ed_b[(*i)++] = KW_CreateEditbox(gui->gui, gui->c_o.frame,
		gui->game->texture_list[obj->texture - 1], gui->c_o.rects[1]);
	else
		gui->c_o.ed_b[(*i)++] = KW_CreateEditbox(gui->gui, gui->c_o.frame,
		"No texture", gui->c_o.rects[1]);
	KW_CreateLabel(gui->gui, gui->c_o.frame, "Normals", gui->c_o.rects[2]);
	if (obj->normal)
		gui->c_o.ed_b[(*i)++] = KW_CreateEditbox(gui->gui, gui->c_o.frame,
		gui->game->normal_list[obj->normal - 1], gui->c_o.rects[3]);
	else
		gui->c_o.ed_b[(*i)++] = KW_CreateEditbox(gui->gui, gui->c_o.frame,
		"No normal", gui->c_o.rects[3]);
}

void		color_emission(t_gui *gui, t_obj *obj, int *i)
{
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	KW_CreateLabel(gui->gui, gui->c_o.frame, "Color", gui->c_o.rects[0]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->color.s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->color.s[1], gui->c_o.rects[2]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->color.s[2], gui->c_o.rects[3]);
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	KW_CreateLabel(gui->gui, gui->c_o.frame, "Emission", gui->c_o.rects[0]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->emission.s[0], gui->c_o.rects[1]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->emission.s[1], gui->c_o.rects[2]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->emission.s[2], gui->c_o.rects[3]);
	basis(gui, obj, i);
	text_normal(gui, obj, i);
	gui->c_o.labelrect.y += 30;
	gui->c_o.editboxrect[0].y += 30;
	gui->c_o.editboxrect[1].y += 30;
	gui->c_o.editboxrect[2].y += 30;
	KW_CreateLabel(gui->gui, gui->c_o.frame, "Metalness", gui->c_o.rects[0]);
	gui->c_o.ed_b[(*i)++] = f_e(gui, obj->metalness, gui->c_o.rects[1]);
	radius(gui, obj, i);
}
