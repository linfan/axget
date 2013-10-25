#!/bin/bash
# File : test_download.sh
# Compare download speed of wget and axget requesting same resource

# default values
AX_PROTOCOL="http"
AX_HOST="localhost"
AX_PORT="80"
AX_FILE="index.html"

# show usage
function usage
{
    cat << EOUSAGE
 Usage: test_download.sh [-c <protocol>] [-h <host name/ip>] [-p <port>] [-f <file path>]
EOUSAGE
    exit 0
}

# parse parameter pair
function handel_parameters
{
    case ${1} in
        -c ) AX_PROTOCOL=${2}
            ;;
        -h ) AX_HOST=${2}
            ;;
        -p ) AX_PORT=${2}
            ;;
        -f ) AX_FILE=${2}
            ;;
        -i ) AX_INTERFACE=${2}
            ;;
        ? ) echo "[ ERROR ] invalid parameter !"
            usage
            ;;
    esac
}

# parse parameters
while [ $# -ge 2 ]; do
    handel_parameters ${1} ${2}
    shift 2
done

echo "Downloading ${AX_PROTOCOL}://${AX_HOST}:${AX_PORT}/${AX_FILE} through ${AX_INTERFACE}"

# remove old files
rm -f ax.1.data ax.5.data w.data

echo "=== axget with 5 threads ==="
time ./axget -n 5 -o ax.5.data ${AX_PROTOCOL}://${AX_HOST}:${AX_PORT}/${AX_FILE} 2>&1 | grep 'B/s' | tail -1

echo "=== axget with 1 thread ==="
time ./axget -n 1 -o ax.1.data ${AX_PROTOCOL}://${AX_HOST}:${AX_PORT}/${AX_FILE} 2>&1 | grep 'B/s' | tail -1

echo "=== wget ==="
time wget -O w.data ${AX_PROTOCOL}://${AX_HOST}:${AX_PORT}/${AX_FILE} 2>&1 | grep 'B/s' | tail -1

# show file
ls -l ax.1.data ax.5.data w.data

