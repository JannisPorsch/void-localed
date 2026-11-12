#include "vconsole.h"

#define CONFIG_PATH "/etc/vconsole.conf"

/*
 * read_vconsole_value:
 * @key: key to look up in CONFIG_PATH (for example "XKBLAYOUT").
 * @fallback: value returned when the file or key is missing.
 *
 * Reads a value from CONFIG_PATH. The file has no group header, so a
 * synthetic [vconsole] group is prepended before parsing. Surrounding single
 * or double quotes are stripped from the value.
 *
 * Returns: a new string GVariant holding the value, or @fallback if the file
 *     or key is missing. An existing empty value is returned as empty.
 */
GVariant *read_vconsole_value(const gchar *key, const gchar *fallback)
{
	GKeyFile *kf = g_key_file_new();
	gchar *contents = NULL;
	gchar *value = NULL;
	GVariant *ret;

	if (g_file_get_contents(CONFIG_PATH, &contents, NULL, NULL))
	{
		gchar *data = g_strconcat("[vconsole]\n", contents, NULL);

		if (g_key_file_load_from_data(kf, data, -1, G_KEY_FILE_NONE, NULL))
			value = g_key_file_get_string(kf, "vconsole", key, NULL);

		g_free(data);
		g_free(contents);
	}

	if (value)
	{
		gsize len = strlen(value);

		if (len >= 2 && ((value[0] == '"' && value[len - 1] == '"') ||
				(value[0] == '\'' && value[len - 1] == '\'')))
		{
			gchar *unquoted = g_strndup(value + 1, len - 2);

			g_free(value);
			value = unquoted;
		}

		ret = g_variant_new_string(value);
		g_free(value);
	}
	else
		ret = g_variant_new_string(fallback);

	g_key_file_free(kf);
	return ret;
}
