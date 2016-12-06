# EspRfRemote
Sketch to use an ESP mit Arduino as a RF remote for chap-ass power outlets (BH9938)

Step 1:
install Arduino IDE

Step 2:
Add to File -> Preferences -> Additional Boards.. the url: http://arduino.esp8266.com/stable/package_esp8266com_index.json

Step 3:
Tools -> Boards Manager.. install ESP8266 by esp community

Step 4:
Connect your device prefferably WeMos and set serial port
And connect a 4433Mhs transmitter to pin D6 (or other you specify later in the web ui)

Step 5:
set ssid and password to your wifi crendentails

Step 6:
Upload sketch. Open Tools -> Serial Monitor with 115200 Baud rate

Step 7:
Successful connect will show ip
Use your browser to open the ip
http://<ip.of.the.device>

A simple UI will show up. You can control the parameters and the corresponding url that can be used to perform the action will be shown
