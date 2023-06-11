#include <ESP8266WiFi.h>
#include <Servo.h>
#include "SSD1306Wire.h"
#include "text.h"
#define MAX_SRV_CLIENTS 3


/* 设置oled屏幕的相关信息 */
const int I2C_ADDR = 0x3c;              // oled屏幕的I2c地址
#define SDA_PIN 0                       // SDA引脚，默认gpio4(D2)
#define SCL_PIN 4                       // SCL引脚，默认gpio5(D1)

/* 新建一个oled屏幕对象，需要输入IIC地址，SDA和SCL引脚号 */
SSD1306Wire oled(I2C_ADDR, SDA_PIN, SCL_PIN);

#define IN1 16
#define IN2 05  
#define IN3 13
#define IN4 15

#define ZX 

Servo myServo;
int pos = 0;
bool is1 = false;
bool is2 = false;
 

const char* ssid = "Galaxy S22";
const char* password = "hf200080";


WiFiServer server(8080);

WiFiClient serverClients[MAX_SRV_CLIENTS];
WiFiClient serverClients2[MAX_SRV_CLIENTS];
String IP = "";
String str = "";
String sudu = "gao";

void drawRect(void);
void Server();
int GETWIFI();


void setup() {
  // put your setup code here, to run once:

  /* oled屏幕初始化 */
  oled.init();
  oled.flipScreenVertically();          // 设置屏幕翻转
  oled.setContrast(255);                // 设置屏幕亮度
  drawRect();                           // 测试屏幕显示
  oled.clear(); oled.display();         // 清除屏幕

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);


  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  myServo.attach(02);

  // digitalWrite(IN1,1);
  // analogWrite(IN1,100);

  GETWIFI();
}

void loop() {
  // put your main code here, to run repeatedly:

  uint8_t i;


  if (server.hasClient()){
    for (i =0;i<MAX_SRV_CLIENTS;i++){
      if (!serverClients[i] || serverClients[i].connected()){
        if (!serverClients[i]){
          serverClients[i].stop();
        }
        serverClients[i] = server.available();
        Serial.print("\n 新的客户端：");
        Serial.print(i);
        continue;
      }
    }
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  for (i = 0; i< MAX_SRV_CLIENTS;i++){

    if (serverClients[i] && serverClients[i].connected()){
      if (serverClients[i].available()){
        Serial.print("\n");
        while(serverClients[i].available()){
          str += serverClients[i].read();          
        }

        Serial.print(str);
        if (str == "2"){
          digitalWrite(IN2,0);
          digitalWrite(IN4,0);
          if (sudu == "gao"){
            digitalWrite(IN1,1);
            digitalWrite(IN3,1);
          }
          else if (sudu == "zhong"){
            analogWrite(IN1,200);
            analogWrite(IN3,200);
          }
          else if (sudu == "di"){
            analogWrite(IN1,100);
            analogWrite(IN3,100);
          }
                    
        }

        else if (str == "13"){
          oled.clear();oled.display();
          delay(500);

          oled.drawString(4, 0, "IP:"+IP);

          oled.drawFastImage(4 + 15*0, 16, 16, 16, text2[5]);
          oled.drawFastImage(4 + 15*1, 16, 16, 16, text2[6]);
          oled.drawFastImage(4 + 15*2, 16, 16, 16, text2[7]);
          oled.drawFastImage(4 + 15*3, 16, 16, 16, text2[4]);
          oled.drawFastImage(4 + 15*4, 16, 16, 16, text2[9]);
          oled.drawFastImage(4 + 15*5, 16, 16, 16, text2[10]);
          oled.display();
        }
        else if (str == "14"){
          oled.clear();oled.display();
          delay(500);

          oled.drawString(4, 0, "IP:"+IP);


          oled.drawFastImage(4 + 15*0, 16, 16, 16, text2[5]);
          oled.drawFastImage(4 + 15*1, 16, 16, 16, text2[6]);
          oled.drawFastImage(4 + 15*2, 16, 16, 16, text2[7]);
          oled.drawFastImage(4 + 15*3, 16, 16, 16, text2[8]);
          oled.drawFastImage(4 + 15*4, 16, 16, 16, text2[9]);
          oled.drawFastImage(4 + 15*5, 16, 16, 16, text2[10]);
          oled.display();
        }

        else if (str == "1"){
          digitalWrite(IN1,0);
          digitalWrite(IN3,0);
          if (sudu == "gao"){
            digitalWrite(IN2,1);
            digitalWrite(IN4,1);
          }
          else if (sudu == "zhong"){
            analogWrite(IN2,200);
            analogWrite(IN4,200);
          }
          else if (sudu == "di"){
            analogWrite(IN2,100);
            analogWrite(IN4,100);
          }
        }
        else if (str == "3"){
          myServo.write(180);
          // for (pos = 90;pos <=180;pos++){
          //   myServo.write(pos);         
          // }
        }
        else if (str == "4"){
          myServo.write(0);
          // for (pos = 90;pos >=0;pos--){
          //   myServo.write(pos);
          // }
        }
        else if (str == "5" || str == "6"){
          digitalWrite(IN2,0);
          digitalWrite(IN1,0);   
          digitalWrite(IN4,0);  
          digitalWrite(IN3,0);  
        }
        else if (str == "7" || str == "8"){
          myServo.write(90);
        }

        else if (str == "10"){
          sudu = "gao";
        }
        else if (str == "11"){
          sudu = "zhong";
        }
        else if (str == "12"){
          sudu = "di";
        }
        str = "";

        

      }
    }
  }
}

int GETWIFI(){
  WiFi.begin(ssid,password);
  Serial.print("\nConnect to ");
  Serial.print(ssid);
  Serial.print("\n");

  /* 显示汉字 */
  oled.clear();oled.display();
  oled.drawFastImage(4 + 15*0, 22, 16, 16, text[0]);
  oled.drawFastImage(4 + 15*1, 22, 16, 16, text[1]);
  oled.drawFastImage(4 + 15*2, 22, 16, 16, text[2]);
  oled.drawFastImage(4 + 15*3, 22, 16, 16, text[3]);
  oled.drawFastImage(4 + 15*4, 22, 16, 16, text[4]);
  oled.drawFastImage(4 + 15*5, 22, 16, 16, text[5]);
  oled.drawFastImage(4 + 15*6, 22, 16, 16, text[6]);
  oled.drawFastImage(4 + 15*7, 22, 16, 16, text[7]);
  oled.drawFastImage(15*8-2, 22, 16, 16, text[8]);
  oled.drawFastImage(15*9-2, 22, 16, 16, text[9]);
  oled.display();

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nConnected success\n");
    IP = WiFi.localIP().toString();
    Serial.print("IP is :");
    Serial.print(IP);
    Server();

    /* 显示汉字 */
    oled.clear();oled.display();
    delay(500);

    oled.drawString(4, 0, "IP:"+IP);


    oled.drawFastImage(4 + 15*0, 16, 16, 16, text2[5]);
    oled.drawFastImage(4 + 15*1, 16, 16, 16, text2[6]);
    oled.drawFastImage(4 + 15*2, 16, 16, 16, text2[7]);
    oled.drawFastImage(4 + 15*3, 16, 16, 16, text2[8]);
    oled.drawFastImage(4 + 15*4, 16, 16, 16, text2[9]);
    oled.drawFastImage(4 + 15*5, 16, 16, 16, text2[10]);


    oled.display();

    return 1;
  }
  else {
    Serial.print("\nConnected failed\n");
    return 0;
  }
  return 0;
}

void Server(){
  server.begin();
  server.setNoDelay(true);
}

void drawRect(void) {
  for (int16_t i=0; i<oled.getHeight()/2; i+=2) {
    oled.drawRect(i, i, oled.getWidth()-2*i, oled.getHeight()-2*i);
    oled.display();
    delay(50);
  }
  oled.clear();oled.display();

}