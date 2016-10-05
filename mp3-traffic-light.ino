#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
#include <Ethernet.h>

//////////// Serial /////////////////////////////
#define BAUD_RATE 9600

//////////// Ethernet /////////////////////////////
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign 
const IPAddress ip(192, 168, 1, 10);
EthernetClient networkClient;

//////////// Traffic light service ////////////////
#define TRAFFIC_LIGHT_SERVICE_PORT 3000
const char trafficLightHost[] = "192.168.1.5";
unsigned int lastConnectionTime = 0;
const unsigned int pollInterval = 1500;

//////////// Timing ///////////////////////////////
#define DELAY 500

//////////// RGB Pixels ///////////////////////////
#define RGB_PIXELS 9
#define PIN_RGB_LED 13
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(RGB_PIXELS, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);

//////////////// MP3 /////////////////////////////
SdFat sd;
SFEMP3Shield MP3player;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // Initialize serial modem
  setupSerialModem();
  setupRGBLeds();
//  setupEthernet();
  setupMp3Player();
}

void setupSerialModem() {
  Serial.begin(BAUD_RATE);  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println(F("Starting."));
}

void setupRGBLeds() {
  pixels.begin(); 
  resetLights();
  Serial.println(F("Lights ready."));
}

void setupEthernet() {
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP."));
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  
  Serial.println(F("Ethernet ready."));
}

void setupMp3Player() {
  // Initialize SD card.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED))  {
    sd.initErrorHalt();
  }
  if(!sd.chdir("/")) {
    sd.errorHalt("sd.chdir");
  }

  // Initialize the MP3 Player Shield.
  uint8_t mp3PlayerResult;
  mp3PlayerResult = MP3player.begin();
  
  if(mp3PlayerResult != 0 && mp3PlayerResult !=6 ) {
    Serial.print(F("Error code: "));
    Serial.println(mp3PlayerResult);
  }
  
  Serial.println(F("MP3 player ready."));  
}

void loop() {
//  // If interval is past get the last status from the traffic light service.
//  if (millis() - lastConnectionTime > pollInterval) {
//    getTrafficLightStatus();
//  }
//
//  if (networkClient.available()) {
//    char c = networkClient.read();
//    Serial.write(c);
//    resetLights();
//    setLight(1, 1, 150, 0); 
//  }
}

void resetLights() {
  for(byte i=0;i<RGB_PIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }
}

void setLight(byte lightNr, byte R, byte G, byte B) {
  pixels.setPixelColor(lightNr, pixels.Color(R,G,B));
  pixels.show();
}

void playTrack(char trackName[]) {
  uint8_t result;

  result = MP3player.playMP3(trackName, 0);
 
  if(result != 0) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to play track"));
  }
}

void getTrafficLightStatus() {
  networkClient.stop();

  if (networkClient.connect(trafficLightHost, TRAFFIC_LIGHT_SERVICE_PORT)) {
    // send the HTTP GET request:
    networkClient.println(F("GET /status/traffic-light HTTP/1.1"));
    //networkClient.println(F("Host: www.arduino.cc"));
    networkClient.println(F("User-Agent: traffic-light"));
    networkClient.println(F("Connection: close"));
    networkClient.println();

    // Note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    Serial.println(F("Get status failed."));
  }
}
