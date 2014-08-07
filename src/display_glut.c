/*
 * display_glut.c
 *
 *  Created on: 6. 8. 2014
 *      Author: jiri
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <sys/time.h>
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "display_glut.h"
#include "math_lib.h"

static struct CTX *ctx = NULL;

/**
 * \brief Initialize OpenGL context
 */
static void gl_init(void)
{
	glClearColor(0.2, 0.2, 0.2, 1.0);	/* Is replaced with gradient later */
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPointSize(ctx->display->canvas.point_size);
	glLineWidth(ctx->display->canvas.line_width);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ctx->display->light.ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ctx->display->light.diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, ctx->display->light.specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ctx->display->material.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ctx->display->material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ctx->display->material.specular);
	glMaterialf(GL_FRONT, GL_SHININESS, ctx->display->material.shininess);
}

static void display_string_2d(char *string, int x, int y, const uint8_t *col)
{
	glColor3ubv(col);
	glRasterPos2f(x, y);
	while (*string) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
	}
}

static void display_string_3d(char *string, float *pos, const uint8_t *col)
{
	glColor3ubv(col);
	glRasterPos3fv(pos);
	while (*string) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
	}
}

#define MAX_STR_LEN	100

/**
 * \brief This function draw text information about scene
 */
static void display_text_info(void)
{
	static int screen_counter = 0;
	static struct timeval tv, last;
	static float sfps = 0;
	float fps;
	int tmp;
	char str_info[MAX_STR_LEN];
	short x_pos = 0, y_pos = 0;

	x_pos = 10;
	y_pos = 5;

	/* Draw help */
	y_pos += 15;
	tmp = snprintf(str_info, MAX_STR_LEN-1,
			"ESC: Quit, F: Fullscreen, LMB: Pan, RMB: Move, MMB: Zoom");
	str_info[tmp] = '\0';
	display_string_2d(str_info, x_pos, y_pos, white_col);

	/* Compute value of FPS */
	if(screen_counter == 0) {
		gettimeofday(&last, NULL);
		fps = 0.0;
	} else {
		unsigned long int delay;
		gettimeofday(&tv, NULL);
		delay = (tv.tv_sec - last.tv_sec)*1000000 + (tv.tv_usec - last.tv_usec);
		fps = (float)1000000.0/(float)delay;
		if(sfps == 0) {
			sfps = fps;
		} else {
			sfps = (1 - 0.1)*sfps + (0.1)*fps;
		}
		last.tv_sec = tv.tv_sec;
		last.tv_usec = tv.tv_usec;
	}

	/* Draw smoothed FPS */
	y_pos += 20;
	tmp = snprintf(str_info, MAX_STR_LEN-1, "FPS: %5.1f", sfps);
	str_info[tmp] = '\0';
	display_string_2d(str_info, x_pos, y_pos, white_col);

	screen_counter++;
}

static void display_cube(void)
{
	glBegin(GL_QUADS);
	{
		glNormal3f( 0.0,  0.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f( 1.0, -1.0, -1.0);
		glVertex3f( 1.0,  1.0, -1.0);
		glVertex3f(-1.0,  1.0, -1.0);
	}
	{
		glNormal3f( 1.0,  0.0,  0.0);
		glVertex3f( 1.0, -1.0, -1.0);
		glVertex3f( 1.0,  1.0, -1.0);
		glVertex3f( 1.0,  1.0,  1.0);
		glVertex3f( 1.0, -1.0,  1.0);
	}
	{
		glNormal3f( 0.0,  1.0,  0.0);
		glVertex3f( 1.0,  1.0, -1.0);
		glVertex3f(-1.0,  1.0, -1.0);
		glVertex3f(-1.0,  1.0,  1.0);
		glVertex3f( 1.0,  1.0,  1.0);
	}
	{
		glNormal3f(-1.0,  0.0,  0.0);
		glVertex3f(-1.0,  1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0,  1.0);
		glVertex3f(-1.0,  1.0,  1.0);
	}
	{
		glNormal3f( 0.0, -1.0,  0.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f( 1.0, -1.0, -1.0);
		glVertex3f( 1.0, -1.0,  1.0);
		glVertex3f(-1.0, -1.0,  1.0);
	}
	{
		glNormal3f( 0.0, 0.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f( 1.0, -1.0, 1.0);
		glVertex3f( 1.0,  1.0, 1.0);
		glVertex3f(-1.0,  1.0, 1.0);
	}
	glEnd();
}


/**
 * \brief This function displays 3d scene
 */
static void glut_on_display(void)
{
	float cube_pos[3] = {1.1, 1.1, 1.1};
/*	const float eps = 1.0e-6;*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, ctx->display->window.width, ctx->display->window.height);

#if 0
	/* Draw background gradient */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_ALWAYS);
	glBegin(GL_QUADS);
	glColor4fv(ctx->display->canvas.bg_col_grad_bottom);
	glVertex3f(-1.0f, -1.0f, -1.0f + eps);
	glColor4fv(ctx->display->canvas.bg_col_grad_bottom);
	glVertex3f( 1.0f, -1.0f, -1.0f + eps);
	glColor4fv(ctx->display->canvas.bg_col_grad_top);
	glVertex3f( 1.0f,  1.0f, -1.0f + eps);
	glColor4fv(ctx->display->canvas.bg_col_grad_top);
	glVertex3f(-1.0f,  1.0f, -1.0f + eps);
	glEnd();
#endif

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, ctx->display->window.width, 0, ctx->display->window.height);
	glScalef(1, -1, 1);
	glTranslatef(0, -ctx->display->window.height, 0);

	/* BEGIN: Drawing of 2D stuff */

	display_text_info();

	/* END: Drawing of 2D stuff */

	/* Set projection matrix for 3D stuff here */
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluPerspective(ctx->display->camera.field_of_view,
			(double)ctx->display->window.width/(double)ctx->display->window.height,
			ctx->display->camera.near_clipping_plane,
			ctx->display->camera.far_clipping_plane);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(ctx->display->camera.pos[0], ctx->display->camera.pos[1], ctx->display->camera.pos[2],
			ctx->display->camera.target[0], ctx->display->camera.target[1], ctx->display->camera.target[2],
			ctx->display->camera.up[0], ctx->display->camera.up[1], ctx->display->camera.up[2]);
	glPushMatrix();

	/* BEGIN: Drawing of 3d staff */

	display_string_3d("Cube", cube_pos, white_col);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	display_cube();

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	/* END: Drawing of 3d staff */

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glFlush();
	glutSwapBuffers();
}

/**
 * \brief Display scene in regular periods
 */
static void glut_on_timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(1000/ctx->fps, glut_on_timer, value);
}

/**
 * Callback function called on window resize
 */
static void glut_on_resize(int w, int h)
{
	ctx->display->window.width = w;
	ctx->display->window.height = h;
}

/**
 * \brief This function is called on mouse click
 */
static void glut_on_mouse_click(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON) {
		/* Start rotate of camera */
		if(state == GLUT_DOWN) {
			ctx->display->mouse.state = GLUT_LEFT_BUTTON;
			ctx->display->mouse.pos[0] = x;
			ctx->display->mouse.pos[1] = y;
		} else {
			ctx->display->mouse.state = -1;
		}
	} else if(button == GLUT_RIGHT_BUTTON) {
		/* Start move of camera */
		if(state == GLUT_DOWN) {
			ctx->display->mouse.state = GLUT_RIGHT_BUTTON;
			ctx->display->mouse.pos[0] = x;
			ctx->display->mouse.pos[1] = y;
		} else {
			ctx->display->mouse.state = -1;
		}
	} else if(button == GLUT_MIDDLE_BUTTON) {
		/* Start zoom in/out of camera */
		/* Start rotate of camera */
		if(state == GLUT_DOWN) {
			ctx->display->mouse.state = GLUT_MIDDLE_BUTTON;
			ctx->display->mouse.pos[0] = x;
			ctx->display->mouse.pos[1] = y;
		} else {
			ctx->display->mouse.state = -1;
		}
	}
}

/**
 * \brief This function is called on mouse move
 */
static void glut_on_mouse_drag(int x, int y)
{
	float dx, dy;

	if(ctx->display->mouse.state == GLUT_LEFT_BUTTON) {
		float rel_pos[3];
		float sphere[3];

		rel_pos[0] = ctx->display->camera.pos[0] - ctx->display->camera.target[0];
		rel_pos[1] = ctx->display->camera.pos[1] - ctx->display->camera.target[1];
		rel_pos[2] = ctx->display->camera.pos[2] - ctx->display->camera.target[2];

		cartesion_to_spherical(rel_pos, sphere);

		dx = x - ctx->display->mouse.pos[0];
		dy = y - ctx->display->mouse.pos[1];

		if(abs(dy) < 100) {
			sphere[1] -= (float)dy/100.0;
		}
		if(abs(dx) < 100) {
			sphere[2] -= (float)dx/100.0;
		}

		spherical_to_cartesian(sphere, rel_pos);

		ctx->display->camera.pos[0] = rel_pos[0] + ctx->display->camera.target[0];
		ctx->display->camera.pos[1] = rel_pos[1] + ctx->display->camera.target[1];
		ctx->display->camera.pos[2] = rel_pos[2] + ctx->display->camera.target[2];

		ctx->display->mouse.pos[0] = x;
		ctx->display->mouse.pos[1] = y;

	} else if(ctx->display->mouse.state == GLUT_RIGHT_BUTTON) {
		float fdx, fdy;

		fdx = ctx->display->camera.target[0] - ctx->display->camera.pos[0];
		fdy = ctx->display->camera.target[1] - ctx->display->camera.pos[1];

		dx = x - ctx->display->mouse.pos[0];
		dy = y - ctx->display->mouse.pos[1];

		if(abs(dx) < 100) {
			ctx->display->camera.target[0] -= 0.001*fdy*dx;
			ctx->display->camera.pos[0]    -= 0.001*fdy*dx;

			ctx->display->camera.target[1] -= 0.001*(-fdx)*dx;
			ctx->display->camera.pos[1]    -= 0.001*(-fdx)*dx;
		}

		ctx->display->mouse.pos[0] = x;
		ctx->display->mouse.pos[1] = y;

	} else if(ctx->display->mouse.state == GLUT_MIDDLE_BUTTON) {
		float rel_pos[3];
		float sphere[3];

		rel_pos[0] = ctx->display->camera.pos[0] - ctx->display->camera.target[0];
		rel_pos[1] = ctx->display->camera.pos[1] - ctx->display->camera.target[1];
		rel_pos[2] = ctx->display->camera.pos[2] - ctx->display->camera.target[2];

		cartesion_to_spherical(rel_pos, sphere);

		dx = x - ctx->display->mouse.pos[0];
		dy = y - ctx->display->mouse.pos[1];

		if(abs(dy) < 100) {
			sphere[0] += (float)dy/5.0;
		}

		spherical_to_cartesian(sphere, rel_pos);

		ctx->display->camera.pos[0] = rel_pos[0] + ctx->display->camera.target[0];
		ctx->display->camera.pos[1] = rel_pos[1] + ctx->display->camera.target[1];
		ctx->display->camera.pos[2] = rel_pos[2] + ctx->display->camera.target[2];

		ctx->display->mouse.pos[0] = x;
		ctx->display->mouse.pos[1] = y;
	}

}

/**
 * \brief This function handle keyboard
 */
void glut_on_keyboard(unsigned char key, int x, int y)
{
	/* Was key pressed inside window? */
	if((x > 0) && (x < ctx->display->window.width) &&
			(y > 0) && (y < ctx->display->window.height)) {
		key = (key > 'A' && key <= 'Z') ? key + 'a' - 'A' : key;

		switch (key) {
		case 27:
			exit(0);
			break;
		case 'f':
			if(ctx->display->window.fullscreen == 0) {
				glutFullScreen();
				ctx->display->window.fullscreen = 1;
			} else {
				ctx->display->window.fullscreen = 0;
				glutReshapeWindow(ctx->display->window.width, ctx->display->window.height);
				glutPositionWindow(ctx->display->window.left, ctx->display->window.top);
			}
			break;
		default:
			break;
		}
	}
}

/**
 * \brief This function set up glut callback functions and basic settings
 */
static void glut_init(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(ctx->display->window.width, ctx->display->window.height);
	glutInitWindowPosition(ctx->display->window.left, ctx->display->window.top);
	glutCreateWindow("GLUT Template");
	glutDisplayFunc(glut_on_display);
	glutReshapeFunc(glut_on_resize);
	glutKeyboardFunc(glut_on_keyboard);
	glutMouseFunc(glut_on_mouse_click);
	glutMotionFunc(glut_on_mouse_drag);
	glutTimerFunc(1000/ctx->fps, glut_on_timer, 0);
	gl_init();
}

/**
 * \brief This function start GLUT never ending loop
 */
void display_loop(struct CTX *_ctx, int argc, char *argv[])
{
	ctx = _ctx;

	glut_init(argc, argv);

	if(ctx != NULL) {
		glutMainLoop();
	}
}

/**
 * \brief This function create new structure holding informations about display
 */
struct Display *create_display(void)
{
	struct Display *disp = NULL;

	disp = (struct Display*)malloc(sizeof(struct Display));

	disp->window.fullscreen = 0;
	disp->window.left = 100;
	disp->window.top = 100;
	disp->window.width = 800;
	disp->window.height = 600;

	disp->mouse.state = -1;
	disp->mouse.pos[0] = disp->mouse.pos[1] = 0;

	disp->camera.field_of_view = 45.0;
	disp->camera.near_clipping_plane = 0.1;
	disp->camera.far_clipping_plane = 200.0;

	disp->camera.pos[0] = 5.0;
	disp->camera.pos[1] = -5.0;
	disp->camera.pos[2] = 5.0;

	disp->camera.target[0] = 0.0;
	disp->camera.target[1] = 0.0;
	disp->camera.target[2] = 0.0;

	disp->camera.up[0] = 0.0;
	disp->camera.up[1] = 0.0;
	disp->camera.up[2] = 1.0;

	disp->light.ambient[0] = disp->light.ambient[1] = disp->light.ambient[2] = 0.7;
	disp->light.ambient[3] = 1.0;
	disp->light.diffuse[0] = disp->light.diffuse[1] = disp->light.diffuse[2] = 0.9;
	disp->light.diffuse[3] = 1.0;
	disp->light.specular[0] = disp->light.specular[1] = disp->light.specular[2] = 0.5;
	disp->light.specular[3] = 1.0;
	disp->light.pos[0] = 0.0;
	disp->light.pos[1] = 0.0;
	disp->light.pos[2] = 5.0;
	disp->light.pos[3] = 1.0;

	disp->material.ambient[0] = disp->material.ambient[1] = disp->material.ambient[2] = 0.2;
	disp->material.ambient[3] = 1.0;
	disp->material.diffuse[0] = disp->material.diffuse[1] = disp->material.diffuse[2] = 0.3;
	disp->material.diffuse[3] = 1.0;
	disp->material.specular[0] = disp->material.specular[1] = disp->material.specular[2] = 1.0;
	disp->material.specular[3] = 0.5;
	disp->material.shininess = 20.0;

	disp->canvas.bg_col_grad_bottom[0] = 0.15;
	disp->canvas.bg_col_grad_bottom[1] = 0.15;
	disp->canvas.bg_col_grad_bottom[2] = 0.15;
	disp->canvas.bg_col_grad_bottom[3] = 1.0;
	disp->canvas.bg_col_grad_top[0] = 0.4;
	disp->canvas.bg_col_grad_top[1] = 0.4;
	disp->canvas.bg_col_grad_top[2] = 0.4;
	disp->canvas.bg_col_grad_top[3] = 1.0;

	disp->canvas.point_size = 2.0;
	disp->canvas.line_width = 1.0;

	return disp;
}

