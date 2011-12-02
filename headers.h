#ifndef _HEADERS_H
#define _HEADERS_H

#include <pthread.h>
#include <list>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <netinet/if_ether.h>
#include <netinet/ip_icmp.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include<linux/if_packet.h>
#include <netinet/ether.h>
#include <sys/mman.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FRAME_LEN 54
#define FILE_SIZE 1024
#define SRC_ETHER_ADDR "00:04:23:c5:d7:8e"
#define DST_ETHER_ADDR "00:11:43:d6:d6:18"

#define INTERFACE_1 "eth0"
#define INTERFACE_2 "eth1"
#define INTERFACE_3 "eth2"
#define INTERFACE_4 "eth3"

#define RTR_ETHER_1 "00:04:23:c7:a6:0e"
#define RTR_ETHER_2 "00:04:23:c7:a6:0f"
#define RTR_ETHER_3 "00:04:23:c7:a6:22"
#define RTR_ETHER_4 "00:04:23:c7:a6:23"

#define NODE0_ETHER "00:04:23:c7:a8:0a"
#define NODE1_ETHER "00:04:23:c5:d4:70"
#define NODE2_ETHER "00:04:23:c7:a6:78"
#define NODE3_ETHER "00:04:23:c7:a6:52"

struct sniff_ethernet {
    u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */
    u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */
    u_short ether_type;                     /* IP? ARP? RARP? etc */
};

struct frame {
    u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */
    u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */
    u_short ether_type;                     /* IP? ARP? RARP? etc */
    u_char type ;	// 0x4e - packet type for content based routing
    uint16_t len ;
    unsigned char buf[7] ;
};
void *sniffer(void *) ;
#endif
