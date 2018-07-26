#include <TinyGPS.h>
#include <SoftwareSerial.h>
SoftwareSerial ss(3, 4);
float flat, flon;
 unsigned long age;
String latt,lngg;
int temp=0;
TinyGPS gps;
void setup() 
{ 
Serial.begin(9600); 
ss.begin(9600);
 gsm_init();
 Serial.println("AT+CNMI=2,2,0,0,0"); 
 };

void gsm_init()
{
 
  boolean at_flag=1;
  while(at_flag)
  {
    Serial.println("AT");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      at_flag=0;
    }
    
    delay(1000);
  }

  boolean echo_flag=1;
  while(echo_flag)
  {
    Serial.println("ATE0");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }
 delay(1000);
  boolean net_flag=1;
  while(net_flag)
  {
    Serial.println("AT+CPIN?");
    while(Serial.available()>0)
    {
      
      if(Serial.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  delay(1000);
  delay(1000);
}

void serialEvent()
{
  while(Serial.available())
  {
    if(Serial.find("Track Vehicle"))
    {
      temp=1;
      break;
    }
    else
    temp=0;
  }
}

void init_sms()
{
  Serial.println("AT+CMGF=1");
  delay(400);
  Serial.println("AT+CMGS=\"+917834822414\"");   // use your 10 digit cell no. here
  delay(400);
}

void send_data(String message)
{
  Serial.println(message);
  delay(200);
}


void tracking()
{
    init_sms();
    send_data("Vehicle Tracking Alert:");
    send_data("Your Vehicle Current Location is:");
    Serial.print("Latitude:");
    send_data(latt);
    Serial.print("Longitude:");
    send_data(lngg);
    send_data("Please take some action soon..\nThankyou");
    send_sms();
    delay(500);
    delay(2000);
}

 void send_sms()
{
  Serial.write(26);
}

void loop() 
{
  
serialEvent();
  if(temp)
  {
    get_gps();
  
   
tracking();
  }
}

void get_gps()
{
     bool newData = false;
  unsigned long chars;
 
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      //Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }                             
  }
 
  if (newData)      //If newData is true
  {
    
    gps.f_get_position(&flat, &flon, &age);   
   latt=String(flat==TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
  lngg=String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
   
    
    delay(200);
}
}







