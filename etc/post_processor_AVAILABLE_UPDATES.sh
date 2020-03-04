#!/bin/bash
#
# This script replaces the report token %%AVAILABLE_UPDATES%% with the number of available (security) updates
# It relies on update-notifier-common to refresh the list of updates daily and uses the apt-check utility to get the count of available updates.
#

all=$(/usr/lib/update-notifier/apt-check 2>&1 | cut -d ';' -f 1)
security=$(/usr/lib/update-notifier/apt-check 2>&1 | cut -d ';' -f 2)

if [ "$all" -gt 0 ]; then 
  sed -i "s/%%AVAILABLE_UPDATES%%/Available updates: $all. Security updates: $security/g" "$1"
else
  sed -i "s/%%AVAILABLE_UPDATES%%//g" "$1"
fi
