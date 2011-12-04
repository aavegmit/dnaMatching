#include "hasher.h"

int sock ;
unsigned char *ref_buffer ;

int BindRawSocketToInterface(char *device, int rawsock, int protocol)
{
    struct sockaddr_ll sll;
    struct ifreq ifr;

    bzero(&sll, sizeof(sll));
    bzero(&ifr, sizeof(ifr));

    strncpy((char *)ifr.ifr_name, device, IFNAMSIZ);
    if((ioctl(rawsock, SIOCGIFINDEX, &ifr)) == -1)
    {
        printf("Error getting Interface index !\n");
        exit(-1);
    }

    /* Bind our raw socket to this interface */

    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(protocol); 

    if((bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)))== -1){
        perror("Error binding raw socket to interface\n");
        exit(-1);
    }
    return 1;
}

void CreateEthernetHeader(struct frame *header, char *dst_mac, int protocol)
{
    /* copy the Dst mac addr */
    memcpy(header->ether_dhost, (void *)ether_aton(dst_mac), 6);

    /* copy the protocol */
    header->ether_type = htons(protocol);
}

void init_sender(char *interface){
    struct frame ref_header ;
    sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1) { 
        printf("when opening socket in PAListener");
    }
    BindRawSocketToInterface(interface, sock, ETH_P_ALL);

    ref_header.type = TYPE_REF ;
    CreateEthernetHeader(&ref_header, RTR_HASHER_ETHER, ETH_P_ALL);
    ref_buffer = (unsigned char *)malloc(sizeof(ref_header) + sizeof(struct content_ref)) ;
    memcpy(ref_buffer, &ref_header, sizeof(ref_header)) ;
}

void sendRefSeq(bitset<SUBSEQ_SIZE> bits, uint32_t offset, string chrome){
    static int counter = 0;
    struct content_ref content ;
    content.subseq = bits ;
    content.offset = offset ;
    strncpy(content.chr, chrome.c_str(), sizeof(content.chr)) ;

    memcpy(&ref_buffer[sizeof(struct frame)], &content, sizeof(content)) ;

    // Send the content
    if(write(sock,ref_buffer,sizeof(struct frame) + sizeof(struct content_ref)) < 0){
	perror("sendto");
    }
    counter++;
    printf("Sent packets are: %d\r", counter);
    fflush(stdout);
}

