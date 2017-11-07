#!/bin/bash

APP_NAME=$1
LOG_NAME=$2
STRACE=$3

if [[ $# -lt 1 ]]; then
    echo ">>> ERROR: Not enough parameters"
    cat <<-END
		./runvideowall.sh <APP_NAME> [LOG_NAME] [strace]
		Example:
		./runvideowall.sh Application-x86_64.AppImage.0509.1
		./runvideowall.sh Application-x86_64.AppImage.0509.1 "" strace
	END
	exit
fi

ulimit -c unlimited
sudo bash -c 'echo "core.%e.%p" > /proc/sys/kernel/core_pattern'

echo $APP_NAME "is running"

if [[ -z $LOG_NAME ]]; then
	$STRACE ./$APP_NAME > $APP_NAME.log 2>&1
else
	$STRACE ./$APP_NAME > $LOG_NAME.log 2>&1
fi



