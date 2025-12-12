# void-localed
Limited localed service implementation for void linux, used by gdm to set the keymap.  
The keymap is hard coded in main.c and should consist of values you would pass to setxkbmap.  
There is no logging or error checks, so use at own will/risk

# Build and install instructions
dependencies: base-devel glib-devel  
Standard installation using
```
# edit MY_LAYOUT, MY_VARIANT, MY_MODEL, MY_OPTIONS in src/main.c to your needs
> make
> sudo make install
> sudo cp org.freedesktop.locale1.conf /etc/dbus-1/system.d/
# create runit service that executes /usr/local/bin/void-localed and reboot
```

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
