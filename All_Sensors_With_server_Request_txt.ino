#include <SPI.h>
#include <Ethernet.h>


//Initializing variables 

byte mac[] = { 0x00, 0xAA, 0xBB, 0x08, 0xDE, 0x02 };
byte service[] = { 83, 212, 84, 224};   //okeanos
IPAddress ip(192,168,1, 177);           //my IP address
IPAddress gateway(192,168,1, 1);        //my Gateway
IPAddress subnet(255, 255, 0, 0);       //my Subnet
EthernetServer server(23);
boolean gotAMessage = false; 

int movementSensorStatus = 0;
int lightSensorStatus = 0;
int magneticSwichStatus = 0;
int ledPIN = 7;
int movementSwichPIN = 8;
int magneticSwichPIN = 2;
int lightSensorSwichPIN = 0;
bool magnet = false;


char* connString = "83.212.84.224:8080/ArduinoRestService/rest/setvaluesfromarduino/";  //Service pou kalw
int movementStatusToURL = 0;          //parametroi
int lightStatusToURL = 0;
int magnetStatusToURL = 0;

int low = 510;                   // arxikopoisi metavlitis tou lighdiode gia sigkrisi
#define NOFIELD 505L 
#define TOMILLIGAUSS 3756L

EthernetClient client;            //Ethernet Client for connection to the web



void setup(){
  Serial.begin(9600);                    
 pinMode (ledPIN, OUTPUT);
 pinMode (movementSwichPIN, INPUT);  
 pinMode (lightSensorSwichPIN, INPUT);  
 pinMode (magneticSwichPIN, INPUT);  

 
 
  // start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // initialize the ethernet device not using DHCP:
    Ethernet.begin(mac, ip, gateway, subnet);
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  // start listening for clients
  server.begin();
  
  
  
  
  
  
}
void loop () {

 digitalWrite(ledPIN, LOW);  
  delay(1000);

    movementSensorStatus = digitalRead(movementSwichPIN);
    lightSensorStatus = analogRead(lightSensorSwichPIN);  
    magneticSwichStatus = digitalRead(magneticSwichPIN);
    //magneticSwichStatus = analogRead(magneticSwichPIN);
 
    // wait for a new client:
    EthernetClient client = server.available();

    // when the client sends the first byte, say hello:
     if (client) 
     {
        if (!gotAMessage) 
        {
          Serial.println("We have a new client");
          client.println("Hello, client!"); 
          gotAMessage = true;
        }
        // read the bytes incoming from the client:
        char message = client.read();
        server.write(message);
        Serial.print(message);
  
      }
 
  if(movementSensorStatus == HIGH)                    //movement Sensor if()  for debud
  {
   Serial.print("movement HIGH");
   digitalWrite(ledPIN, HIGH);  
   delay(150);
   server.write("movement HIGH MESSAGE");
      server.write("\n");
     movementStatusToURL = 1;
  }
  else 
  {
    Serial.print("movement LOW");
    movementStatusToURL = 0;
  }
  
  
   Serial.print(lightSensorStatus - 500);     
 delay(500);
  if((lightSensorStatus - 500) < 470)                     //light sensor if() for debug.
  {                                                      // kanonikes sinthikes dwmatiou einai  420 - 480 (MAX 500)
     Serial.print("                 light LOW"); 
    lightStatusToURL = 0;
   
  }
  else
  {
   digitalWrite(ledPIN, HIGH);
   delay(150);
   Serial.print("                light HIGH"); 
   server.write("light HIGH MESSAGE");
   server.write("\n");
     lightStatusToURL = 1;
   }
   Serial.print("\n");   
 
/* Serial.print(magneticSwichStatus);    // DEBUG
 Serial.print(" ");    // DEBUG
  Serial.print(magneticSwichStatus - 1021);    // DEBUG
 delay(1000);
if((magneticSwichStatus - 1022) < 0)                   //MAGNET SENSOR if
  {
  //   digitalWrite(ledPIN, HIGH);
    delay(150);
   Serial.print("                                  magnet HIGH\n");  
   params[] = params[] + "/1"
  magnetStatusToURL = 1;  
}
  else{
   Serial.print("                                  magnet LOW\n"); 
   params[] = params[] + "/0"
 magnetStatusToURL = 0;    
}  
*/  
 
 
        delay(1500);
        if (movementStatusToURL >0 || lightStatusToURL > 0) // || magnetStatusToURL > 0)  //elegxos ena einai kapoio apo ta sensors energopoiimeno
        {                                                                                 // wste na stalthei to URL aitima sto Server
         // start the Ethernet connection:
         if (Ethernet.begin(mac) == 0) 
         {
            Serial.println("Failed to configure Ethernet using DHCP");
            Ethernet.begin(mac, ip);
          }
      
           delay(1000);
            Serial.println("connecting...");
       
            if (client.connect(service, 8080)) {                                  //elegxos ean i sindesi itan success me ton server
            Serial.println("connected");                                          //se mia sigkekrimeni thira
           
             //if(movementStatusToURL == 0 && lightStatusToURL == 0 && magnetStatusToURL == 1)    //magnetic siwtch enabled case
             //{                                                                                  //parentheseis mesa sta IF
             //   client.println("GET   /ArduinoRestService/rest/setvaluesfromarduino/0/0/1");    //0.0.1 
             //   Serial.println("/ArduinoRestService/rest/setvaluesfromarduino/0/0/1");
             //}
              if(movementStatusToURL == 0 && lightStatusToURL == 1) // && magnetStatusToURL == 0)
             {    
                client.println("GET   /ArduinoRestService/rest/setvaluesfromarduino/0/1/0");      //0.1.0 
                Serial.println("/ArduinoRestService/rest/setvaluesfromarduino/0/1/0");
             }
             //if(movementStatusToURL == 0 && lightStatusToURL == 1 && (magnetStatusToURL == 1))  
             //{
             //   client.println("GET   /ArduinoRestService/rest/setvaluesfromarduino/0/1/1");    //0.1.1 
             //   Serial.println("/ArduinoRestService/rest/setvaluesfromarduino/0/1/1");
             //}
             
            if(movementStatusToURL == 1  && lightStatusToURL==0)  // && magnetStatusToURL == 0) 
             {
                client.println("GET   /ArduinoRestService/rest/setvaluesfromarduino/1/0/0");      //1.0.0 
                Serial.println("/ArduinoRestService/rest/setvaluesfromarduino/1/0/0");
             }
             //if(movementStatusToURL == 1 && lightStatusToURL == 0 && magnetStatusToURL == 1) 
             //{
             //   client.println("GET   /ArduinoRestService/rest/setvaluesfromarduino/1/0/1");    //1.0.1 
             //   Serial.println("/ArduinoRestService/rest/setvaluesfromarduino/1/0/1");
             //}
             if(movementStatusToURL == 1 && lightStatusToURL == 1) //&& (magnetStatusToURL == 0)) 
             {
                client.println("GET   /ArduinoRestService/rest/setvaluesfromarduino/1/1/0");      //1.1.0 
                Serial.println("/ArduinoRestService/rest/setvaluesfromarduino/1/1/0");
             }
                     
             //if(movementStatusToURL == 1 && lightStatusToURL == 0 && magnetStatusToURL == 1) 
             //{
             //   client.println("GET   /ArduinoRestService/rest/setvaluesfromarduino/1/1/1");    //1.1.1 
             //   Serial.println("/ArduinoRestService/rest/setvaluesfromarduino/1/1/1");
             //}
             
            client.println("Host: http://83.212.84.224:8080");
            Serial.println("Host: http://83.212.84.224:8080");
            client.println("Connection: close");
            Serial.println("Connection: close");
            client.println();
            Serial.println("Exit");
      } 
        
        
        if (client.available()) { char c = client.read();
        Serial.print(c);
        Serial.print("lololol");
    }
  
    if (!client.connected()) 
    {
      Serial.println();
      Serial.println("disconnecting...");
      client.stop();
      Serial.println("disconnected");
     }
   
    
}
}
