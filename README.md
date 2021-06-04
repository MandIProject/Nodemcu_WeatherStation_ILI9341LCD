# Nodemcu_WeatherStation_ILI9341LCD
Simple weather station project using Nodemcu, ILI9341 LCD and Openweathermap api.
For now it shows only IST time.
So the drawSun, drawMoon and drawEvening will work according to IST.
If you want to make this project:
<ul>
  <li>Install the libraries mentioned in the WeatherStation.ino file using Arduino IDE's library manager</li>
  <li>The fonts used are freely available in Adafuit library. You can search and see the available fonts and use them</li>
  <li>Don't forget to enter your Wifi name, Wifi password and blynk app authentication code in Weather1.h file</li>
  <li>To use any other time zone, just change the "Hour = Hour + 5" to your location's time difference from UTC -> "Hour = Hour + time_difference"</li>
</ul>
