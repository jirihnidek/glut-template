/*
 *
 * ***** BEGIN BSD LICENSE BLOCK *****
 *
 * Copyright (c) 2014, Jiri Hnidek
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***** END BSD LICENSE BLOCK *****
 *
 * Authors: Jiri Hnidek <jiri.hnidek@tul.cz>
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <verse.h>

#include "main.h"
#include "display_glut.h"

/**
 * \brief Clean client context
 */
static void clean_client_ctx(struct CTX *ctx)
{
	(void)ctx;
}

/**
 * \brief Set default values of client context
 */
static void init_client_ctx(struct CTX *ctx)
{
	ctx->display = create_display();
	ctx->fps = DEDEFAULT_FPS;
}

/**
 * \brief Print help
 */
static void print_help(char *prog_name)
{
	printf("\n Usage: %s [OPTION...]\n", prog_name);
	printf("\n");
	printf("  This program is 3D GLUT template.\n");
	printf("\n");
	printf("  Options:\n");
	printf("   -f fps           use defined FPS value (default value is 25)\n");
	printf("   -h               display this help and exit\n");
	printf("\n");
}


int main(int argc, char *argv[])
{
	struct CTX ctx;
	int opt;

    init_client_ctx(&ctx);

	/* When client was started with some arguments */
	if(argc > 1) {
		/* Parse all options */
		while( (opt = getopt(argc, argv, "hf:")) != -1) {
			switch(opt) {
				case 'f':
					if(sscanf(optarg, "%u", &ctx.fps) != 1) {
						ctx.fps = DEDEFAULT_FPS;
					}
					break;
				case 'h':
					print_help(argv[0]);
					clean_client_ctx(&ctx);
					exit(EXIT_SUCCESS);
				case ':':
					clean_client_ctx(&ctx);
					exit(EXIT_FAILURE);
				case '?':
					clean_client_ctx(&ctx);
					exit(EXIT_FAILURE);
			}
		}

		/* Process remaining argument */
	}

#if 0
	if( pthread_create(&ctx.dummy_thread, NULL, dummy_loop, (void*)&ctx) != 0) {
		clean_client_ctx(&ctx);
		return EXIT_FAILURE;
	}
#endif

	display_loop(&ctx, argc, argv);

	/* Free client context */
	clean_client_ctx(&ctx);

	return EXIT_SUCCESS;
}
