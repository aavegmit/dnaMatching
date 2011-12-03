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
#include <bitset>


#define FRAME_LEN 54
#define FILE_SIZE 1024
#define RTR_HASHER_ETHER "00:15:17:1e:04:30"
#define RTR_HASHER_INTERFACE "eth2"

#define READ_TYPE 0x4e
#define REFERENCE_TYPE 0x4f

#define INTERFACE_0 "eth0"
#define INTERFACE_1 "eth3"
#define INTERFACE_2 "eth4"
#define INTERFACE_3 "eth5"

#define INDEXER0_ETHER "00:15:17:1e:03:46"
#define INDEXER1_ETHER "00:16:36:ca:17:92"
#define INDEXER2_ETHER "00:15:17:1e:05:2e"
#define INDEXER3_ETHER "00:15:17:1e:03:3e"

#define NUM_HASHERS 1
#define TYPE_READ 0x4e
#define TYPE_REF  0x4f
#define SUBSEQ_SIZE 64
#define MSB_SUBSEQ 63

using namespace std ;

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
};

struct content_ref {
    bitset<SUBSEQ_SIZE> subseq ;
    uint32_t offset ;
} ;

struct content_read {
    bitset<SUBSEQ_SIZE> subseq ;
    bitset<104> read ;
} ;

void *sniffer(void *) ;
#endif
