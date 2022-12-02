#include <stdint.h>
#include <FastLED.h>
//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//定義靜態變數
static const uint8_t D0 = 16;
static const uint8_t D1 = 5;
static const uint8_t D2 = 4;
static const uint8_t D3 = 0;
static const uint8_t D4 = 2;
static const uint8_t D5 = 14;
// ----------

//定義常數
//#define LED 2
#define NUM_LEDS 30     //總數量
#define LED_TYPE WS2812 // 型號
#define COLOR_ORDER GRB // RGB LED 燈珠排列順序
// ----------


//wifi的設定資料(global var)
const char* ssid = "ZH";
const char* password = "-pmc4394";
const char* mqttServer = "192.168.32.178";

IPAddress ip (192,168,32,178); // The remote server ip to ping
// ----------

//Led燈條的設定資料(global var)
const uint8_t pin[] = {D0, D1, D2, D3, D4, D5};
const String r = "r";
const String g = "g";
const String b = "b";

//Led燈條的變數(global var)
CRGB leds1[NUM_LEDS]; // 建立光帶leds
CRGB leds2[NUM_LEDS]; //
CRGB leds3[NUM_LEDS]; //
CRGB leds4[NUM_LEDS]; //
CRGB leds5[NUM_LEDS]; //

CRGB RGBcolor(0, 0, 0); // RGBcolor（红色数值，绿色数值，蓝色数值）
CRGB RGBcolor_000(0, 0, 0);

// global var
int myLayout, myBegin, myEnd, mySegments;
String temp_color[] = {"r", "g", "b", "off"};
String str_msg= String("off");

char* led_status =" LedOff";
// ----------

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// 收到信息後的CallBack函數
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message Received from [");
  Serial.print(topic);                //顯示MQTT的topic
  Serial.print("] , value: ");

  char message[10]={0x00};
  for (int i = 0; i < length; i++) {
    message[i]=(char)payload[i];
    Serial.print((char)payload[i]);   //顯示MQTT的topic value(json資料形式)
  }
  message[length]=0x00;
  Serial.println("");
  Serial.print("Message Length(Bytes) ");
  Serial.println(length);

  StaticJsonDocument <256> doc;
  deserializeJson(doc, payload);
  const char* ledStrip_layout = doc["Layout"];  //解Layout的JSON序列化(Serialize)資料
  const char* ledStrip_begin = doc["Begin"];      //解Begin的JSON序列化(Serialize)資料
  const char* ledStrip_end = doc["End"];        //解End的JSON序列化(Serialize)資料
  const char* ledStrip_msg = doc["Msg"];        //解Msg的JSON序列化(Serialize)資料
  //顯示json資料
  Serial.print("ledStrip_Layout: ");
  Serial.print(ledStrip_layout);

  Serial.print("  ledStrip_Begin: ");
  Serial.print(ledStrip_begin);

  Serial.print("  ledStrip_End: ");
  Serial.print(ledStrip_end);

  Serial.print("  ledStrip_Msg: ");
  Serial.print(ledStrip_msg);
  Serial.print("  ");

  myLayout = atoi(ledStrip_layout);
  myBegin = atoi(ledStrip_begin);
  myEnd = atoi(ledStrip_end);
  mySegments = myEnd - myBegin + 1;
  switch(myLayout){
    case 1:
      Serial.print("ledStrip_Layout 1: ");
      Serial.print(ledStrip_layout);
      Serial.print(" Begin: ");
      Serial.print(myBegin);
      Serial.print(" Segments: ");
      Serial.print(mySegments);      
      break;
    case 2:
      Serial.print("ledStrip_Layout 2: ");
      Serial.print(ledStrip_layout);
      Serial.print(" Begin: ");
      Serial.print(myBegin);
      Serial.print(" Segments: ");
      Serial.print(mySegments);       
      break;
    case 3:
      Serial.print("ledStrip_Layout 3: ");
      Serial.print(ledStrip_layout);
      Serial.print(" Begin: ");
      Serial.print(myBegin);
      Serial.print(" Segments: ");
      Serial.print(mySegments);       
      break;
    case 4:
      Serial.print("ledStrip_Layout 4: ");
      Serial.print(ledStrip_layout);
      Serial.print(" Begin: ");
      Serial.print(myBegin);
      Serial.print(" Segments: ");
      Serial.print(mySegments);       
      break; 
    default:
      Serial.print("ledStrip_Layout 5: ");
      Serial.print(ledStrip_layout);
      Serial.print(" Begin: ");
      Serial.print(myBegin);
      Serial.print(" Segments: ");
      Serial.print(mySegments);                           
  }
  
  str_msg= String(ledStrip_msg);
  if (str_msg.equals("on")) {           // 如果收到的訊息為"on"
    //digitalWrite(LED, LOW);           // 點亮LED
    //Serial.println("Station1/Layout1/Led5 LED ON");
    String strLed(" Station1/");       // 初始化字串
    strLed = strLed + ledStrip_layout+"/" + ledStrip_begin + "/" + ledStrip_end + " LED_ON";
    
    Serial.println(strLed);
    led_status =" LedOn";
    
    leds_setup(myLayout, myBegin-1, mySegments, temp_color[0], 200);
    //delay(200);
    FastLED.show();
    
  } else if (str_msg.equals("off")){  // 如果收到的訊息為"off"                            
    //digitalWrite(LED, HIGH);        // 點滅LED
    //Serial.println("Station1/Layout1/Led5 LED OFF");
    String strLed(" Station1/");       // 初始化字串
    strLed = strLed + ledStrip_layout + "/" + ledStrip_begin + "/" + ledStrip_end + " LED_OFF";
    Serial.println(strLed);
    led_status =" LedOff";

    leds_setup(myLayout, myBegin-1, mySegments, temp_color[3], 0);
    //fill_solid(leds1, 30, RGBcolor_000);
    //fill_solid(leds1 + myBegin - 1 , mySegments, RGBcolor_000);
    //fill_solid(leds1 + 0, 30, RGBcolor_000);
    //delay(200);
    FastLED.show();
  } 
}

void setup() { 
  //pinMode(LED, OUTPUT);     // 設定LED port為輸出模式
  //digitalWrite(LED, HIGH);  // 設定LED off
  init_led_output();          // 初始化燈條
  
  Serial.begin(9600);         // 設定serial monitor 通訊速率
 
  WiFi.mode(WIFI_STA);        // 設定ESP8266工作模式為無線終端模式
    
  connectWifi();              // 連線WiFi
  
  mqttClient.setServer(mqttServer, 1883);   // 設定MQTT服務和port  
  mqttClient.setCallback(mqttCallback);     // 設定MQTT訂閱callback
  
  connectMQTTserver();                      // 連線MQTT服務
}

void loop() {
  String temp_color[] = {"r", "g", "b"};
  
  delay(200);

  mqttClient.loop();
}

// ESP8266連線wifi
void connectWifi(){ 
  WiFi.begin(ssid, password);
  //WiFi.begin(ssid);     //if no password  
 
  //等待WiFi連線,成功連線後輸出成功訊息
  Serial.print("Waitting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   // You can get IP address assigned to ESP8266    
}

// 連線MQTT服務并訂閱信息
void connectMQTTserver(){
  // 客户端ID
  String clientId = "CLIENT-esp8266-1";
 
  // 連線MQTT服務
  if (mqttClient.connect(clientId.c_str(), NULL, NULL)) { 
    Serial.println("MQTT Server Connected.");
    Serial.println("Server Address: ");
    Serial.println(mqttServer);
    Serial.println("ClientId: ");
    Serial.println(clientId);
    subscribeTopic(); // 訂閱指定主题
  } else {
    Serial.print("MQTT Server Connect Failed. Client State:");
    Serial.println(mqttClient.state());
    delay(1000);
  }   
}

// 訂閱指定主题
void subscribeTopic(){ 
  String topicString = "Station1";
  char subTopic[topicString.length() + 1];  
  strcpy(subTopic, topicString.c_str());
  
  // 確認訂閱主题是否訂閱成功
  if(mqttClient.subscribe(subTopic)){
    Serial.println("Subscrib Topic: ");
    Serial.println(subTopic);
  } else {
    Serial.print("Subscribe Fail...");
  }  
}

//初始化燈條,輸出腳位 D1~D5 對應 leds1~leds5
void init_led_output(){
  //delay(2000);
  delay(1000);
  LEDS.addLeds<LED_TYPE, D1, COLOR_ORDER>(leds1, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, D2, COLOR_ORDER>(leds2, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, D3, COLOR_ORDER>(leds3, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, D4, COLOR_ORDER>(leds4, NUM_LEDS);
  LEDS.addLeds<LED_TYPE, D5, COLOR_ORDER>(leds5, NUM_LEDS);
  //delay(1000);
  delay(500);
}

//關閉全部leds
void leds_all_off(){
  fill_solid(leds1 + 0, 30, RGBcolor_000);
  fill_solid(leds2 + 0, 30, RGBcolor_000);
  fill_solid(leds3 + 0, 30, RGBcolor_000);
  fill_solid(leds4 + 0, 30, RGBcolor_000);
  fill_solid(leds5 + 0, 30, RGBcolor_000);

  FastLED.show();
}

void leds_all_on(String color_val, uint8_t max_bright) //顏色：r,g,b 亮度:0~255
{
    leds_color_set(color_val);
    FastLED.setBrightness(max_bright);
    fill_solid(leds1 + 0, 30, RGBcolor);
    fill_solid(leds2 + 0, 30, RGBcolor);
    fill_solid(leds3 + 0, 30, RGBcolor);
    fill_solid(leds4 + 0, 30, RGBcolor);
    fill_solid(leds5 + 0, 30, RGBcolor);
    
    FastLED.show();
}

void leds_setup(int layout, int address_x, int leds_count, String color_val, uint8_t max_bright) //(層,起始燈,亮幾顆,亮什麼顏色：r,g,b , 亮度:0~255)
{
    leds_all_off();
    leds_color_set(color_val);
    FastLED.setBrightness(max_bright); // 亮度
    switch (layout){
    case 1:
        fill_solid(leds1 + address_x, leds_count, RGBcolor);
        break;
    case 2:
        fill_solid(leds2 + address_x, leds_count, RGBcolor);
        break;
    case 3:
        fill_solid(leds3 + address_x, leds_count, RGBcolor);
        break;
    case 4:
        fill_solid(leds4 + address_x, leds_count, RGBcolor);
        break;
    case 5:
        fill_solid(leds5 + address_x, leds_count, RGBcolor);
        break;
    default:
        break;
    }
}

void leds_color_set(String color_val) // set RGBcolor
{
    RGBcolor.r = 0;
    RGBcolor.g = 0;
    RGBcolor.b = 0;

    if (color_val == "r")
    {
        RGBcolor.r = 255;
    }
    else if (color_val == "g")
    {
        RGBcolor.g = 255;
    }
    else if (color_val == "b")
    {
        RGBcolor.b = 255;
    }
    else
    {
        RGBcolor.r = 0;
        RGBcolor.g = 0;
        RGBcolor.b = 0;
    }
}
