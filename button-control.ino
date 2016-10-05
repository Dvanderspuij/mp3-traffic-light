#include <SPI.h>
#include <Ethernet.h>

//////////// Serial /////////////////////////////
#define BAUD_RATE 9600

//////////// Ethernet /////////////////////////////
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign 
const IPAddress ip(192, 168, 1, 110);
EthernetClient networkClient;

/////////// TRAFFIC LIGHT SERVICE ////////////////
#define TRAFFIC_LIGHT_SERVICE_PORT 3000
byte trafficLightHost[] = { 192, 168, 1, 104 };

////////// BUTTON STATE ///////////////////////////
bool lastButtonsState[3] = {false, false, false};


//////////// Timing ///////////////////////////////
#define DELAY 500

//////////// Timing ///////////////////////////////
#define BUTTON_PIN_GREEN 2
#define BUTTON_PIN_YELLOW 6
#define BUTTON_PIN_RED 4


//////////// Button state index. ///////////////////////////////
#define BUTTON_STATE_INDEX_YELLOW 1
#define BUTTON_STATE_INDEX_GREEN 0
#define BUTTON_STATE_INDEX_RED 2



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // Initialize serial modem
  setupSerialModem();
  setupEthernet();

  lastButtonsState[BUTTON_STATE_INDEX_YELLOW] = getYellowPinState();
  lastButtonsState[BUTTON_STATE_INDEX_GREEN] = getGreenPinState();
  lastButtonsState[BUTTON_STATE_INDEX_RED] = getRedPinState();
  Serial.println(lastButtonsState[BUTTON_STATE_INDEX_GREEN]);
  Serial.println(lastButtonsState[BUTTON_STATE_INDEX_YELLOW]);
  Serial.println(lastButtonsState[BUTTON_STATE_INDEX_RED]);
  
}


void setupSerialModem() {
  Serial.begin(BAUD_RATE);  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println(F("Starting."));
}

void printIpAdrress() {
  // Print your local IP address:
 Serial.print(F("IP address: "));
 ip = Ethernet.localIP();
 for (byte i = 0; i < 4; i++) {
   Serial.print(ip[i], DEC);
   Serial.print(".");
 }
 Serial.println();
}

void ReadButtonState() {
    
  if (getGreenPinState() != lastButtonsState[BUTTON_STATE_INDEX_GREEN]) {
    lastButtonsState[BUTTON_STATE_INDEX_GREEN] = getGreenPinState();
    
    if (getGreenPinState()) {
      Serial.println("Green button triggered");
      sendButtonEvent(BUTTON_STATE_INDEX_GREEN);
    }
  }

  if (getYellowPinState() != lastButtonsState[BUTTON_STATE_INDEX_YELLOW]) {
    lastButtonsState[BUTTON_STATE_INDEX_YELLOW] = getYellowPinState();
    
    if (getYellowPinState()) {
     Serial.println("Yellow button triggered");
     sendButtonEvent(BUTTON_STATE_INDEX_YELLOW);
    }
  }
  
  if (getRedPinState() != lastButtonsState[BUTTON_STATE_INDEX_RED]) {
    lastButtonsState[BUTTON_STATE_INDEX_RED] = getRedPinState();
    
    if (getRedPinState()) {
    Serial.println("Red button triggered");
    sendButtonEvent(BUTTON_STATE_INDEX_RED);
    }
  }   
}

void sendButtonEvent(int index) {
     networkClient.stop();
     
   if (networkClient.connect(trafficLightHost, TRAFFIC_LIGHT_SERVICE_PORT) ) {
     if (index == BUTTON_STATE_INDEX_GREEN) {
        networkClient.println(F("GET /button/green HTTP/1.1"));
        networkClient.println(F("Connection: close"));
        networkClient.println();
        Serial.println(F("Green button request send"));
      }
      else if(index == BUTTON_STATE_INDEX_YELLOW) {
        networkClient.println(F("GET /button/yellow HTTP/1.1"));
        networkClient.println(F("Connection: close"));
        networkClient.println();
        Serial.println(F("Yellow button request send"));
      }
      else if(index == BUTTON_STATE_INDEX_RED) {
       networkClient.println(F("GET /button/red HTTP/1.1"));
       networkClient.println(F("Connection: close"));
       networkClient.println();
        Serial.println(F("Red button request send"));
      }
      
   } else {
       Serial.println(F("Error connecting to traffic light service."));
   }
}



void setupEthernet() {
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP."));
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  
  Serial.println(F("Ethernet ready."));
  printIpAdrress();
}

void loop() {
  ReadButtonState();
}

bool getYellowPinState() {
  return digitalRead(BUTTON_PIN_YELLOW);
}
bool getGreenPinState() {
  return digitalRead(BUTTON_PIN_GREEN);
}
bool getRedPinState() {
  return digitalRead(BUTTON_PIN_RED);
}

