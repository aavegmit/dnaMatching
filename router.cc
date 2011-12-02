#include "headers.h"

int sock[4] ;
int BindRawSocketToInterface(char *device, int rawsock, int protocol)
{

    struct sockaddr_ll sll;
    struct ifreq ifr;

    bzero(&sll, sizeof(sll));
    bzero(&ifr, sizeof(ifr));

    /* First Get the Interface Index  */

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

    if((bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)))== -1)
    {
        perror("Error binding raw socket to interface\n");
        exit(-1);
    }
    return 1;
}


void CreateEthernetHeader(struct frame *header, char *src_mac, char *dst_mac, int protocol)
{
    
    //ethernet_header = (struct sniff_ethernet *)malloc(sizeof(struct sniff_ethernet));

    /* copy the Src mac addr */
    memcpy(header->ether_shost, (void *)ether_aton(src_mac), 6);

    /* copy the Dst mac addr */
    memcpy(header->ether_dhost, (void *)ether_aton(dst_mac), 6);

    /* copy the protocol */
    header->ether_type = htons(protocol);
}



int main(int argc, char **argv){
    FILE *fp ;

    for(int i = 0 ; i < 4 ; ++i){
	sock[i] = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock[i] == -1) { 
	    printf("when opening socket in PAListener");
	}
    }

    // Bind interfaces
    BindRawSocketToInterface(INTERFACE_1, sock[0], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_2, sock[1], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_3, sock[2], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_4, sock[3], ETH_P_ALL);

    pthread_t interfaceSniffer[4];

    int res ;
    for(int i = 0 ; i < 4 ; ++i){
	res = pthread_create(&interfaceSniffer[i], NULL, sniffer, (void *)sock[i]); 
	if( res != 0){
	    fprintf(stderr, "TCP read thread creation failed\n") ;
	    exit(EXIT_FAILURE) ;
	}
    }

    pthread_join(interfaceSniffer[0], NULL) ;
    pthread_join(interfaceSniffer[1], NULL) ;
    pthread_join(interfaceSniffer[2], NULL) ;
    pthread_join(interfaceSniffer[3], NULL) ;

}

void *sniffer(void *arg){
    int sockfd = (long)arg ;
    struct frame *header ;
    int length, tempsock ;
    unsigned char *buffer ;
    int ethhdr_len = sizeof(struct sniff_ethernet);
    int counter = 0;
    buffer = (unsigned char*)malloc(FRAME_LEN+ethhdr_len); 
    while(1){
	length = read(sockfd, buffer, FRAME_LEN + ethhdr_len);
	printf("Recived %d\r", counter) ;

	if(length != FRAME_LEN+ethhdr_len)
	    continue ;

	header = (struct frame *)(buffer);
	if(header->type != 0x4e)
	    continue ;
	if(!strcmp((char *)header->buf, "YELLOW")){
	    CreateEthernetHeader(header, RTR_ETHER_1, NODE0_ETHER, ETH_P_ALL);
	    tempsock = sock[0] ;
	}
	else if(!strcmp((char *)header->buf, "RED")){
	    CreateEthernetHeader(header, RTR_ETHER_2, NODE1_ETHER, ETH_P_ALL);
	    tempsock = sock[1] ;
	}
	else if(!strcmp((char *)header->buf, "BLUE")){
	    CreateEthernetHeader(header, RTR_ETHER_3, NODE2_ETHER, ETH_P_ALL);
	    tempsock = sock[2] ;
	}
	else if(!strcmp((char *)header->buf, "GREEN")){
	    CreateEthernetHeader(header, RTR_ETHER_4, NODE3_ETHER, ETH_P_ALL);
	    tempsock = sock[3] ;
	}
//	printf("Y: %d, R: %d, B: %d, G: %d\n", ycount, rcount, bcount, gcount) ;
        if(write(tempsock,header,ethhdr_len+FRAME_LEN) < 0){
            perror("sendto");
        }
	
    }
} 
