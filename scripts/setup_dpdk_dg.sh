#!/bin/bash
set -x
modprobe uio_pci_generic
cd /dpdk/dpdk/x86_64-native-linux-gcc
modprobe uio
insmod kmod/igb_uio.ko
/dpdk/dpdk/usertools/dpdk-devbind.py --bind=uio_pci_generic 0000:05:00.0 
/dpdk/dpdk/usertools/dpdk-devbind.py --status
echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
