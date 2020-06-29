#include <Steering.h>
#include <Encryption.h>
#include <ConnectToEEPROM.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
//#include <EEPROM.h>

#include <ESP8266HTTPClient.h>

/*
#include <Servo.h>
Servo horizontalServo;
#define SERVO_PIN1 D1M
Servo verticalServo;
#define SERVO_PIN2 D3
*/

/* Set these to your desired softAP credentials. They are not configurable at runtime */
#ifndef APSSID
#define APSSID "WiFiRC_Car"
#define APPSK  "12345678"
#endif

/* Set Motor Shield GPIO pins
#define PWMA_GPIO 5  // Side connected to A+/A-
#define PWMB_GPIO 4  // Side connected to B+/B-
#define DA_GPIO 0  // Side connected to A+/A- for reverse
#define DB_GPIO 2  // Side connected to B+/B- for reverse
*/

const unsigned short WEBSERVERURL_SIZE = 50;
char WEBSERVERURL[WEBSERVERURL_SIZE] = "http://192.168.1.19:8080";
const unsigned short MAX_GET_ERROR = 10;
const unsigned short WIFI_MEMORY = 8;
const unsigned short SSID_SIZE = 32;

char ssid[WIFI_MEMORY][SSID_SIZE];
char password[WIFI_MEMORY][SSID_SIZE];
const char* passKey = "ThisIsTheKey";

Steering drive;
Encryption encryption(passKey);
ConnectToEEPROM credentials(SSID_SIZE, WIFI_MEMORY);

/*
#define DEFAULTCAMHORIZONTAL 90
#define DEFAULTCAMVERTICAL 70
#define CAMSTEP 4
int horizontalServoPos = DEFAULTCAMHORIZONTAL;
int verticalServoPos = DEFAULTCAMVERTICAL;
*/
String httpSerialPrev = "";
String WiFiConnectionsPrev = "";
//String CameraSerialPrev = "";

bool connectedToWifi = 0;
const char *softAP_ssid = APSSID;
const char *softAP_password = APPSK;

/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
const char *myHostname = "esp8266";

unsigned short getErr = 0;
unsigned short lastGetErr = 0;
unsigned short finalIndex;
unsigned short nextIndex;

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);


/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
unsigned long lastConnectTry = 0;

/** Current WLAN status */
unsigned int status = WL_IDLE_STATUS;

/** Restart Arduino */
void(* resetFunc) (void) = 0;

void postSSID();

//void saveCredentials();

void shiftSSID(char ssidCorr[SSID_SIZE]){

}

void removeSSID(String rem){
  int p = -1;
  for (int i = 0 ; i < WIFI_MEMORY ; i++) {
    if (rem == String(ssid[i])) {
      strcpy(ssid[i], "");
      strcpy(password[i], "");
      p = i;
    }
    if ((p >= 0) && (ssid[i] != "")) {
      strcpy(ssid[p], ssid[i]);
      strcpy(password[p], password[i]);
      strcpy(ssid[i], "");
      strcpy(password[i], "");
      p = i;
    }
  }
  nextIndex --;
  credentials.save(ssid, password);
  postSSID();
}

void addSSID(String ssidAdd, String passAdd){
  ssidAdd.toCharArray(ssid[nextIndex], sizeof(ssid[nextIndex]) - 1);
  passAdd.toCharArray(password[nextIndex], sizeof(password[nextIndex]) - 1);
  credentials.save(ssid, password);
  postSSID();
}

void postToServer(String url, String msg){
  //Serial.print(url);
  //Serial.print("  :  ");
  //Serial.print(msg);
  WiFiClient client;
  HTTPClient Post;
  Post.begin(client, url);
  Post.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Post.POST(msg);
  Post.end();
}

void postSSID(){
  String postmsg = String("ssidCurr=") + String(WiFi.SSID()) + String("&");
  Serial.println(postmsg);
  for(int q = 0; q < (nextIndex == 0 ? WIFI_MEMORY : nextIndex + 1); q++){
    Serial.print(q);
    Serial.print(":  ");
    Serial.println(postmsg);
    postmsg += "ssid" + String(q) + "=" + String(ssid[q]) + (q == WIFI_MEMORY - 1 ? "" : "&");
  }
  Serial.println(postmsg);
  postToServer(String(WEBSERVERURL) + String("/saveWiFiConnections.php"), postmsg);
}

/** Is this an IP? */
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

/** Handle root or redirect to captive portal */
void handleRoot() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += "<html><head><title>Main page</title></head><body><p style='font-size:2rem; font-weight:bold; text-align:center;'>WELCOME TO WiFiRC-Car PROJECT!</p><div style='width:50vw; height:90vh; margin:auto; background-color:#F0F0F0; z-index:1; display: flex; align-items: center; justify-content: center;'><div style='width:90%; height:90%;'>";
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>");
  }
  Page += "<p>You have reached the captive portal of an Arduino project made by Kasper van Maasdam.</p><p>The goal of this project is to make a WiFi remote-controlled car, hence the name 'WiFiRC-Car'.<br/>The date I wrote this was 16-06-2020 and I was working on for quite a while already.<br/>It is still a work in progress, so if you have any thoughts on how to improve or if you find bugs,<br/>please contact me at kaspervanm@gmail.com.</p><p>The car gets its commands from a webserver by continuously reading the contents to check if something has changed.<br/>I tried to make the webserver as seperate as possible from the car itself. This way dependancy issues can be easily solved.<br/>Because the car communicates to the server over the internet, it needs to be connected to WiFi. Here is where you come into play.<br/>The car is able to store up to eight service set identifiers (SSID's) and their corresponding passwords.<br/>Each with a max of 32 characters. The passwords are encrypted with a key before being stored on the arduino EEPROM.<br/>This way, no passwords can be recovered, even if the EEPROM gets into the wrong hands.<br/>In order to instantiate a connection between the arduino and the server, you must enter the SSID and password of the WiFi available in the area where the car will be driving.<br/></p><p>To continue setting up your device, <a href='/wifi'>click here to configure the wifi connection</a>.</p></div></div><div style='margin-top:10px; margin:auto; width:100vw; background-color:lightgrey; text-align:center; z-index:2;'><p>If you stumbled upon this captive portal by accident, please leave.<br />(unless, of course, you are okay with entering you wifi password.)</p></div></body></html>";

  server.send(200, "text/html", Page);
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/** Wifi config page handler */
void handleWifi() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += F(
            "<html><head></head><body>"
            "<h1>Wifi config</h1>");
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>");
  }
  Page +=
    String(F(
             "\r\n<br />"
             "<table><tr><th align='left'>SoftAP config</th></tr>"
             "<tr><td>SSID ")) +
    String(softAP_ssid) +
    F("</td></tr>"
      "<tr><td>IP ") +
    toStringIp(WiFi.softAPIP()) +
    F("</td></tr>"
      "</table>"
      "\r\n<br />"
      "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>");
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      Page += String(F("\r\n<tr><td>SSID ")) + WiFi.SSID(i) + ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F(" *")) + F(" (") + WiFi.RSSI(i) + F(")</td></tr>");
    }
  } else {
    Page += F("<tr><td>No WLAN found</td></tr>");
  }
  Page += String(F(
            "</table>"
            "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
            "<input type='text' placeholder='network' name='n' required/>"
            "<br /><input type='password' placeholder='password' name='p'/>"
            "<br /><label>For those that really know what they are doing:<br />Webserver url. max ")) +
            String(WEBSERVERURL_SIZE) +
            F(
            " chars</label>"
            "<br /><input type='text' size=") +
            String(WEBSERVERURL_SIZE) +
            F(
            " placeholder='DO NOT CHANGE! UNLESS YOU KNOW WHAT YOU ARE DOING.' name='url'/>"
            "<br /><input type='submit' value='Connect/Disconnect'/></form>"
            "<p>You may want to <a href='/'>return to the home page</a>.</p>"
            "</body></html>");
  server.send(200, "text/html", Page);
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  char* buff = new char[SSID_SIZE -1];
  char url[WEBSERVERURL_SIZE] = "";
  server.arg("n").toCharArray(ssid[nextIndex], SSID_SIZE -1);
  server.arg("p").toCharArray(buff, SSID_SIZE -1);
  server.arg("url").toCharArray(url, WEBSERVERURL_SIZE);
  Serial.print("url: ");
  Serial.println(url);
  Serial.print("WEBSERVERURL before: ");
  Serial.println(WEBSERVERURL);
  if (String(url).length() > 0) strcpy(WEBSERVERURL, url);
  Serial.print("WEBSERVERURL after: ");
  Serial.println(WEBSERVERURL);
  Serial.print("buff: ");
  Serial.println(buff);
  if (String(buff).length() > 0) strcpy(password[nextIndex], encryption.encrypt(buff));
  else strcpy(password[nextIndex], "");
  Serial.print("password[nextIndex]: ");
  Serial.println(password[nextIndex]);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  connect = String(ssid[finalIndex]).length() > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++) {
    message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message);
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.println("\nConfiguring access point...");
  WiFi.disconnect();
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound(handleNotFound);
  server.begin(); // Web server start
  Serial.println("HTTP server started");
  ConnectToEEPROM::Credentials creds;
  creds = credentials.load();

  for(unsigned short s = 0; s < WIFI_MEMORY; s++){
    if(String(creds.ssid[s]).length() == 0) break;
    strcpy(ssid[s], creds.ssid[s]);
    strcpy(password[s], creds.password[s]);
  }

  nextIndex = credentials.getNextIndex(ssid);

  for (int i = 0 ; i < WIFI_MEMORY ; i++) {
    if (String(ssid[i]).length() == 0) break;
    Serial.print(i);
    Serial.print(": ");
    Serial.print(ssid[i]);
    Serial.print(" - ");
    Serial.println(String(password[i]).length() > 0 ? "********" : "<no password>");
  }

  connect = (String(ssid[0]).length() > 0); // Request WLAN connect if there is a SSID

  /*
  horizontalServo.attach(SERVO_PIN1);
  verticalServo.attach(SERVO_PIN2);
  horizontalServo.write(horizontalServoPos);
  verticalServo.write(verticalServoPos);
  */
}

void connectWifi() {
  Serial.println("Connecting as wifi client...");
  int n = WiFi.scanNetworks();
  Serial.println(n);
  Serial.println((nextIndex == 0 and String(ssid[1]).length() != 0 ? WIFI_MEMORY : nextIndex+1));//(nextIndex == 0 ? 1 : nextIndex)));
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      Serial.print("i: ");
      Serial.println(i);
      for (int p = 0 ; p < (nextIndex == 0 and String(ssid[1]).length() != 0 ? WIFI_MEMORY : nextIndex+1) ; p++) {
        Serial.print("p: ");
        Serial.println(p);
        Serial.print("WiFi.SSID(i): ");
        Serial.print(String(WiFi.SSID(i)));
        Serial.print("  --  ssid[p]: ");
        Serial.println(String(ssid[p]));
        if (String(WiFi.SSID(i)) == String(ssid[p])) {
          Serial.print("WiFi.SSID: ");
          Serial.println(p);
          WiFi.disconnect(String(WiFi.SSID(i)));
          char* pass_temp = new char[SSID_SIZE];
          strcpy(pass_temp, encryption.decrypt(password[p]));
          WiFi.begin(ssid[p], pass_temp);
          int connRes = WiFi.waitForConnectResult();
          Serial.print("connRess: ");
          Serial.println(connRes);
          Serial.print("WiFi status: ");
          Serial.println(WiFi.status());
          if (WiFi.status() == WL_CONNECTED){
            WiFiClient client;
            HTTPClient http;
            http.begin(client, String(WEBSERVERURL) + String("/serial/Serial.out"));
            int httpCode = http.GET();
            http.end();
            Serial.print(ssid[p]);
            Serial.print(": httpCode = ");
            Serial.println(httpCode);
            if (httpCode > 0) {
              finalIndex = p;
              connectedToWifi = true;
              WiFi.softAPdisconnect (true);
              Serial.println("saving");
              credentials.save(ssid, password);
              nextIndex = credentials.getNextIndex(ssid);
              Serial.println("saving2");
              postSSID();
              Serial.println("saving3");
              return;
            }else WiFi.disconnect();
          }
        }
        Serial.println("no");
      }
    }
  }
}

void getWifiFromServer() {
  if (connect) {
    Serial.println("Connect requested");
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }
  {
    unsigned int s = WiFi.status();
    if (s == 0 && millis() > (lastConnectTry + 45000)) {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */
      if (String(ssid[0]) != String("")) connect = true;
    }
    if (status != s) { // WLAN status change
      Serial.print("Status: ");
      Serial.println(s);
      status = s;
      if (s == WL_CONNECTED) {
        /* Just connected to WLAN */
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid[finalIndex]);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
      } else if (s == WL_NO_SSID_AVAIL) {
        WiFi.disconnect();
      }
    }
    if (s == WL_CONNECTED) {
      MDNS.update();
    }
  }
  /** Do work:
      DNS */
  dnsServer.processNextRequest();
  /** HTTP */
  server.handleClient();
}

void moveCam(int addHori, int addVert);

void loop() {
  if(!connectedToWifi) getWifiFromServer();
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    /** Get and send image from Raspberry Pi through serial to server */
    {
      /*if (Serial.available() > 0) {
        String base64img = Serial.readString();
        Serial.println(base64img);
        String postmsg = String("frame_base64=") + base64img;
        postToServer(String(WEBSERVERURL) + String("/vidSaver.php"), postmsg);
      }*/
      String base64img = "";
      String len = "";
      if (Serial.available() > 0) {
        for (unsigned short i = 0; i < 10; i++){
          len += (char) Serial.read();
        }
        for (unsigned short i = 0; i < len.toInt(); i++){
          base64img += String((char) Serial.read());
          Serial.println(base64img);
        }
        if(base64img.length() > 0){
          String postmsg = String("frame_base64=") + base64img;
          Serial.println();
          Serial.print(len.toInt());
          Serial.print(":  ");
          Serial.println(postmsg);
          postToServer(String(WEBSERVERURL) + String("/vidSaver.php"), postmsg);
        }
      }
    }
    
    /** Get steering commands from server */
    {
      /** HTTPClient must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...) */
      HTTPClient httpSerial;

      /** configure server and url */
      httpSerial.begin(client, String(WEBSERVERURL) + String("/serial/Serial.out"));

      /** start connection and send HTTP header */
      int httpCode = httpSerial.GET();
      if (httpCode > 0) {
        /** HTTP header has been send and Server response header has been handled */

        /** file found at server */
        if (httpCode == HTTP_CODE_OK) {
          /** with API */
          String payload = httpSerial.getString();
          if(httpSerialPrev != payload){
            httpSerialPrev = payload;
            Serial.print("httpSerial payload: ");
            Serial.println(payload);
            drive.stop();
            drive.move(payload);
          }
        }
      } else {
        Serial.printf("[httpSerial] GET... failed, error: %s\n", httpSerial.errorToString(httpCode).c_str());
        drive.stop();
        if (millis() < lastGetErr + 20000 ){
          getErr++;
          Serial.print("Error messages left before reconnecting: ");
          Serial.println(MAX_GET_ERROR - getErr);
        }else getErr = 1;

        lastGetErr = millis();
        if (getErr > MAX_GET_ERROR){
          getErr = 0;
          connect = true;
          getWifiFromServer();
        }
      }
      httpSerial.end();
    }

    /** Get WiFi commands from server */
    {
      HTTPClient WiFiConnections;

      /** configure server and url */
      WiFiConnections.begin(client, String(WEBSERVERURL) + String("/serial/WiFiConnections.out"));

      /** start connection and send HTTP header */
      int httpCode = WiFiConnections.GET();
      if (httpCode > 0) {
        /** HTTP header has been send and Server response header has been handled */

        /** file found at server */
        if (httpCode == HTTP_CODE_OK) {
          /** with API */
          String payload = WiFiConnections.getString();
          if (WiFiConnectionsPrev != payload){
            WiFiConnectionsPrev = payload;
            Serial.print("WiFiConnections payload: ");
            Serial.println(payload);
            if (String(payload[0]) == String("c")){
              resetFunc();
            }else if (String(payload[0]) == String("r")){
              payload.remove(0,2);
              removeSSID(payload);
            }else if (String(payload[0]) == String("a")){
              payload.remove(0,2);
              String ssidAdd = "";
              String passAdd = "";
              for (int t = 0; t < payload.length(); t++){
                if(String(payload[t]) == String(" ")) {
                  ssidAdd = payload.substring(0, t);
                  payload.remove(0, ssidAdd.length() + 1);
                  passAdd = payload;
                }
              }
              addSSID(ssidAdd, passAdd);
            }
          }
        }
      } else {
        Serial.printf("[WiFiConnections] GET... failed, error: %s\n", WiFiConnections.errorToString(httpCode).c_str());
        drive.stop();
        if (millis() < lastGetErr + 20000 ){
          getErr++;
          Serial.print("Error messages left before reconnecting: ");
          Serial.println(MAX_GET_ERROR - getErr);
        }else getErr = 1;

        lastGetErr = millis();
        if (getErr > MAX_GET_ERROR){
          getErr = 0;
          connect = true;
          getWifiFromServer();
        }
      }
      WiFiConnections.end();
    }

  /** Get Camera movement instructions from server */
  /*
    {
      HTTPClient CameraSerial;

      //configure server and url
      CameraSerial.begin(client, String(WEBSERVERURL) + String("/serial/CamSerial.out"));

      // start connection and send HTTP header
      int httpCode = CameraSerial.GET();
      if (httpCode > 0) {
        //HTTP header has been send and Server response header has been handled

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          // with API
          String payload = CameraSerial.getString();
          //if (CameraSerialPrev != payload){
            //CameraSerialPrev = payload;
          //Serial.print("CameraSerial payload: ");
          //Serial.println(payload);
          if (payload.indexOf("r") != -1){
            moveCam(DEFAULTCAMHORIZONTAL, DEFAULTCAMVERTICAL);
          }if(payload.indexOf("h+") != -1){
            moveCam(CAMSTEP,0);
          }if(payload.indexOf("h-") != -1){
            moveCam(-CAMSTEP,0);
          }if(payload.indexOf("v+") != -1){
            moveCam(0,CAMSTEP);
          }if(payload.indexOf("v-") != -1){
            moveCam(0,-CAMSTEP);
          }
          //}
        }
      } else {
        Serial.printf("[WiFiConnections] GET... failed, error: %s\n", CameraSerial.errorToString(httpCode).c_str());
        drive.stop();
        if (millis() < lastGetErr + 20000 ){
          getErr++;
          Serial.print("Error messages left before reconnecting: ");
          Serial.println(MAX_GET_ERROR - getErr);
        }else getErr = 1;

        lastGetErr = millis();
        if (getErr > MAX_GET_ERROR){
          getErr = 0;
          connect = true;
          getWifiFromServer();
        }
      }
      CameraSerial.end();
    }*/
  }

  delay(25);
}

/*
void moveCam(int addHori, int addVert){
  horizontalServoPos += (horizontalServoPos + addHori > 180 || horizontalServoPos + addHori < 0 ? 0 : addHori);
  verticalServoPos += (verticalServoPos + addVert > 180 || verticalServoPos + addVert < 65 ? 0 : addVert);
  Serial.print(horizontalServoPos);
  Serial.print("  --  ");
  Serial.println(verticalServoPos);
  horizontalServo.write(horizontalServoPos);
  verticalServo.write(verticalServoPos);
}
*/
