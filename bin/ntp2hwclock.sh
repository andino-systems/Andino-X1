#!/bin/bash

# Location of logfile
LOGFILE="/usr/local/oeebox/etc/log/ntp.log"

if [ ! -f $LOGFILE ]; then
  touch $LOGFILE
fi

# Set the maximum allowed difference in seconds between Hw-Clock and Sys-Clock
maxDiffSec="2"

msgNoConnection="No connection to time-server"
msgConnection="Connection to time-server"

# Check for NTP connection
if ( ntpq -p | grep -q "^*"  ); then
        echo $msgConnection >> $LOGFILE
        echo "---------------------------------"  >> $LOGFILE

        secHwClock=$(sudo hwclock --debug | grep "^Hw clock time" | awk '{print $(NF-3)}')
        echo "HwClock: $secHwClock sec" >> $LOGFILE

        secSysClock=$(date +"%s")
        echo "SysClock: $secSysClock sec" >> $LOGFILE
        echo "---------------------------------" >> $LOGFILE

        secDiff=$(($secHwClock-$secSysClock))

        # Compute absolute value
        if ( echo $secDiff | grep -q "-" ); then
            secDiff=$(echo $secDiff | cut -d "-" -f 2)
        fi

        echo "Difference: $secDiff sec" >> $LOGFILE

        msgDiff="HwClock difference: $secDiff sec"
        if [ "$secDiff" -gt "$maxDiffSec" ] ; then
                echo "---------------------------------" >> $LOGFILE
                echo "The difference between Hw- and Sys-Clock is more than $maxDiffSec sec." >> $LOGFILE
                echo "Hw-Clock will be updated" >> $LOGFILE

                # Update hwclock from system clock
                sudo hwclock -w
                msgDiff="$msgDiff --> HW-Clock updated." >> $LOGFILE
        fi
        if !(awk '/./{line=$0} END{print line}' $LOGFILE | grep -q "$msgConnection") || [ "$secDiff" -gt "$maxDiffSec" ]; then
                echo $(date)": "$msgConnection". "$msgDiff >> $LOGFILE
        fi
else
        # No NTP connection
        echo $msgNoConnection
        if !(awk '/./{line=$0} END{print line}' $LOGFILE | grep -q "$msgNoConnection"); then
                echo $(date)": $msgNoConnection" >> $LOGFILE
        fi
fi
