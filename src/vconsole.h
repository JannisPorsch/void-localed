#ifndef __VCONSOLE_H__
#define __VCONSOLE_H__

#include <gio/gio.h>

GVariant *read_vconsole_value(const gchar *key, const gchar *fallback);

#endif // !__VCONSOLE_H__
