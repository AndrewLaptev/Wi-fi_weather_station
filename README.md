# Wi-fi_weather_station
This project is based on code from ["ESP8266 weather station using Arduino IDE"](https://circuits4you.com/2019/03/22/esp8266-weather-station-arduino/) tutorial
but another sensors and with significant revisions in backend.

The weather station is based on the NodeMCU ESP8266 board
<p align="center">
  <img src="https://github.com/AndrewLaptev/wi-fi_weather_station/blob/master/docs/nodemcuv3_pinout.png"/>
</p>

and three sensors:
* BMP180 - temperature and pressure (degrees Celsius and bar)
* BH1750 - illumination (lux)
* AM2320 - humidity (can also measure temperature)
<p align="center">
  <img src="https://github.com/AndrewLaptev/wi-fi_weather_station/blob/master/docs/scheme.png"/>
</p>

NodeMCU connect to Wi-Fi AP and runs the web server with site. The measurements from all sensors are sent to the web server in JSON format and site display current date and measurements every 2 second with JS script. The HTML, CSS and JS web page code is stored in the [index.h](https://github.com/AndrewLaptev/wi-fi_weather_station/tree/master/include).

<p align="center">
  <img src="https://github.com/AndrewLaptev/wi-fi_weather_station/blob/master/docs/demo.png"/>
</p>
