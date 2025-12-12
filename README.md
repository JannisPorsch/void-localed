# void-localed
Basic localed service implementation for void linux, used by gdm to set keymap

# Build and install instructions
Standard installation using
```
make
sudo make install
sudo cp org.freedesktop.locale1.conf /etc/dbus-1/system.d/
```

# uninstall instructions
Standard remove using
```
make clean
sudo make uninstall
sudo rm /etc/dbus-1/system.d/org.freedesktop.locale1.conf
```
