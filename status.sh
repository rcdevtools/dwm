#!/bin/sh
# SPDX-FileCopyrightText: 2023 Jason Pena Jason Pena <jasonpena@awkless.com>
# SPDX-License-Identifier: MIT

imperial_date_convert() {
  year="$(date -d "now" +'%Y')"
  day="$(date -d "now" +'%j')"
  hour="$(date -d "now" +'%H')"

  chkn=0

  makr="0.11407955"
  yfrac="$(echo "(($day - 1) * 24 + $hour) * $makr" | bc -l)"
  yfrac="${yfrac%.*}" # Round down...

  milln="$(echo "$year / 1000 + 1" | bc -l)"
  milln="${milln%.*}" # Round down...

  myear="$(echo "$year - ($milln - 1) * 1000" | bc -l)"

  printf "%1d%03d%03d.M%02d\n" "$chkn" "$yfrac" "$myear" "$milln"
}

IDATE_STATUS="/tmp/idate.status"
imperial_date_status() {
  if [ -e "$IDATE_STATUS" ]; then
    rm -rf "$IDATE_STATUS"
  fi

  touch "$IDATE_STATUS"

  idate_icon="󰇧"
  while true; do
    idate_status="$(imperial_date_convert)"
    printf "%s %s\n" "$idate_icon" "$idate_status" >"$IDATE_STATUS"
    sleep 3600
  done &
}

GDATE_STATUS="/tmp/gdate.status"
gregorian_date_status() {
  if [ -e "$GDATE_STATUS" ]; then
    rm -rf "$GDATE_STATUS"
  fi

  touch "$GDATE_STATUS"

  gdate_icon="󱑁"
  while true; do
    gdate_status="$(date +'%Y-%m-%d %H:%M:%S')"
    printf "%s %s\n" "$gdate_icon" "$gdate_status" >"$GDATE_STATUS"
    sleep 1
  done &
}

BAT_STATUS="/tmp/bat.status"
battery_status() {
  if [ -e "$BAT_STATUS" ]; then
    rm -rf "$BAT_STATUS"
  fi

  touch "$BAT_STATUS"

  while true; do
    bat_icon="󰁹"
    bat_stat="$(</sys/class/power_supply/BAT0/status)"
    case "$bat_stat" in
      "Discharging")
        bat_icon="󱧥"
        ;;
      "Charging")
        bat_icon="󰂄"
        ;;
      *)
        bat_icon="󰁹"
        ;;
    esac
    bat_cap="$(</sys/class/power_supply/BAT0/capacity)"

    printf "%s %3s%%\n" "$bat_icon" "$bat_cap" >"$BAT_STATUS"
    sleep 5
  done &
}

NWIFI_STATUS="/tmp/nwifi.status"
wifi_name_status() {
  if [ -e "$NWIFI_STATUS" ]; then
    rm -rf "$NWIFI_STATUS"
  fi

  touch "$NWIFI_STATUS"

  nwifi_icon="󰀂"
  while true; do
    nwifi_status="$(nmcli device | grep wifi\  | awk '{print $1}')"
    printf "%s %s\n" "$nwifi_icon" "$nwifi_status" >"$NWIFI_STATUS"
    sleep 60
  done &
}

SSID_WIFI_STATUS="/tmp/ssid_wifi.status"
ssid_wifi_status()
{
	if [ -e "$SSID_WIFI_STATUS" ]; then
		rm -rf "$SSID_WIFI_STATUS"
	fi

	touch "$SSID_WIFI_STATUS"

  ssid_icon="󰛳"
  while true; do
    ssid_status="$(nmcli -f IN-USE,SSID dev wifi |
			awk '/^\*/{if (NR!=1) {print $2}}')"
    printf "%s %3s\n" "$ssid_icon" "$ssid_status" >"$SSID_WIFI_STATUS"
    sleep 1
  done &
}

SWIFI_STATUS="/tmp/swifi.status"
wifi_strength_status() {
  if [ -e "$SWIFI_STATUS" ]; then
    rm -rf "$SWIFI_STATUS"
  fi

  touch "$SWIFI_STATUS"

  swifi_icon="󱂇"
  while true; do
    swifi_status="$(nmcli -f IN-USE,SIGNAL dev wifi |
      awk '/^\*/{if (NR!=1) {print $2}}')"
    printf "%s %3s\n" "$swifi_icon" "$swifi_status" >"$SWIFI_STATUS"
    sleep 1
  done &
}

VOL_STATUS="/tmp/vol.status"
volume_status() {
  if [ -e "$VOL_STATUS" ]; then
    rm -rf "$VOL_STATUS"
  fi

  touch "$VOL_STATUS"

  while true; do
    if [ "$(pamixer --get-mute)" = "true" ]; then
      vol_icon="󰖁"
    else
      vol_icon="󰕾"
    fi

    vol_status="$(pamixer --get-volume)"
    printf "%s %3d%%\n" "$vol_icon" "$vol_status" >"$VOL_STATUS"
    sleep 1
  done &
}

CPU_STATUS="/tmp/cpu.status"
cpu_status() {
  if [ -e "$CPU_STATUS" ]; then
    rm -rf "$CPU_STATUS"
  fi

  touch "$CPU_STATUS"

  cpu_icon=""
  while true; do
    cpu_status="$(top -b -n1 | grep "Cpu(s)" | awk '{print $2 + $4}')"
    printf "%s %3.0f%%\n" "$cpu_icon" "$cpu_status" >"$CPU_STATUS"
    sleep 1
  done &
}

MEM_STATUS="/tmp/mem.status"
memory_status() {
  if [ -e "$MEM_STATUS" ]; then
    rm -rf "$MEM_STATUS"
  fi

  touch "$MEM_STATUS"

  mem_icon="󱊞"
  while true; do
    mem_full="$(free -hg | grep Mem | awk '{print $2}')"
    mem_full="${mem_full%"Gi"}"
    mem_used="$(free -hg | grep Mem | awk '{print $3}')"
    mem_used="${mem_used%"Gi"}"
    printf "%s %s/%s GB\n" "$mem_icon" "$mem_used" "$mem_full" >"$MEM_STATUS"
    sleep 60
  done &
}

imperial_date_status
gregorian_date_status
battery_status
wifi_name_status
ssid_wifi_status
wifi_strength_status
volume_status
cpu_status
memory_status

trap 'jobs -p | xargs kill' EXIT

while true; do
  idate="$(<"$IDATE_STATUS")"
  gdate="$(<"$GDATE_STATUS")"
  bat="$(<"$BAT_STATUS")"
  nwifi="$(<"$NWIFI_STATUS")"
	ssidw="$(<"$SSID_WIFI_STATUS")"
  swifi="$(<"$SWIFI_STATUS")"
  vol="$(<"$VOL_STATUS")"
  cpu="$(<"$CPU_STATUS")"
  mem="$(<"$MEM_STATUS")"
  status="$(echo "[$vol | $bat | $cpu | $mem];[$nwifi | $ssidw | $swifi];[$gdate | $idate]")"
  xsetroot -name "$status"
  sleep 1
done
