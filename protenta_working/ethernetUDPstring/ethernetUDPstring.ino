/*
 UDPSendReceiveString:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender

 A Processing sketch is included at the end of file that can be used to send
 and received messages for testing with a computer.

 created 21 Aug 2010
 by Michael Margolis

 This code is in the public domain.
 */

#include <PortentaEthernet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// The IP address will be dependent on your local network:
IPAddress source_ip(192, 168, 1, 10);
IPAddress destination_ip(192, 168, 1, 20);

unsigned int localPort = 50000;      // local port to listen on

// buffers for receiving and sending data
//char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
//char ReplyBuffer[] = "acknowledged";        // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() 
{

  // start the Ethernet
  Ethernet.begin(source_ip);
  Udp.begin(localPort);
}

void loop()
{
    char data[]="Ethernet Test11";
    Udp.beginPacket(destination_ip,50000);
    Udp.write(data);
    Udp.endPacket();
  delay(10);
}
