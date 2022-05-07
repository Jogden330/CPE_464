#include "trace.h"


void usage(const char *name) {
  fprintf(stderr,"usage: %s Tracefile\n",name);
  exit(EXIT_FAILURE);
}

int main( int argc, char *argv[] )  {

  pcap_t *tracefile;
  struct pcap_pkthdr *pkt_header;
  const u_char *pkt_data;
  char errbuf[PCAP_ERRBUF_SIZE];
  u_char *sudoheader;
  int number;
  int type;
  int length;

  /* if the number of arguments is wrong print usage message */

  if(argc != 2) {
    usage(argv[0]);
  }
  
  /* open the paket file */
  if(NULL == (tracefile = pcap_open_offline(argv[1], errbuf))){
    printf("im in this error");
    exit(EXIT_FAILURE);
  }
 
   /* cycle thrue eace paket in the paket file and print there header
 *    end when ther are no more pakets */ 
  number = 0;
  while(-2 != (pcap_next_ex(tracefile, &pkt_header, &pkt_data))){

    /* print paket informaton */
    printf("\n");
    printf("Packet number: %d  Frame Len: %d\n\n", ++number, pkt_header->caplen);
    /* read ethernet paket and decide what to do next bader on its output */

    switch(ethernet_layer(pkt_data)){
        case 1: 
         /* creat sudo header used for TCP protocol */
         if(!(sudoheader = malloc(SUDO_HEADER_OFFSET))){
              perror("malloc");
              exit(EXIT_FAILURE); 
            
         }      
         
          /*  print IP header and raturn the leangth of the header
 *            mutate type and sudo header for use in swithing and TCP chksum */      
          length =  IP_layer(&pkt_data[ETHERNET_HEADER_LENGTH], &type, sudoheader);

          /* swith based on the protacal type and proform those header printing */
          switch(type){
               case(1):
                    ICMP_protocol((&pkt_data[ETHERNET_HEADER_LENGTH + length]));
                    break;
               case(2):
                    UDP_protocol((&pkt_data[ETHERNET_HEADER_LENGTH + length]));
                    break;
               case(3):
                    TCP_layer((&pkt_data[ETHERNET_HEADER_LENGTH + length]), sudoheader);
                    break;
                default:
                break;

          }
          /* free the sudo header */
          free(sudoheader);
          break;
        case 2:
          APR_protocol(&pkt_data[ETHERNET_HEADER_LENGTH]);
      
          break;
         default:
            printf("idk why im hear");
            break;
    }
        
  }  
  /* close paket file path */ 
  pcap_close(tracefile);

  return 0;
}
