# void-localed
Basic localed service implementation for void linux, used by gdm to set keymap.
The keymap is hard coded in main.c and there is no debugging, use at own will/risk

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
