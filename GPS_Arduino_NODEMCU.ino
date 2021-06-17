//include these basic libarries
/*
Installation of these libraries would be updated soon
Schematics would be updated soon

*/
#include <TinyGPS++.h>//GPS library
#include <SoftwareSerial.h>//serialCommunication
#include <ESP8266WiFi.h>//ESP

TinyGPSPlus gps;  // The TinyGPS++ object

SoftwareSerial ss(4, 5); // The serial connection to the GPS device

const char* ssid = "//add you Wifi SSID password ";
const char* password = "//add your password";


//Variable Declaration
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;

WiFiServer server(80);//port 80
void setup()
{
  Serial.begin(115200);//baud
  ss.begin(9600);//baud
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)//check status
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  //if the serial monitor doesnt show up with the IP, it can been by using netstat -a command in cmd

}
//checking, getting coords from gps
void loop()
{

  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
      }

      if (gps.date.isValid())
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
          date_str = '0';
        date_str += String(date);

        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month);

        date_str += " / ";

        if (year < 10)
          date_str += '0';
        date_str += String(year);
      }

      if (gps.time.isValid())
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();

        minute = (minute + 30);
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;

        if (hour >= 12)
          pm = 1;
        else
          pm = 0;

        hour = hour % 12;

        if (hour < 10)
          time_str = '0';
        time_str += String(hour);

        time_str += " : ";

        if (minute < 10)
          time_str += '0';
        time_str += String(minute);

        time_str += " : ";

        if (second < 10)
          time_str += '0';
        time_str += String(second);

        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";

      }

    }
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  //Building a small UI using HTML
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>GPS Interfacing Project Msc CS</title> <style>";
  s += "a:link {background-color: RED;text-decoration: none;}";
  s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS Interfacing Project Msc CS</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details of my place</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude My place</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude My place </th> <td ALIGN=CENTER >";
  s += lng_str;
  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  s += date_str;
  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  s += time_str;
  s += "</td>  </tr> </table> ";

//checking if the location is Valid and adding the lat and long to the maps.google.com
  if (gps.location.isValid())
  {
     s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    s += lat_str;
    s += "+";
    s += lng_str;
    s += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";
  }

  s += "</body> </html> \n";

  client.print(s);
  delay(100);

}
