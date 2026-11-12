#include "common.h"
#include "locale.h"

/*
 * main:
 * @argc: argument count (unused).
 * @argv: argument vector (unused).
 *
 * Program entry point. Starts the org.freedesktop.locale1 service on the
 * system bus and runs the main loop to serve property reads.
 *
 * Returns: 0 when the main loop ends normally, 1 if the service cannot be
 *     started or memory runs out.
 */
int main(int argc, char **argv)
{
    UNUSED(argc); UNUSED(argv);
	GError *err = NULL;
	GMainLoop *loop;

	if (!locale1_start(&err))
	{
		g_printerr("void-localed: %s\n", err->message);
		g_error_free(err);
		return 1;
	}

	loop = g_main_loop_new(NULL, FALSE);
	if (!loop)
	{
		g_printerr("void-localed: out of memory\n");
		locale1_stop();
		return 1;
	}

	g_main_loop_run(loop);
	locale1_stop();

	return 0;
}
