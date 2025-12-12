# void-localed
Basic localed service implementation with hard-coded keymap for void linux, used by gdm to set keymap

# Build and install instructions
Standard installation using
```
# edit MY_LAYOUT, MY_VARIANT, MY_MODEL, MY_OPTIONS in src/main.c to your needs
make
sudo make install
sudo cp org.freedesktop.locale1.conf /etc/dbus-1/system.d/
# create runit service that executes /usr/local/bin/void-localed
```

# uninstall instructions
Standard removal using
```
make clean
sudo make uninstall
sudo rm /etc/dbus-1/system.d/org.freedesktop.locale1.conf
```
