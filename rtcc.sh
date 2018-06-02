#!/bin/sh

modprobe -v rtc_ds1307

if [ $? -eq 0 ]; then
  # rtc is connected and working, use it
  echo "RTC Hardware connected, getting time"
  systemctl disable ntp.service
  systemctl stop ntp.service
  echo "ds1307 0x68" > /sys/class/i2c-adapter/i2c-1/new_device
  hwclock -s
else
  # rtc is offline / not connected, get time via ntp
  echo "RTC Hardware disconnected/battery dead/offline, use NTP"
  systemctl enable ntp.service
  systemctl start ntp.service
fi



