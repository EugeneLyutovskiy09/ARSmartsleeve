# ARSmartsleeve
ArduinoSketches:

1.Thermistor_Photoresistor. This code was written to test out the sensors and play around with the mapping function. I am not sure if I will need this one futher on in my project. (only in case i have enough time to implement the Sleeve itself) 

2.UduinoPackage. This is an arduino sketch for one of the assets I found in unity assets store. This package helps you set up connection between arduino and unity. It can recieve readings from the pins from the Arduino straight to the unity via the COM-port and assign the values to the objects.

3. Uduino_Wifi. This code is supposed to return IP andress and the number of the port in the serial display that I will need for the Unity Uduino_Wifi asset. That is what i needed the ESP8266's ip address and the port for. P.S: it does not work so far. That is why I am lookig for different way now.

4. connection_and_IP_esp8622WIFI. This is a code returns ip address of the ESP8266 wifi module (which turned out to be terribly hard to set up)
This code is needed for setting up IoT to transfer data to the cloud and from there into the Unity. As I could not make the Uduino_Wifi sketch work I found this one to connect the module to wifi and recieve the IP address (before that I had thought that my hardware was at fault) the code works and connects the module. I got the IP address and the module seems to be working fine, however I still cannot set up connection with the Unity via the Uduino_Wifi asset. 

Unity:

1.AssignArduinoReadings C++ code assigning the slider readings from the Tsensor. Just a testing version (turned out to be extremely hard without programming background)
