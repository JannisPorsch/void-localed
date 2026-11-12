#include "locale.h"
#include "common.h"
#include "vconsole.h"

#define BUS_NAME "org.freedesktop.locale1"
#define OBJECT_PATH "/org/freedesktop/locale1"

#define DEFAULT_LAYOUT "us"
#define DEFAULT_VARIANT ""
#define DEFAULT_MODEL "pc105"
#define DEFAULT_OPTIONS ""

static const gchar introspection_xml[] =
	"<node>"
	"    <interface name='org.freedesktop.locale1'>"
	"        <property type='s' name='X11Layout' access='read'/>"
	"        <property type='s' name='X11Variant' access='read'/>"
	"        <property type='s' name='X11Model' access='read'/>"
	"        <property type='s' name='X11Options' access='read'/>"
	"        <property type='as' name='Locale' access='read'/>"
	"        <property type='s' name='VConsoleKeymap' access='read'/>"
	"        <property type='s' name='VConsoleKeymapToggle' access='read'/>"
	"    </interface>"
	"</node>";

static GDBusNodeInfo *introspection_data = NULL;
static guint owner_id = 0;

/*
 * handle_get_property:
 * @con: D-Bus connection (unused).
 * @sender: unique name of the caller (unused).
 * @obj: object path (unused).
 * @iface: interface name (unused).
 * @property: name of the requested property.
 * @err: location for a D-Bus error when the property is unknown (unused
 *     values are not written).
 * @user_data: registration user data (unused).
 *
 * GDBus get-property callback for org.freedesktop.locale1. X11 properties are
 * read from /etc/vconsole.conf; Locale is always empty and the VConsole
 * keymap properties are always empty strings.
 *
 * Returns: a new GVariant with the property value, or NULL with @err set to
 *     G_DBUS_ERROR_UNKNOWN_PROPERTY for an unknown property.
 */
static GVariant *handle_get_property(GDBusConnection *con, const gchar *sender, const gchar *obj, const gchar *iface, const gchar *property,
		GError **err, gpointer user_data)
{
    UNUSED(con); UNUSED(sender); UNUSED(obj); UNUSED(iface); UNUSED(user_data);

	if (!g_strcmp0(property, "X11Layout")) return read_vconsole_value("XKBLAYOUT", DEFAULT_LAYOUT);
	if (!g_strcmp0(property, "X11Variant")) return read_vconsole_value("XKBVARIANT", DEFAULT_VARIANT);
	if (!g_strcmp0(property, "X11Model")) return read_vconsole_value("XKBMODEL", DEFAULT_MODEL);
	if (!g_strcmp0(property, "X11Options")) return read_vconsole_value("XKBOPTIONS", DEFAULT_OPTIONS);

	if (!g_strcmp0(property, "Locale")) return g_variant_new_strv((const gchar * const []){NULL}, 0);
	if (!g_strcmp0(property, "VConsoleKeymap") || !g_strcmp0(property, "VConsoleKeymapToggle")) return g_variant_new_string("");

	if (err)
	    g_set_error(err, G_DBUS_ERROR, G_DBUS_ERROR_UNKNOWN_PROPERTY, "Unknown property %s", property);

	return NULL;
}

/*
 * handle_method_call:
 * @con: D-Bus connection (unused).
 * @sender: unique name of the caller (unused).
 * @obj: object path (unused).
 * @iface: interface name (unused).
 * @method: name of the called method (unused).
 * @params: method parameters (unused).
 * @invocation: invocation used to send the reply.
 * @user_data: registration user data (unused).
 *
 * GDBus method-call callback. All methods succeed without doing anything and
 * return an empty reply.
 */
static void handle_method_call(GDBusConnection *con, const gchar *sender, const gchar *obj, const gchar *iface, const gchar *method,
		GVariant *params, GDBusMethodInvocation *invocation, gpointer user_data)
{
    UNUSED(con); UNUSED(sender); UNUSED(obj); UNUSED(iface); UNUSED(method); UNUSED(params); UNUSED(user_data);
	g_dbus_method_invocation_return_value(invocation, NULL);
}

static const GDBusInterfaceVTable iface_vtable =
{
	handle_method_call,
	handle_get_property,
	NULL,
	{ 0 }
};

/*
 * on_bus_acquired:
 * @con: connection to the system bus.
 * @name: acquired bus name (unused).
 * @user_data: user data passed to g_bus_own_name() (unused).
 *
 * Registers the org.freedesktop.locale1 object at
 * /org/freedesktop/locale1 using the introspection data. The process exits if
 * registration fails.
 */
static void on_bus_acquired(GDBusConnection *con, const gchar *name, gpointer user_data)
{
    UNUSED(name); UNUSED(user_data);
	GError *err = NULL;
	if (!g_dbus_connection_register_object(con, OBJECT_PATH, introspection_data->interfaces[0], &iface_vtable, NULL, NULL, &err))
	{
		g_printerr("void-localed: failed to register object: %s\n", err->message);
		g_error_free(err);
		exit(1);
	}
}

/*
 * on_name_acquired:
 * @con: connection to the system bus (unused).
 * @name: acquired bus name (unused).
 * @user_data: user data passed to g_bus_own_name() (unused).
 *
 * Called once the bus name is owned. Nothing needs to be done, so it returns
 * immediately.
 */
static void on_name_acquired(GDBusConnection *con, const gchar *name, gpointer user_data)
{
    UNUSED(con); UNUSED(name); UNUSED(user_data);
    return;
}


/*
 * on_name_lost:
 * @con: connection to the system bus (unused).
 * @name: lost bus name (unused).
 * @user_data: user data passed to g_bus_own_name() (unused).
 *
 * Called when the bus name cannot be acquired or is lost. Prints an error and
 * exits with status 1, since the service is useless without the name.
 */
static void on_name_lost(GDBusConnection *con, const gchar *name, gpointer user_data)
{
    UNUSED(con); UNUSED(name); UNUSED(user_data);
	g_printerr("void-localed: lost bus name, exiting\n");
	exit(1);
}

/*
 * locale1_start:
 * @error: location for a GError on failure.
 *
 * Parses the D-Bus introspection XML and requests ownership of
 * org.freedesktop.locale1 on the system bus.
 *
 * Returns: TRUE on success, FALSE with @error set if the introspection XML
 *     cannot be parsed or the bus name cannot be owned.
 */
gboolean locale1_start(GError **error)
{
	introspection_data = g_dbus_node_info_new_for_xml(introspection_xml, error);
	if (!introspection_data)
	{
		if (error && *error)
			g_prefix_error(error, "failed to parse introspection XML: ");
		else if (error)
			g_set_error(error, G_DBUS_ERROR, G_DBUS_ERROR_FAILED, "failed to parse introspection XML");

		return FALSE;
	}

	owner_id = g_bus_own_name(G_BUS_TYPE_SYSTEM, BUS_NAME, G_BUS_NAME_OWNER_FLAGS_NONE, on_bus_acquired, on_name_acquired, on_name_lost, NULL, NULL);
	if (!owner_id)
	{
		g_dbus_node_info_unref(introspection_data);
		introspection_data = NULL;
		if (error)
			g_set_error(error, G_DBUS_ERROR, G_DBUS_ERROR_FAILED, "failed to own %s", BUS_NAME);

		return FALSE;
	}

	return TRUE;
}

/*
 * locale1_stop:
 *
 * Releases the org.freedesktop.locale1 bus name and frees the introspection
 * data. Safe to call after a failed or successful locale1_start().
 */
void locale1_stop(void)
{
	if (owner_id)
	{
		g_bus_unown_name(owner_id);
		owner_id = 0;
	}

	if (introspection_data)
	{
		g_dbus_node_info_unref(introspection_data);
		introspection_data = NULL;
	}
}
