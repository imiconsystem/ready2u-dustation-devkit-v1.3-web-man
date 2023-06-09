#define R1 23

String APssid = "R2Devkit";
String APpassword = "dddddddd";

int SETMODE = 0;
int devRunmode;
int DEVRUNMODE = 1; // Accesspoint mode (AP)
//int DEVRUNMODE = 2; // Station mode (ST)

String WiFissid = "";
String WiFipassword = "";

String WiFiRSSI;
String localIP;
String webTitle = "DustStion1";
String device_name = "dst";

int32_t maxTemp, maxHumi, maxPM1, maxPM2, maxPM10;


int32_t PM1, PM2, PM10, AQI;
float_t TEMP, HUMI;

struct PM25AQI {
  String word;
  String icon;
};
struct PM25AQI pm25lev = {};

String logString;

void setup() {

  Serial.begin(9600);  // For debug
  Serial.println("ESP start.");
  serialSetup();
  pmsSetup();
  oledSetup();
  oledLogSetup();
  relaySetup();
  resetbuttonSetup();
  storageSetup();
  dhtSetup();

  TEMP = 0;
  HUMI = 0;
  PM1 = 0;
  PM2 = 0;
  PM10 = 0;
  AQI = 0.0;

  
  Serial.print("DEVICE MODE: ");
  Serial.println(storageGetInt("devRunmode"));
  if (SETMODE == 1)  // SET
  {
    SETMODE = true;
    logString = "Set mode..";
    wifiapSetup(1); // AP for setup
    oledLogLoop();

  } else if (SETMODE == 2) {  // RESET FACTORY
    logString = "Wait 10 seconds then push EN(reset) button.";
    oledLogLoop();
    storageClear();
    ESP.restart();
  } else {
    // RUN
    if(storageGetInt("devRunmode").toInt()==2) {
      webserverSetup(); // ST mode
    }else {
      wifiapSetup(0); // AP for Run
    }
    logString = "System starting..";
    oledLogLoop();
  }
}

void controlRelay() {
  if (TEMP > maxTemp) {
    relayHi(R1);
  } else if (HUMI > maxHumi) {
    relayHi(R1);
  } else if (PM1 > maxPM1) {
    relayHi(R1);
  } else if (PM2 > maxPM2) {
    relayHi(R1);
  } else if (PM10 > maxPM10) {
    relayHi(R1);
  } else {
    relayLo(R1);
  }
}



void loop() {


  if (SETMODE == 0) {

    if(storageGetInt("devRunmode") == 0) webserverLoop();
    clientLoop();
    pmsLoop();
    dhtLoop();
    controlRelay();
    oledLoop();

  } else {
    clientLoop();
  }
}