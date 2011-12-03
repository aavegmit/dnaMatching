#include "headers.h"

int *sock ;
int *hash_sock ;
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


void CreateEthernetHeader(struct frame *header, char *dst_mac, int protocol)
{
    /* copy the Dst mac addr */
    memcpy(header->ether_dhost, (void *)ether_aton(dst_mac), 6);

    /* copy the protocol */
    header->ether_type = htons(protocol);
}

int main(int argc, char **argv){
    FILE *fp ;
    sock = (int *)malloc(4 * sizeof(int)) ;
    hash_sock = (int *)malloc(NUM_HASHERS * sizeof(int)) ;


    for(int i = 0 ; i < NUM_HASHERS ; ++i){
	hash_sock[i] = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock[i] == -1) { 
	    printf("when opening socket in PAListener");
	}
    }

    for(int i = 0 ; i < 4 ; ++i){
	sock[i] = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock[i] == -1) { 
	    printf("when opening socket in PAListener");
	}
    }

    // Bind interfaces
    BindRawSocketToInterface(RTR_HASHER_INTERFACE, hash_sock[0], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_0, sock[0], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_1, sock[1], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_2, sock[2], ETH_P_ALL);
    BindRawSocketToInterface(INTERFACE_3, sock[3], ETH_P_ALL);

    pthread_t interfaceSniffer[4];

    int res ;
    for(int i = 0 ; i < NUM_HASHERS ; ++i){
	res = pthread_create(&interfaceSniffer[i], NULL, sniffer, (void *)hash_sock[i]); 
	if( res != 0){
	    fprintf(stderr, "TCP read thread creation failed\n") ;
	    exit(EXIT_FAILURE) ;
	}
    }

    for( int i = 0 ; i < NUM_HASHERS ; ++i)
	pthread_join(interfaceSniffer[i], NULL) ;

}

void *sniffer(void *arg){
    int sockfd = (long)arg ;
    bitset<SUBSEQ_SIZE> subseq ;

    int index ;
    struct frame *header ;
    struct content_ref *packet_ref ;
    struct content_read *packet_read ;

    int length, tempsock ;

    unsigned char *header_buf ;
    unsigned char *content_buf ;

    int header_len = sizeof(struct frame);
    int packet_read_len = sizeof(struct content_read) ;
    int packet_ref_len = sizeof(struct content_ref) ;

    header_buf = (unsigned char*)malloc(header_len); 
    content_buf = (unsigned char *)malloc(packet_read_len) ;


    while(1){
	length = read(sockfd, header_buf, header_len );

	if(length != header_len){
	    printf("Expected to receive %d, instead received %d\n", header_len, length) ;
	    continue ;
	}

	header = (struct frame *)(header_buf);

	if(header->type == TYPE_REF){
	    while(1){
		length = read(sockfd, content_buf, packet_ref_len );

		if(length == packet_ref_len)
		    break ;
	    }
	    subseq = ((struct content_ref *)(content_buf))->subseq;
	} else if(header->type == TYPE_READ){
	    while(1){
		length = read(sockfd, content_buf, packet_read_len );

		if(length == packet_read_len)
		    break ;
	    }
	    subseq = ((struct content_read *)(content_buf))->subseq;
	}
	else
	    continue ;

	if (subseq[MSB_SUBSEQ])
	    index = SUBSEQ_SIZE - 7 ;
	else
	    index = SUBSEQ_SIZE - 9 ;

	// 00 - representing A or N
	if(!subseq[index] && !subseq[index-1]){
	    printf("Starting with A or N %s\n", subseq.to_string()) ;
	    CreateEthernetHeader(header, INDEXER0_ETHER, ETH_P_ALL);
	    tempsock = sock[0] ;
	}
	// 01 - representing G
	else if(!subseq[index] && subseq[index-1]){
	    printf("Starting with G\n") ;
	    CreateEthernetHeader(header, INDEXER1_ETHER, ETH_P_ALL);
	    tempsock = sock[1] ;
	}
	// 10 - representing T
	else if(subseq[index] && !subseq[index-1]){
	    printf("Starting with T\n") ;
	    CreateEthernetHeader(header, INDEXER2_ETHER, ETH_P_ALL);
	    tempsock = sock[2] ;
	}
	// 11 - representing C
	else if(subseq[index] && subseq[index-1]){
	    printf("Starting with C\n") ;
	    CreateEthernetHeader(header, INDEXER3_ETHER, ETH_P_ALL);
	    tempsock = sock[3] ;
	}

        if(write(tempsock,header,header_len) < 0){
            perror("sendto");
        }
        if(write(tempsock,content_buf,length) < 0){
            perror("sendto");
        }
    }
} 
