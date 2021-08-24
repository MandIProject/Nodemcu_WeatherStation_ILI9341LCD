# Nodemcu_WeatherStation_ILI9341LCD and India Covid Statistics
Simple weather station project using Nodemcu, ILI9341 LCD and Openweathermap api.
For now it shows only IST time.
So the drawSun, drawMoon and drawEvening will work according to IST.
<p><strong>Parts used:</strong></p>
<ul>
  <li>Generic Nodemcu ESP8266</li>
  <li>Generic ILI9341 LCD 240x320 display</li>
</ul>
<p><strong>If you want to make this project:</strong></p>
<ul>
  <li>Install the libraries mentioned in the WeatherStation.ino file using Arduino IDE's library manager</li>
  <li>The fonts used are freely available in Adafuit library. You can search and see the available fonts and use them</li>
  <li>Don't forget to enter your Wifi name, Wifi password and blynk app authentication code in Weather1.h file</li>
  <li>To use any other time zone, just change the "Hour = Hour + 5" to your location's time difference from UTC -> "Hour = Hour + time_difference"</li>
</ul>

<ul>
  <li>The icons used in this project were taken from <a>https://www.vecteezy.com/</a></li>
  <li>Then convert from jpeg to c file bitmap format using <a>http://www.rinkydinkelectronics.com/t_imageconverter565.php</a></li>
  <li>Resize was done using normal photo editors</li>
  <li>Make a blynk app, set the virtual pin to V6 and connect it to the nodeMCU</li>
</ul>

<img src="20210604_220707.jpg" width="500" height="500">
