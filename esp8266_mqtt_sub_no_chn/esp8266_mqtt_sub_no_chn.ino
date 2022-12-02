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
int stripArray[30];
//int* stripArray;
int stripIndex=0;
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
    String strLed(" Station1/");        // 初始化字串
    strLed = strLed + ledStrip_layout+"/" + ledStrip_begin + "/" + ledStrip_end + " LED_ON";
    
    Serial.println(strLed);
    led_status =" LedOn";
    
    strip_color_on(); //power on指定燈條位置    
  } else if (str_msg.equals("off")){    // 如果收到的訊息為"off"                            
    //digitalWrite(LED, HIGH);          // 點滅LED
    //Serial.println("Station1/Layout1/Led5 LED OFF");
    String strLed(" Station1/");        // 初始化字串
    strLed = strLed + ledStrip_layout + "/" + ledStrip_begin + "/" + ledStrip_end + " LED_OFF";
    Serial.println(strLed);
    led_status =" LedOff";

    strip_color_off();  //power off指定燈條位置 
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

void strip_color_on(){
  boolean myTest=false;

  //檢查array內是否有值, true: 有
  for (int i=0; i<30; i=i+2) {
    if (myBegin-1 == stripArray[i] && mySegments == stripArray[i+1]) {
      myTest=true;
      Serial.println("on STRIP1: " +String(i)+" "+ String(stripArray[i])+" "+String(stripArray[i+1]));        
      break;
    }
  }
  
  Serial.println("on STRIP2: "+String(myTest));
  
  //新值加入
  if (myTest==false) {
    Serial.println("on STRIP3: "+String(stripIndex));
    stripArray[stripIndex]=myBegin-1;
    stripIndex++;
    stripArray[stripIndex]=mySegments;
    stripIndex++;
    Serial.println("on STRIP4: "+String(stripArray[stripIndex-2])+" "+String(stripArray[stripIndex-1]));
  }
 
  for (int i = 0; i < 30; i=i+2) {
    if (stripArray[i] !=0 || stripArray[i+1] !=0) {      
      for (int j = stripArray[i]; j < stripArray[i]+stripArray[i+1]; j++){
        leds1[j] = CRGB::Red;
        FastLED.show();
        delay (10);
      }
    }
  }
}

void strip_color_off(){
  //檢查array內是否有值, true: 有
  for (int i=0; i<30; i=i+2) {
     if (myBegin-1 == stripArray[i] && mySegments == stripArray[i+1]) {
       Serial.println("off STRIP11: " +String(i)+" "+ String(stripArray[i])+" "+String(stripArray[i+1]));  
       for (int j = stripArray[i]; j < stripArray[i]+stripArray[i+1]; j++){
         leds1[j] = CRGB::Black;
         FastLED.show();
         delay (10);
       }
       stripArray[i]=0;   
       stripArray[i+1]=0;
       Serial.println("off STRIP22: " +String(i)+" "+ String(stripArray[i])+" "+String(stripArray[i+1]));          
       break;
     }
  }
}
