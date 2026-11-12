# void-localed
Limited localed service implementation for void linux, used by gdm to set the keymap.  
The X11 keymap is read from `/etc/vconsole.conf` using the `XKBLAYOUT`, `XKBVARIANT`, `XKBMODEL`, and `XKBOPTIONS` entries. If an entry is missing, the corresponding `DEFAULT_*` value in `src/locale1.c` is used as a fallback.

# Build and install instructions
dependencies: base-devel glib-devel  
Standard installation using
```
> make
> sudo make install
> sudo cp org.freedesktop.locale1.conf /etc/dbus-1/system.d/
# create runit service that executes /usr/local/bin/void-localed and reboot
```
To change the keymap, edit `/etc/vconsole.conf` and restart the service.

# Uninstall instructions
Standard removal using
```
> sudo make uninstall
> sudo rm /etc/dbus-1/system.d/org.freedesktop.locale1.conf
# remove runit service
```

# Creating the runit service

```
> sudo mkdir /etc/sv/void-localed
> sudo tee /etc/sv/void-localed/run > /dev/null << 'EOF'
#!/bin/sh
exec /usr/local/bin/void-localed
EOF
> sudo chmod +x /etc/sv/void-localed/run
> sudo ln -s /etc/sv/void-localed/ /var/service/
```
