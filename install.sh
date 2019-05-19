#!/bin/bash

export START_DIR=$(pwd)

echo 'Checking root privilege...'
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi

echo 'Creating new user << koouser >> with password << koo >> and adding it to sudoers file'
useradd -g users,audio,video,wheel,polkit -m koouser
printf "koo\nkoo\n" | passwd koouser

echo 'koouser ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

echo 'Creating a systemd daemon to automatically log in as the new user'
echo <<EOF > /etc/systemd/system/getty@tty1.service.d/override.conf
[Service]
ExecStart=
ExecStart=-/usr/bin/agetty --autologin koouser --noclear %I $TERM
EOF

echo 'Building project...'
if [[ -d build ]]; then
    rm -rf build
fi

mkdir build
cd build
cmake ..
make && echo "Installing kooReminder in koouser's home directory" && cp kooReminder ~koouser/kooReminder && cp -r ../src/backend ~koouser/

cd $START_DIR

echo 'switch from graphical.target to multi-user.target to disable X...'
sudo systemctl set-default multi-user.target

echo 'Creating a HOTSPOT profile'
nmcli con add type wifi ifname wlan0 con-name KooReminder autoconnect yes ssid KooReminder
nmcli con modify KooReminder 802-11-wireless.mode ap 802-11-wireless.band bg ipv4.method shared
nmcli con modify KooReminder wifi-sec.key-mgmt none

echo 'Creating .bash_profile and .xinitrc ...'
echo <<EOF > ~koouser/.bash_profile
# set up the hotspot (this is still in beta)
echo "Starting the hotspot"
nmcli connection up KooReminder

# starting up kooReminder and the node.js server
if [[ ! $DISPLAY && $XDG_VTNR -eq 1 ]]; then
  node ~/backend/index.js &
  exec startx
fi
EOF

echo <<EOF > ~koouser/.xinitrc
exec ~koouser/kooReminder
EOF

echo 'Setting up file permissions...'
chown -HR koouser.koouser ~koouser/* ~koouser/.* ~koouser/
