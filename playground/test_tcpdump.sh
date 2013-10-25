#!/bin/bash
# File : test_tcpdump.sh
# Compare the tcpdump data of wget and axget downloading same resource

# Set to 1 for pcap file
declare -i AX_CREATE_PCAP_FILE=0

# default values
AX_PROTOCOL="http"
AX_HOST="localhost"
AX_PORT="80"
AX_FILE="index.html"
AX_INTERFACE="lo"

# show usage
function usage
{
    cat << EOUSAGE
 Usage: test_tcpdump.sh [-c <protocol>] [-h <host name/ip>] [-p <port>] [-f <file path>] [-i <network interface>]
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

if [ "`echo ${AX_HOST} | grep -o '[0-9\.]\{7,\}$'`" == "${AX_HOST}" ]; then
    AX_IP=${AX_HOST}
else
    AX_IP=`nslookup "${AX_HOST}" | grep -A1 ${AX_HOST} | grep -o '[0-9\.]\{7,\}$' | head -1`
fi

# remove old files
rm -f w.* ax.*

# download using wget
if [ ${AX_CREATE_PCAP_FILE} -eq 1 ]; then
    tcpdump -XvvennSs 0 -i ${AX_INTERFACE} tcp port ${AX_PORT} and host ${AX_IP} -w w.pcap >/dev/null 2>&1 &
else
    tcpdump -XvvennSs 0 -i ${AX_INTERFACE} tcp port ${AX_PORT} and host ${AX_IP} > w.res 2>&1 &
fi
sleep 2 # wait tcpdump to setup
wget ${AX_PROTOCOL}://${AX_HOST}:${AX_PORT}/${AX_FILE} -O w.html >/dev/null 2>&1
sleep 3 # wait all package captured
kill -INT %1
sleep 2 # wait process to be kill
jobs >/dev/null 2>&1

# download using axget
if [ ${AX_CREATE_PCAP_FILE} -eq 1 ]; then
    tcpdump -XvvennSs 0 -i ${AX_INTERFACE} tcp port ${AX_PORT} and host ${AX_IP} -w ax.pcap >/dev/null 2>&1 &
else
    tcpdump -XvvennSs 0 -i ${AX_INTERFACE} tcp port ${AX_PORT} and host ${AX_IP} > ax.res 2>&1 &
fi
sleep 2 # wait tcpdump to setup
./axget ${AX_PROTOCOL}://${AX_HOST}:${AX_PORT}/${AX_FILE} -o ax.html >/dev/null 2>&1
sleep 3 # wait all package captured
kill -INT %1
sleep 2 # wait process to be kill
jobs >/dev/null 2>&1

# show files
ls -l w.* ax.*

