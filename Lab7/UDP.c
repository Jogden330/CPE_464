#include "UDP.h"
#include "checksum.h"

int createPDU(uint8_t * pduBuffer, uint32_t sequenceNumber, uint8_t flag, uint8_t * payload, int payloadLen){
	uint32_t netsequence;	
       	uint16_t csum = 0;
    //    int  pduLenght= 0;

//	sequenceNumber++;
        netsequence =  htonl(sequenceNumber);
	memcpy(pduBuffer, &netsequence, sizeof(uint32_t));
	memcpy(&pduBuffer[4], &csum, sizeof(uint16_t));
	memcpy(&pduBuffer[6], &flag, sizeof(uint8_t));
	memcpy(&pduBuffer[7], payload, payloadLen);
	csum = in_cksum((unsigned short int *)pduBuffer, (payloadLen + 7));

	memcpy(&pduBuffer[4], &csum, sizeof(uint16_t));

        printf("made the pdu\n");
	return 	payloadLen + 7;
 }

void outputPDU(uint8_t * pduBuffer,int PDUlen){

	uint32_t seqNum;
        uint8_t flag;
        uint16_t csum;
        int payloadLen = PDUlen - 7;

        memcpy(&seqNum, pduBuffer, sizeof(uint32_t));
        
        memcpy(&csum,&pduBuffer[4], sizeof(uint16_t));
        memcpy(&flag,&pduBuffer[6], sizeof(uint8_t));
        seqNum = ntohl(seqNum);
        printf("Seqence Number is %d\n", seqNum);
       
	csum = in_cksum((unsigned short int *)pduBuffer, PDUlen);
        if(csum == 0){
            printf("checksome correct\n");
        } else {
            printf("checksome incorrect\n");

       }
        printf("The Flag is %d\n", flag);

        printf("payload %.*s\n",payloadLen, &pduBuffer[7]);
  
}
