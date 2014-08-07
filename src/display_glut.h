/*
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * ***** END GPL LICENSE BLOCK *****
 *
 * Contributor(s): Jiri Hnidek <jiri.hnidek@tul.cz>.
 *
 */

#ifndef DISPLAY_GLUT_H_
#define DISPLAY_GLUT_H_

#define DEDEFAULT_FPS	25

static const uint8_t red_col[3] = {255, 0, 0};
static const uint8_t white_col[3] = {255, 255, 255};
static const uint8_t gray_col[3] = {155, 155, 155};
static const uint8_t light_red_col[3] = {200, 0, 0};
static const uint8_t light_red_col_a[4] = {200, 0, 0, 150};
static const uint8_t orange_col[3] = {255, 127, 0};
static const uint8_t orange_col_a[4] = {255, 127, 0, 150};
static const uint8_t yellow_col[3] = {255, 255, 0};
static const uint8_t yellow_col_a[4] = {255, 255, 0, 150};
static const uint8_t green_col[3] = {0, 200, 0};
static const uint8_t green_col_a[4] = {0, 200, 0, 150};

/**
 * Information about surface
 */
typedef struct Material {
	float	diffuse[4];
	float	specular[4];
	float	ambient[4];
	float	shininess;
} Material;

/**
 * Information about light
 */
typedef struct Light {
	float	diffuse[4];
	float	specular[4];
	float	ambient[4];
	float	pos[4];
} Light;

/**
 * Information about camera
 */
typedef struct Camera {
	float	field_of_view;
	float	near_clipping_plane;
	float	far_clipping_plane;
	float	pos[3];
	float	target[3];
	float	up[3];
} Camera;

/**
 * Information about window
 */
typedef struct Window {
	uint16_t	fullscreen;
	uint16_t	left;
	uint16_t	top;
	uint16_t	width;
	uint16_t	height;
} Window;

/**
 * Information about mouse cursor
 */
typedef struct Mouse {
	uint8_t		state;
	uint16_t	pos[2];
} Mouse;

/**
 * Information about canvas
 */
typedef struct Canvas {
	float	bg_col_grad_top[4];
	float	bg_col_grad_bottom[4];
	float	point_size;
	float	line_width;
} Canvas;

/**
 * All information required to display basic scene
 */
typedef struct Display {
	struct Window		window;
	struct Mouse		mouse;
	struct Camera		camera;
	struct Material		material;
	struct Light		light;
	struct Canvas		canvas;
} Display;

void display_loop(struct CTX *_ctx, int argc, char *argv[]);
struct Display *create_display(void);

#endif /* DISPLAY_GLUT_H_ */
