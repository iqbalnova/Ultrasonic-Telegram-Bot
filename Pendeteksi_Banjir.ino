#include "CTBot.h"
#include <WiFiClient.h>
#include <ThingSpeak.h>

CTBot myBot;
#define trigPin D7
#define echoPin D6

char ssid[] = "-fajri house-";
char pass[] = "iqbalfajri23";


const char* serverTS = "api.thingspeak.com";
String apiKey = "A5C4ABPQN7XGT4QE";   

String token = "1596009614:AAGjuaqMMVKrN6gCvrmQQs-AEfcP9ipMNyg";

long duration; 
int distance;
int tinggi;
String kondisi;

void setup()
{
  Serial.begin(9600);
  
  Serial.println("\nStarting TelegramBot...");
  Serial.print("Menyambungkan ke : ");
  Serial.println(ssid);
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
  {
    Serial.print("Terhubung dengan : ");
    Serial.println(ssid);
  }
  else
  {
    Serial.println("\nError.");
  }

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}



void loop()
{
  TBMessage msg;

      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
 
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
 
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2; 

      tinggi = 75 - distance;
      sendHeight(tinggi);
      
  if (myBot.getNewMessage(msg))
  {
    if (msg.text.equalsIgnoreCase("/cek"))
    {
      Serial.println("Mengirim pesan ke Telegram...");
      
      String balasan;
      balasan = (String)"Pembacaan Ketinggian\n" +
                (String)"Ketinggian Air Saat Ini : " + tinggi + (String)" cm\n";
     
      myBot.sendMessage(msg.sender.id, balasan);
    }

    else if (msg.text.equalsIgnoreCase("/kondisi"))
    {
  
      if (tinggi < 20)
      {
        kondisi = "Ketinggian Air Aman";
      }
      else if ((tinggi > 20) && (tinggi < 50 ))
      {
        kondisi = "Ketinggian Air Sedang";
      }
      else if (tinggi > 50)
      {
        kondisi = "Ketinggian Air Bahaya";
      }
      
      String balasan;
      balasan = (String)"Kondisi Ketinggian Air Saat Ini : " + kondisi + (String)" cm\n";
     
      myBot.sendMessage(msg.sender.id, kondisi);
    }
   
    
    else if (msg.text.equalsIgnoreCase("/start"))
    {
      Serial.println("Mengirim pesan ke Telegram...");

      String balasan;
      balasan = (String)"Welcome " + msg.sender.username + (String)".\n" +
                (String)"/cek : untuk mengetahui ketinggian saat ini.\n" +
                (String)"/kondisi : cek kondisi saat ini.";
      myBot.sendMessage(msg.sender.id, balasan);
    }
    else
    {
      Serial.println("Mengirim pesan ke Telegram...");
      String balasan;
      balasan = (String)"Pesan tidak ada!\n" +
                (String)"Silahkan cek kembali dengan\n" +
                (String)"mengirim pesan /start.";
      myBot.sendMessage(msg.sender.id, balasan);
    }
  }
  delay(500); 
}

void sendHeight(float tinggi)
  {  
  WiFiClient tclient;
   if (tclient.connect(serverTS, 80)) 
   { 
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(tinggi);
   postStr += "\r\n\r\n";
   delay(10000);
   tclient.print("POST /update HTTP/1.1\n");
   tclient.print("Host: api.thingspeak.com\n");
   tclient.print("Connection: close\n");
   tclient.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   tclient.print("Content-Type: application/x-www-form-urlencoded\n");
   tclient.print("Content-Length: ");
   tclient.print(postStr.length());
   tclient.print("\n\n");
   tclient.print(postStr);
   delay(100000);
   }

 tclient.stop();
}
