#ifndef _VCONSOLE_H_
#define _VCONSOLE_H_

#include <gio/gio.h>

GVariant *read_vconsole_value(const gchar *key, const gchar *fallback);

#endif // !_VCONSOLE_H_
