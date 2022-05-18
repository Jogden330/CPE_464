typedef struct serverWindow{
	int lower;
        int current;
        int upper;
        int windowsize;
        struct Data * PDUs;    

} serverWindow;

typedef struct Data{
	int valflag;
        int index;
        int seqNum;
        int size;
        unit8_t * pdu;
 }
}


serverWindow * window_init(int windowsize );


void printServerWindow_metadata(serverWindow * window);


void printEntireWindow(serverWindow * window);


void addPDUtoWindow(serverWindow * window, uint8_t * pduBuff, int size);
