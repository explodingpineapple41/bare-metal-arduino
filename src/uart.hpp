#ifndef comms_h
#define comms_h

// 256 allows the pointers to take advantage of overflow so modulus not required
#define BUF_SIZE 256

class UARTSerial {
public:
    static void init();
    static void send(char c);
    static void print(const char *s);
    static void sendInt(int x);
    static unsigned char readByte();
    static unsigned char readString(char str[], unsigned char len);
    static bool isAvailable();
    static bool hasString();
    static void flushBuffer();
    static void handleRx();
    
    private:
    UARTSerial();

    volatile static unsigned char buffer[BUF_SIZE];
    volatile static unsigned char bufHead;
    volatile static unsigned char bufTail;
};

#endif