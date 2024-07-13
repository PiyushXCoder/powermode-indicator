# Powermode Indicator

It is a plugin for all desktops supported by libappindicator. It provides menu to choose power profiles.

I do use it on my system. I would love to see contributions and bug reports. 

> It depends on [power-profiles-daemon](https://archlinux.org/packages/extra/x86_64/power-profiles-daemon/)

![How many is shown](./screenshot.png)

## Installation

### Arch Linux
``` bash
paru -S powermode-indicator-git
```

### Ubuntu and Debian

Deb file has been provided in release. You can download and install.

### From sources

> The packages is made using cmake

#### You will need following dependencies:

* cmake > 3.16
* gtkmm-3.0
* glibmm-2.4
* giomm-2.4
* appindicator3-0.1
* gcc or clangd
* pkg-config

#### Steps:

* Clone and change to the project directory
```
cd powermode-indicator
```
* Make a build directory 
``` bash
mkdir build
```
* To build run following commands
``` bash
cmake -B build -S .
cmake --build build
```
* To install run following commands
``` bash
cmake --install build
```

## How to run and configure?

The indicator shows up in Status Tray once `powermode-indicator` command is executed.

I would recommend you to add it in autostart script.

## Troubleshoot

* The indicator doesn't show performace mode or other mode if that is not available on your laptop
* If icons are not shown. Try papirus-icon-theme, breeze-icons or any other complete icon pack
