#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

/**
 * @Author Kaylin Zaroukian, Jerry, Cody Krueger
 * @Date 14 OCT 2018
 * CIS 457 Data Comm
 * Project 2
 *
 * References:
 */

int main() {
  // packet socket appears to be eth1
  int packet_socket;
  int lo_socket;
  int eth0_socket;
  int eth1_socket;
  unsigned char router_mac_addr[6];

  // set of sockets
  fd_set sockets;
  FD_ZERO(&sockets);

  //get list of interface addresses. This is a linked list. Next
  //pointer is in ifa_next, interface name is in ifa_name, address is
  //in ifa_addr. You will have multiple entries in the list with the
  //same name, if the same interface has multiple addresses. This is
  //common since most interfaces will have a MAC, IPv4, and IPv6
  //address. You can use the names to match up which IPv4 address goes
  //with which MAC address.
  struct ifaddrs *ifaddr, *tmp;
  if(getifaddrs(&ifaddr)==-1){
    perror("getifaddrs");
    return 1;
  }
  //have the list, loop over the list
  int i = 0;
  for(tmp = ifaddr; tmp!=NULL; tmp=tmp->ifa_next){
    i++;
    //Check if this is a packet address, there will be one per
    //interface.  There are IPv4 and IPv6 as well, but we don't care
    //about those for the purpose of enumerating interfaces. We can
    //use the AF_INET addresses in this list for example to get a list
    //of our own IP addresses
    if(tmp->ifa_addr->sa_family==AF_PACKET){
      printf("Interface: %s\n",tmp->ifa_name);

      // for eth1
      //create a packet socket on interface r?-eth1
      if(!strncmp(&(tmp->ifa_name[3]),"eth1",4)){
	printf("Creating Socket on interface %s\n",tmp->ifa_name);

  //create a packet socket
	//AF_PACKET makes it a packet socket
	//SOCK_RAW makes it so we get the entire packet
	//could also use SOCK_DGRAM to cut off link layer header
	//ETH_P_ALL indicates we want all (upper layer) protocols
	//we could specify just a specific one

  struct sockaddr_ll *r_mac_addr = (struct sockaddr_ll *)tmp->ifa_addr;
  memcpy(router_mac_addr, r_mac_addr->sll_addr, 6);

	packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

  if(packet_socket<0){
	  perror("socket");
	  return 2;
	}

  //Bind the socket to the address, so we only get packets
	//recieved on this specific interface. For packet sockets, the
	//address structure is a struct sockaddr_ll (see the man page
	//for "packet"), but of course bind takes a struct sockaddr.
	//Here, we can use the sockaddr we got from getifaddrs (which
	//we could convert to sockaddr_ll if we needed to)
	if(bind(packet_socket,tmp->ifa_addr,sizeof(struct sockaddr_ll))==-1){
	  perror("bind");
	}
}

// for lo
if(!strncmp(&(tmp->ifa_name[2]),"lo",2)){
printf("Creating Socket on interface %s\n",tmp->ifa_name);

//create a packet socket
//AF_PACKET makes it a packet socket
//SOCK_RAW makes it so we get the entire packet
//could also use SOCK_DGRAM to cut off link layer header
//ETH_P_ALL indicates we want all (upper layer) protocols
//we could specify just a specific one
lo_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

if(lo_socket<0){
perror("socket");
return 2;
}

//Bind the socket to the address, so we only get packets
//recieved on this specific interface. For packet sockets, the
//address structure is a struct sockaddr_ll (see the man page
//for "packet"), but of course bind takes a struct sockaddr.
//Here, we can use the sockaddr we got from getifaddrs (which
//we could convert to sockaddr_ll if we needed to)
if(bind(lo_socket,tmp->ifa_addr,sizeof(struct sockaddr_ll))==-1){
perror("bind");
}
}

// eth 0
if(!strncmp(&(tmp->ifa_name[3]),"eth0",4)){
printf("Creating Socket on interface %s\n",tmp->ifa_name);

//create a packet socket
//AF_PACKET makes it a packet socket
//SOCK_RAW makes it so we get the entire packet
//could also use SOCK_DGRAM to cut off link layer header
//ETH_P_ALL indicates we want all (upper layer) protocols
//we could specify just a specific one
eth0_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

if(eth0_socket<0){
perror("socket");
return 2;
}

//Bind the socket to the address, so we only get packets
//recieved on this specific interface. For packet sockets, the
//address structure is a struct sockaddr_ll (see the man page
//for "packet"), but of course bind takes a struct sockaddr.
//Here, we can use the sockaddr we got from getifaddrs (which
//we could convert to sockaddr_ll if we needed to)
if(bind(eth0_socket,tmp->ifa_addr,sizeof(struct sockaddr_ll))==-1){
perror("bind");
}
}

// eth2
if(!strncmp(&(tmp->ifa_name[3]),"eth2",4)){
printf("Creating Socket on interface %s\n",tmp->ifa_name);

//create a packet socket
//AF_PACKET makes it a packet socket
//SOCK_RAW makes it so we get the entire packet
//could also use SOCK_DGRAM to cut off link layer header
//ETH_P_ALL indicates we want all (upper layer) protocols
//we could specify just a specific one
eth1_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

if(eth1_socket<0){
perror("socket");
return 2;
}

//Bind the socket to the address, so we only get packets
//recieved on this specific interface. For packet sockets, the
//address structure is a struct sockaddr_ll (see the man page
//for "packet"), but of course bind takes a struct sockaddr.
//Here, we can use the sockaddr we got from getifaddrs (which
//we could convert to sockaddr_ll if we needed to)
if(bind(eth1_socket,tmp->ifa_addr,sizeof(struct sockaddr_ll))==-1){
perror("bind");
}
}
}
printf("Loop number: %d\n",i);
}
  //
  FD_SET(packet_socket, &sockets);
  FD_SET(lo_socket, &sockets);
  FD_SET(eth0_socket, &sockets);
  FD_SET(eth1_socket, &sockets);

  // typedef struct {
  //   uint16_t op;
  //   // 4 bytes
  //   uint32_t net_type;
  //   // eth mac address is 6 bytes
  //   uint32_t eth_mac;
  //   uint16_t src_ip;
  //   uint16_t src_mac;
  //   uint16_t dst_ip;
  //   uint16_dst_mac;
  // } arp_header;
  //
  // typedef struct {
  //   uint16_t type;
  //   uint16_t code;
  //   uint16_t checksum;
  // } icmp_header;


  printf("Ready to recieve now\n");
  while(1){
    printf("Entered while loop\n");
    fd_set tmp_set=sockets;
    select(FD_SETSIZE,&tmp_set,NULL,NULL,NULL);

    int i;
    int len;
    int sock;
    struct sockaddr_ll recvaddr;
    char buf[1500];
    int recvaddrlen;
    int n = 0;
    for(i=0;i<FD_SETSIZE;i++) {
      if(FD_ISSET(i,&tmp_set)) {
        len = sizeof(tmp);
        n = recvfrom(i, buf, 1500,0,(struct sockaddr*)&recvaddr, &recvaddrlen);
        if(recvaddr.sll_pkttype==PACKET_OUTGOING)
          continue;
        //start processing all others
        printf("Got a %d byte packet\n", n);

        // header for arp
        // not sure if we need both of these
        struct ether_header *eth;
        eth = (struct ether_header*)buf;
        // struct ether_arp *arp_hdr;
        // arp_hdr = (struct ether_arp*)(buf + 14);

        // ip headers too
        struct iphdr *ip_pckt_hdr = (struct iphdr *)(buf + 14);

        // we are receiving all arp packets
        // checks to see which packets are arp
        if (ntohs(eth->ether_type) == ETH_P_ARP) {
          printf("ARP packet\n");
          //char *src_mac = ether_ntoa((struct ether_addr *)&eth->ether_shost);
          uint8_t src_mac[6];
          while(i < 6) {
            src_mac[i] = eth->ether_shost[i];
          }
          uint32_t ip_source_addr = ip_pckt_hdr->saddr;
          uint32_t ip_dest_addr = ip_pckt_hdr->daddr;

          //u_char src_ip = arp_hdr->arp_spa[1];
          //u_char goal_ip = arp_hdr->arp_tpa[2];
          //printf("ARP: %d, %d, %d\n", src_mac, src_ip, goal_ip);
        }
      }
    }
    FD_CLR(i,&sockets);

  }
  //free the interface list when we don't need it anymore
  freeifaddrs(ifaddr);
  //exit
  return 0;
}