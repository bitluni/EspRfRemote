/*
Released under Creative Commons Attribution 4.0
by bitluni 2016
https://creativecommons.org/licenses/by/4.0/
Attribution means you can use it however you like as long you
mention that it's base on my stuff.
I'll be pleased if you'd do it by sharing http://youtube.com/bitlunislab
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "RF.h"

const char* ssid = "...";
const char* password = "...";

const int RF_OSC = 200;

ESP8266WebServer server(80);

void handleRoot() {
  String message = "<html><head></head><body style='font-family: sans-serif; font-size: 12px'>";
  message += "Send RF code:<br>";
  message += "<form id='data' action='/rf' target='response'>";
  message += "Output pin D<input name='D' type='number' value='6' style='width:40px' min='0' max='8'> ";
  message += "pulse<input name='t' type='number' value='200' style='width:60px' min='0' max='1000'>µs ";
  message += "ID<input name='id' type='number' value='28013' style='width:80px' min='0' max='1048575'> ";
  message += "<input name='on' type='hidden' value='1'>";
  message += "channel<input name='channel' type='number' value='0' style='width:40px' min='0' max='2'> ";
  message += "<script>function send(on){ document.getElementsByName('on')[0].value = on; document.getElementById('data').submit() }</script>";
  message += "<input type='button' value='on' onclick='send(1)'><input type='button' value='off' onclick='send(0)'>";
  message += "</form><br>";
  message += "<iframe name='response' style='height: 40px; border: 0'></iframe><br><br>";
  message += "have fun -<a href='http://youtube.com/bitlunislab'>bitluni</a></body></html>";
  server.send(200, "text/html", message);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

int getArgValue(String name)
{
  for (uint8_t i = 0; i < server.args(); i++)
    if(server.argName(i) == name)
      return server.arg(i).toInt();
  return -1;
}

void handleRf()
{
	const int pinNumbers[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};
	int pin = getArgValue("D");
	int t = getArgValue("t");
	if(t == -1) t = RF_OSC;
	int id = getArgValue("id");
	int ch = getArgValue("channel");
	int on = getArgValue("on");
	String out = "http://";
	out += WiFi.localIP();
	out += "/rf?D=";
	out += pin;
	out += "&t=";
	out += t;
	out += "&id=";
	out += id;
	out += "&channel=";
	out += ch;
	out += "&on=";
	out += on;
	pinMode(pinNumbers[pin], OUTPUT);
	for(int i = 0; i < 5; i++)
		rfWriteCode(pinNumbers[pin], t, id, (1 << (ch + 1)) | (on > 0? 1: 0));
	server.send(200, "text/plain", out);	
}

void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/rf", handleRf);  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
}
