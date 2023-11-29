#!/bin/sh -

autodetect_board() {
    if [ ! -d /proc/device-tree/ ];
    then
        echo "Proc Device tree are not available, Could not detect on which board we are" > /dev/kmsg
        exit 1
    fi

    LIST="##BOARDS_LIST##"
    board=""
    for b in $LIST; do
        board_lower=$(echo $b | tr '[:upper:]' '[:lower:]')
        if $(grep -q "$board_lower" /proc/device-tree/compatible) ;
        then
            echo "Board $board_lower is compatible"
            board=$b
            break
        fi
    done
    if [ -z "$board" ]; then
        echo "Board is not a valid BOARD (stm32mp257f-dk, stm32mp257f-ev1)" > /dev/kmsg
        exit 0
    fi
}

find_default_project() {
    DEFAULT_PROJECT=""
    if [ -n "$board" ]; then
        if [ -z "$(find @userfs_mount_point@/$board/ -name default)" ]; then
            echo "The default copro example for ${board} doesn't exist" > /dev/kmsg
            exit 1
        else
            default_path_project=$(find @userfs_mount_point@/$board/ -name default)
            DEFAULT_PROJECT=$(dirname $default_path_project)
        fi
    fi
}

firmware_load_start() {
    if [ -n "$DEFAULT_PROJECT" ]; then
        cd $DEFAULT_PROJECT
        ./fw_cortex_m33.sh start
        echo "Booting fw image for ${board}" > /dev/kmsg
    fi
}

firmware_load_stop() {
    # Stop the firmware
    if [ "$(cat /sys/class/remoteproc/remoteproc0/state)" = "running" ]; then
        if [ -n "$DEFAULT_PROJECT" ]; then
            cd $DEFAULT_PROJECT
            ./fw_cortex_m33.sh stop
        fi
        echo "Stopping fw image ${board}" > /dev/kmsg
    else
        echo "Default copro already stopped" > /dev/kmsg
    fi
}

board=""
autodetect_board
find_default_project

case "$1" in
start)
    firmware_load_stop
    firmware_load_start
    ;;
stop)
    firmware_load_stop
    ;;
restart)
    firmware_load_stop
    firmware_load_start
    ;;
*)
    echo "HELP: $0 [start|stop|restart]"
    ;;
esac

exit 0
