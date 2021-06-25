#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>  // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Wifi network station credentials
#define WIFI_SSID "Mi11lite5g"
#define WIFI_PASSWORD "123456789"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "1860656007:AAF46qcXFcgjtCyXjEmP7Vo84kvCeDaiAT8"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

unsigned long bot_lasttime; // last time messages' scan has been done
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages");
  Serial.println(numNewMessages);
  
  String answer, aux;
  for (int i = 0; i < numNewMessages; i++){
  
  
    telegramMessage &msg = bot.messages[i];
    Serial.println("Received " + msg.text);
    if(msg.text=="1" or msg.text=="2" or msg.text=="3" or msg.text=="4" or msg.text=="5" or msg.text=="6" or msg.text=="7" or msg.text=="8" or msg.text=="9" or msg.text=="10" or msg.text=="0"){
       answer.clear();
      Serial.println("Received"  + msg.text);
         if(msg.text=="1" or msg.text=="2" or msg.text=="3" or msg.text=="4" or msg.text=="5" or msg.text=="6" or msg.text=="7" or msg.text=="8" or msg.text=="9" or msg.text=="10" or msg.text=="0"){

           if(msg.text=="0" or msg.text=="1" or msg.text=="2" or msg.text=="3" or msg.text=="4"){
            answer = "bipppp....bipppp.. Fallo en el sistema 👾👾";
          }
          else if(msg.text=="5" or msg.text=="6"){
            answer = "¿Está usted seguro?🥺🥺";
          }
           else if(msg.text=="7" or msg.text=="8"){
             answer="No está nada mal!!!👍🏼👍🏼👍🏼" ; 
           }
           else if(msg.text=="9" or msg.text=="10"){

             answer= "Muchas gracias!💪🤖🤖🤖🦾"; 
           }
        }else if(!(msg.text=="0" and msg.text=="10")) {answer= "La nota que ha introducido no es correcta vuelve a intentarlo";}
        bot.sendMessage(msg.chat_id, answer, "Markdown");
        aux.clear();

    }else if (msg.text == "/start"){
      answer = "Saludos " + msg.from_name + ".\n";
      answer += "Introduzca uno de los siguientes comandos para utilizarme... \n\n";
      answer += "/start iniciarme \n";
      answer += "/status En que estado me encuentro \n";
      answer += "/nota Espero que sea buena... \n";
      answer += "/Manuel Me suena este nombre...\n";
      bot.sendMessage(msg.chat_id, answer, "Markdown");}

    else if (msg.text == "/status"){
      answer = "Gracias por preguntar yo estoy bien, ¿y usted?";
      bot.sendMessage(msg.chat_id, answer, "Markdown");
      }
     else if (msg.text == "/Manuel"){
      answer = "Buenos días Sr. Palma, ¿qué tal su día?";
      bot.sendMessage(msg.chat_id, answer, "Markdown");}
      else if (msg.text == "/nota"){
      answer = "¿Sr. Palma,¿Qué nota me pondría🥺🥺? bip... bip... \nPonga la nota que usted crea que merezco...";
      bot.sendMessage(msg.chat_id, answer, "Markdown");  
     aux="/nota";
     }
    else
      { answer = "Error comando no encontrado";
       bot.sendMessage(msg.chat_id, answer, "Markdown");}

   
  
  }
}
void bot_setup()
{
  const String commands = F("["
                            "{\"command\":\"start\", \"description\":\"Message sent when you open a chat with a bot\"},"
                            "{\"command\":\"status\",\"description\":\"Answer device current status\"},"
                            "{\"command\":\"Manuel\",\"description\":\"Pregunta al Sr. Palma\"},"
                            "{\"command\":\"Nota\",\"description\":\"Pregunta sobre la nota\"}" // no comma on last command
                            "]");
  bot.setMyCommands(commands);
  //bot.sendMessage("25235518", "Hola amigo!", "Markdown");
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  bot_setup();
}
void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}