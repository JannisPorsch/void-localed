#include <gio/gio.h>
#include <sys/types.h>

#define MY_LAYOUT "de"
#define MY_VARIANT ""
#define MY_MODEL "pc105"
#define MY_OPTIONS ""

#define UNUSED(x) (void)(x)

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

static GVariant *handle_get_property(GDBusConnection *con, const gchar *sender, const gchar *obj, const gchar *iface, const gchar *property,
		GError **err, gpointer user_data)
{
    UNUSED(con); UNUSED(sender); UNUSED(obj); UNUSED(iface); UNUSED(err); UNUSED(user_data);

	if (!g_strcmp0(property, "X11Layout")) return g_variant_new_string(MY_LAYOUT);
	if (!g_strcmp0(property, "X11Variant")) return g_variant_new_string(MY_VARIANT);
	if (!g_strcmp0(property, "X11Model")) return g_variant_new_string(MY_MODEL);
	if (!g_strcmp0(property, "X11Options")) return g_variant_new_string(MY_OPTIONS);

	if (!g_strcmp0(property, "Locale")) return g_variant_new_strv((const gchar * const []){NULL}, 0);
	return g_variant_new_string("");
}

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
	NULL
};

static void on_bus_acquired(GDBusConnection *con, const gchar *name, gpointer user_data)
{
    UNUSED(name); UNUSED(user_data);
	g_dbus_connection_register_object(con, "/org/freedesktop/locale1", introspection_data->interfaces[0], &iface_vtable, NULL, NULL, NULL);
}

static void on_name_acquired(GDBusConnection *con, const gchar *name, gpointer user_data)
{
    UNUSED(con); UNUSED(name); UNUSED(user_data);
    return;
}


static void on_name_lost(GDBusConnection *con, const gchar *name, gpointer user_data)
{
    UNUSED(con); UNUSED(name); UNUSED(user_data);
	exit(1);
}

int main(int argc, char **argv)
{
    UNUSED(argc); UNUSED(argv);
	GMainLoop *loop;
	guint owner_id;

	introspection_data = g_dbus_node_info_new_for_xml(introspection_xml, NULL);
	if (!introspection_data) return 1;

	owner_id = g_bus_own_name(G_BUS_TYPE_SYSTEM, "org.freedesktop.locale1", G_BUS_NAME_OWNER_FLAGS_NONE, on_bus_acquired, on_name_acquired, on_name_lost, NULL, NULL);

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_bus_unown_name(owner_id);
	g_dbus_node_info_unref(introspection_data);

	return 0;
}
