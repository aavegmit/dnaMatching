#include "headers.h"
#include "indexer_header.h"

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

int main(int argc, char **argv){
    int length, s, ethhdr_len;
    struct ifreq ethreq;
    struct frame *header ;
    unsigned char *header_buf ;

    if(argc < 2){
	printf("Usage: ./indexer <interface_name>\n");
	exit(0);
    }

    s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (s == -1) { 
	printf("when opening socket in PAListener");
	return NULL;
    }

    BindRawSocketToInterface(argv[1], s, ETH_P_ALL);

    int packet_read_len = sizeof(struct content_read) ;
    int packet_ref_len = sizeof(struct content_ref) ;
    int header_len = sizeof(struct frame)  + max(packet_read_len, packet_ref_len);

    header_buf = (unsigned char*)malloc(header_len); 

    while(1){

	length = read(s, header_buf, header_len );

	if(length != header_len){
	    printf("Expected to receive %d, instead received %d\n", header_len, length) ;
	    continue ;
	}

	header = (struct frame *)(header_buf);

	if(header->type == TYPE_READ)
	    parseReadPacket(header_buf);
	else if(header->type == TYPE_REF){
	    printf("Received ref packet\n\n\n") ;
	    parseRefPacket(header_buf);
	} else
	    continue ;
    }
}
