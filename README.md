# thpio

interface linux kernel - to bunch of sensor connected to arduino (1wire, dht11, adc, gpio)

 - arduino refreshes statuses of buses and sensors and gpio on its own at regular basis (check avr/hal.h: BUS_REFRESH_MS)
 - kernel module provides simple to use interface to arduino buses, sensors and gpio in both directions
 - access to sensors values is as simple as "cat /sys/module/thpo/drivers/usb:thpo/1-1.2:1.0/status" (usb port may change, check script _get_status for details)
 - send 1 to 0 bit of out port is as simple as echo A > /sys/module/thpo/drivers/usb:thpo/1-1.2:1.0/status
 - send 0 to 1 bit of out port is as simple as echo b > /sys/module/thpo/drivers/usb:thpo/1-1.2:1.0/status
 - arduino led is used as heartbeat

prepare hardware:

 - general linux machine (pc or rpi) with usb 
 - arduino nano on at mega 328 chip with ch341 usb-uart converter (or alike)
 - a set of sensors connected to arduino: 1wire, dht11, voltage values to measure (like mpx pressure sensors), binary in or out ports, check avr/hal.h for details

prepare software:

tested with ubuntu 14,16,17, rasbian based on wheezy, stretch

# get git
apt-get install git

# setup avr toolchain
apt-get install avrdude gcc-avr avr-libc

# setup kernel module development toolchain
apt-get install gcc make

# for raspbian (stretch):
apt-get install raspberrypi-kernel-headers

# for ubuntu/debian:
apt-get install linux-libc-dev

# get thpio sources
git clone https://github.com/novodim/thpio/

# start with preparing firmware
cd thpio/avr

# hack hal.h accordingly your hardware, use arduino schematics to convert arduino ports to normal avr mc ports
vi hal.h

# build firmware
make

# flash firmare to arduino with its bootloader, make sure arduion is on /dev/ttyUSB0, hack Makefile if not
make flash

# usually fuses do not need to be programmed if your arduion has working bootloader and clocked by external crystal at maximum frequency (16Mhz)
# get to building linux kernel module
cd ../drv

# build module, if ok then thpo.ko will appear in drv directory (make sure proper version of kernel-headers is installed)
make

# unload uart usb module for your arduino (in my case ch341 module)
rmmod ch341

# check that usb uart module is absent
lsmod | grep ch341

# check last lines of dmesg, should be somethins like:
# usbserial: USB Serial deregistering driver ch341-uart
# ch341-uart ttyUSB0: ch341-uart converter now disconnected from ttyUSB0
# usbcore: deregistering interface driver ch341
# ch341 1-1.2:1.0: device disconnected
dmesg | tail -n 20

# now load freshly compiled thpo.ko
insmod thpo.ko

# check output of kernel, should be something like:
# thpo_probe called sizeof status is: 10
# usbcore: registered new interface driver thpo
dmesg | tail -n 20

# get info last status of sensors from arduino:
./_get_status

### output should be like that:
```
status_size: 58
ds_info_noBus: 2
ds_info_busSize: 2
ds_busError: 0
dht_info: 2
dht_busError: 0
mpx_info: 1
mpx_busError: 0
in_info: 0
out_info: 0
ds-0-0-28ff864b6f140427: +22.7
ds-0-1-28c297b50500002c: +16.6
ds-1-0-28ffdf5b6a1403cf: +22.8
ds-1-1-0000000000000000: +0.0
dht-0-temp: 23
dht-0-hum: 35
dht-1-temp: 23
dht-1-hum: 35
mpx-0: 819
in: 0
out: 0
```
# in order to reflash arduino do the following:
rmmod thpo
cd ../avr
modprobe ch341
make flash
rmmod ch341
cd ../drv
insmod thpo.ko
./_get_status

