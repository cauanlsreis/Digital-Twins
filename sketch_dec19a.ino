#include "SPI.h"
#include <Ethernet.h>

  
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 3 };
     
EthernetServer servidor(80);
  
String readString;

void setup() {
  pinMode(2, INPUT_PULLUP);
  Ethernet.begin(mac, ip);
  servidor.begin();
  Serial.begin(9600);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

    
  // start the server
  servidor.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

}

void loop() {
  // listen for incoming clients
  EthernetClient client = servidor.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 0.4");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          if (digitalRead(2)==0) {
            client.print("<p> BOTAO PRESSIONADO </p>");
            client.println("<br />");
          }else{                      
            digitalRead(2) == 1;
            client.print("<p> BOTAO DESPRESSIONADO </p>");       
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

//Funcionamento botao arduino
//if (digitalRead(2)==0){
    //Serial.println("Botao pressionado");
    //delay(400);
//}else{
    //digitalRead(2)==1;
//}