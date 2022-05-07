#include "trace.h"
#include "checksum.h"

int ethernet_layer(const u_char *frame){

    
    uint8_t des_mac[DES_MAC_LENGTH];
    uint8_t source_mac[SOURCE_MAC_LENGTH];
    uint8_t type[TYPE_LENGTH];
    
    /* Get header data */
    memcpy(des_mac, &frame[DES_MAC_OFFSET], DES_MAC_LENGTH);
    memcpy(source_mac, &frame[SOURCE_MAC_OFFSET], SOURCE_MAC_LENGTH);
    memcpy(type, &frame[TYPE_OFFSET], TYPE_LENGTH);  
    /*Print header data */
    printf("\tEthernet Header\n");
    printf("\t\tDest MAC: %s\n", ether_ntoa((const struct ether_addr *) des_mac));
    printf("\t\tSource MAC: %s\n", ether_ntoa((const struct ether_addr *) source_mac));

   /*check the type and print based on that */ 
   /* 1 = IP, 2 = ARP, and 3 = TCP */
    if(type[0] ==  0x08 && type[1] == 0x00){
      printf("\t\tType: IP\n\n");
      return 1;      
    } else if (type[0] == 0x08 && type[1] == 0x06){
      printf("\t\tType: ARP\n\n");
      return 2;
    } else {
      printf("\t\tType: Unknown\n\n");
      return 0;
    }
        
}

int IP_layer(const u_char *data_gram, int *type, u_char *sudoheader){

  uint8_t ver_len;
  uint8_t tos;
  uint16_t length;

  uint8_t  TTL;
  uint8_t protocol; 
  uint16_t checksome;
  uint8_t sourceIP[IP_SIZE];
  uint8_t desIP[IP_SIZE];
  unsigned short adder;
  uint8_t res;  
  uint16_t segLen;

  /*  copy over the memory to veriables and sudoheader */

  memcpy(&ver_len, &data_gram[IP_HEADER_VERSION_OFFSET], BIT_8_SIZE );
  memcpy(&tos, &data_gram[IP_TYPE_OFFSET], BIT_8_SIZE);
  memcpy(&length, &data_gram[IP_LENGTH_OFFSET], BIT_16_SIZE);
  memcpy(&TTL, &data_gram[IP_TTL_OFFSET], BIT_8_SIZE);

  memcpy(&protocol, &data_gram[IP_PROTOCOL_OFFSET], BIT_8_SIZE);
  memcpy(&sudoheader[SUDO_PROT_OFFSET], &data_gram[IP_PROTOCOL_OFFSET], BIT_8_SIZE);
  

  memcpy(&checksome, &data_gram[IP_CHECKSOME_OFFSET], BIT_16_SIZE);
  memcpy(sourceIP, &data_gram[IP_SOURCE_IP_OFFSET], IP_SIZE);
  memcpy(&sudoheader[SUDO_SOURCE_IP_OFFSET], &data_gram[IP_SOURCE_IP_OFFSET], BIT_32_SIZE);

  memcpy(desIP, &data_gram[IP_DEST_IP_OFFSET], IP_SIZE); 
  memcpy(&sudoheader[SUDO_DES_IP_OFFSET], &data_gram[IP_DEST_IP_OFFSET], BIT_32_SIZE);
  
  /* set the rest of the sudoheader */ 
  res = 0x00;
  memcpy(&sudoheader[SUDO_RES_OFFSET], &res, BIT_8_SIZE);
  segLen = ntohs(length) -  ( ( 0x0F & ver_len)*4 );
  segLen = htons(segLen);
  memcpy(&sudoheader[SUDO_LENGTH_OFFSET], &segLen, BIT_16_SIZE);

  /* print the  header to standered out */

  printf("\tIP Header\n");

  printf("\t\tHeader Len: %u (bytes)\n", ( 0x0F & ver_len)*4 );
  printf("\t\tTOS: 0x%x\n", tos ); 
  printf("\t\tTTL: %u\n", TTL );
  printf("\t\tIP PDU Len: %u (bytes)\n", ntohs(length) );
  
  printf("\t\tProtocol: ");
  if (protocol == 0x01 ){
    printf("ICMP\n");
    *type = 1;
  } else if(protocol == 0x11){
    printf("UDP\n");
    *type = 2;
  } else if (protocol == 0x06){
    printf("TCP\n");
    *type = 3;
  } else {
    printf("Unknown\n");
    *type = 4;
  }
  
  /* proform the cksum operation */
  adder = in_cksum((unsigned short int *) data_gram, ( 0x0F & ver_len)*4);

  printf("\t\tChecksum: ");
  if(adder == 0){
    printf("Correct ");
  } else {
    printf("Incorrect ");
  }
  

  printf("(0x%x)\n", checksome);

  printf("\t\tSender IP: %s\n", inet_ntoa(*(struct in_addr *) sourceIP));
  printf("\t\tDest IP: %s\n", inet_ntoa(*(struct in_addr *) desIP));
 

  /* retun the length of the header */
  return ( ( 0x0F & ver_len)*4 );
 
}

void APR_protocol(const u_char *data_gram){


  uint8_t opcode[APR_OPCODE_LENGTH];
  uint8_t senderMAC[APR_SENDER_MAC_LENGTH];
  uint8_t senderIP[APR_SENDER_IP_LENGTH];
  uint8_t targetMAC[APR_TARGET_MAC_LENGTH];
  uint8_t targetIP[APR_TARGET_IP_LENGTH];

   /* copy over the parts of the data gram to the APR header */
  memcpy(opcode, &data_gram[APR_OPCODE_OFFSET], APR_OPCODE_LENGTH );
  memcpy(senderMAC, &data_gram[APR_SENDER_MAC_OFFSET], APR_SENDER_MAC_LENGTH);
  memcpy(senderIP, &data_gram[APR_SENDER_IP_OFFSET], APR_SENDER_IP_LENGTH);
  memcpy(targetMAC, &data_gram[APR_TARGET_MAC_OFFSET], APR_TARGET_MAC_LENGTH);
  memcpy(targetIP, &data_gram[APR_TARGET_IP_OFFSET], APR_TARGET_IP_LENGTH);


  /* plrint the APR header to standered out */
   
  printf("\tARP header\n");

  if(opcode[1] == 0x01){
    printf("\t\tOpcode: Request\n");
  } else if (opcode[1] == 0x02){
    printf("\t\tOpcode: Reply\n");
  } else {
    printf("\t\tOpcode: Unknown\n");
  }
  printf("\t\tSender MAC: %s\n", ether_ntoa((const struct ether_addr *) senderMAC));
  printf("\t\tSender IP: %s\n", inet_ntoa(*(struct in_addr *) senderIP));
  printf("\t\tTarget MAC: %s\n", ether_ntoa((const struct ether_addr *) targetMAC));
  printf("\t\tTarget IP: %s\n\n", inet_ntoa(*(struct in_addr *) targetIP));
  

}

void ICMP_protocol(const u_char *segment){

    uint8_t type;

    memcpy(&type, &segment[ICMP_TYPE_OFFSET], BIT_8_SIZE);
   
   /* print ICMP protocal header */
    printf("\n\tICMP Header\n");
    printf("\t\tType: ");
    if(type == 0x00 ){
       printf("Reply\n");

   } else if(type == 0x08 ){

       printf("Request\n");
   }  else {
      printf("%d\n", type); 
   }

}

void UDP_protocol(const u_char *segment){

    uint16_t sourcePort;
    uint16_t  desPort;

     /* save and print UDP header */

    printf("\n\tUDP Header\n");

    memcpy(&sourcePort, &segment[UDP_SOURCE_OFFSET], BIT_16_SIZE);
    memcpy(&desPort, &segment[UDP_DES_OFFSET], BIT_16_SIZE);

    printf("\t\tSource Port: : %u\n", ntohs(sourcePort) );
    printf("\t\tDest Port: : %u\n", ntohs(desPort) );


}

void TCP_layer(const u_char *segment, u_char *sudoheader){
     
    uint16_t sourcePort;
    uint16_t desPort;
    uint32_t seqNum;
    uint32_t ackNum;
    uint16_t flags;
    uint16_t winSize;
    uint16_t chkSum;
    unsigned short adder;
    int fullLength;
    uint16_t segLen;

    /* copy over the portins of the TCP header */ 
    memcpy(&sourcePort, &segment[TCP_SOURCE_OFFSET], BIT_16_SIZE );
    memcpy(&desPort, &segment[TCP_DES_OFFSET], BIT_16_SIZE );
    memcpy(&seqNum, &segment[TCP_SEQ_OFFSET], BIT_32_SIZE);
    memcpy(&ackNum, &segment[TCP_ACK_OFFSET], BIT_32_SIZE);
    memcpy(&flags, &segment[TCP_FLG_OFFSET], BIT_16_SIZE);
    memcpy(&winSize, &segment[TCP_WIN_OFFSET],  BIT_16_SIZE);
    memcpy(&chkSum, &segment[TCP_CHK_OFFSET], BIT_16_SIZE);

    /* get the length of the segment from sudo header */
    memcpy(&segLen, &sudoheader[SUDO_LENGTH_OFFSET], BIT_16_SIZE);
    flags = ntohs(flags); 
    /* find the total length of the SUDO header */
    fullLength = SUDO_HEADER_OFFSET + ntohs(segLen);
    
    /* increase the memory space avalable for the sudo header */
    if(!(sudoheader = realloc(sudoheader,fullLength))){
        perror("malloc");
        exit(EXIT_FAILURE);

    }
   
    /* concatinater the sedment and sudo header for the cksum */

    memcpy(&sudoheader[SUDO_HEADER_OFFSET], segment, ntohs(segLen));
  
    /* run checksum on sudo header */
    adder = in_cksum((unsigned short int *) sudoheader, fullLength);

    /* print TCP header */
    printf("\n\tTCP Header\n");
    switch(ntohs(sourcePort)){
       case 80:
       case 8080:   
             printf("\t\tSource Port:  HTTP\n");
             break;
      case 23:
              printf("\t\tSource Port:  TELNET\n");
              break;
       case 20:
       case 21:
   
              printf("\t\tSource Port:  FTP\n");
              break;
        case 110:
              printf("\t\tSource Port:  POP3\n");
              break;
        case 161:

             printf("\t\tSource Port:  SMTP\n");
             break;
        default:
              printf("\t\tSource Port: : %u\n", ntohs(sourcePort));
              break;
    }
    switch(ntohs(desPort)){
       case 80:
       case 8080:   
             printf("\t\tDest Port:  HTTP\n");
             break;
      case 23:
              printf("\t\tDest Port:  TELNET\n");
              break;
       case 20:
       case 21:
   
              printf("\t\tDest Port:  FTP\n");
              break;
        case 110:
              printf("\t\tDest Port:  POP3\n");
              break;
        case 161:

             printf("\t\tDest Port:  SMTP\n");
             break;
        default:
              printf("\t\tDest Port: : %u\n", ntohs(desPort));
              break;
    }
    printf("\t\tSequence Number: %u\n", ntohl(seqNum));
    if(flags & 0x10){
    	printf("\t\tACK Number: %u\n", ntohl(ackNum));
    } else {
 	printf("\t\tACK Number: <not valid>\n");
    }
    printf("\t\tACK Flag: %s\n", (flags & 0x10) ? "Yes" : "No" );
    printf("\t\tSYN Flag: %s\n", (flags & 0x02) ? "Yes" : "No" );
    printf("\t\tRST Flag: %s\n", (flags & 0x04) ? "Yes" : "No" );
    printf("\t\tFIN Flag: %s\n", (flags & 0x01) ? "Yes" : "No" ); 
    printf("\t\tWindow Size: %u\n", ntohs(winSize));


    printf("\t\tChecksum: %s (0x%x)\n", (adder == 0) ?"Correct" : "Incorrect",ntohs(chkSum));
 
 

}

