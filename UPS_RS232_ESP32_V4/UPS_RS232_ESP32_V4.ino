#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> //字串轉JSON轉換https://www.chosemaker.com/board/esp32/lesson-9/

#define TXD2 17
#define RXD2 16

const char* ssid = "XXXX";
const char* password = "XXXXX";
WiFiClient wifiClient;

const char* mqttServer = "broker.emqx.io";  // MQTT伺服器位址 //broker.emqx.io //mqtt3.thingspeak.com
const char* clientID = "mqttx_sdsasad";      // 用戶端ID，Client ID。
const char* mqttUserName = "";  // 使用者名稱，Username。
const char* mqttPwd = "";  //  使用者密碼，Password

const char* topicToSub = "channels/2344234/subscribe/fields/field3"; //訂閱in
const char* topicToPub = "channels/2344234/publish/fields/field3"; //發佈out
PubSubClient mqttClient(wifiClient);

String comdata3, I_voltage , Fault_voltage, O_voltage, O_load, I_frequency, Battery_voltage, Temperature, Utility_Fail, Battery_Low, Bypass_Active, UPS_Failed, UPS_Type, Test_Progress, Shutdown_Active   ;
char json_output[350];

unsigned long int previous_millis_A = 0; //初始化以前的millis
unsigned long int interval_A = 10 * 1000; //間隔5秒執行

void mqttConnect() {
  while (!mqttClient.connected()) {
    Serial.print("正在嘗試 MQTT 連接...");

    if (mqttClient.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("連接");

      mqttClient.publish(topicToPub, clientID); //發布MQTT主題與訊息
      mqttClient.subscribe(topicToSub); //訂閱MQTT

    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println("5 秒後重試");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String receivedMsg;
  for (int i = 0; i < length; i++) {
    receivedMsg += (char)message[i];
  }
  Serial.print("訊息到達主題： ");
  Serial.print(topic);
  Serial.print("Message: ");
  Serial.println(receivedMsg);
  if (String(topic) == topicToSub) {
    if (receivedMsg == ("on")) {
      Serial.println("on");
      digitalWrite(LED_BUILTIN, HIGH);
      Serial2.write("T\r");
      mqttClient.publish(topicToSub, "off");
    } else if (receivedMsg == "off") {
      Serial.println("off");
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

//---------字串轉JSON--------------------------------------
String String_To_JSON()
{
  Serial2.write("Q1\r");
  while (Serial2.available() > 0)
  {
    comdata3 = Serial2.readString();
  }
  I_voltage = comdata3.substring(1, 6).toFloat();  //擷取子字串substring  //字串轉浮點數toFloat
  Fault_voltage = comdata3.substring(7, 12).toFloat();
  O_voltage = comdata3.substring(13, 18).toFloat();
  O_load = comdata3.substring(19, 22).toFloat();
  I_frequency = comdata3.substring(23, 27).toFloat();
  Battery_voltage = comdata3.substring(28, 32).toFloat();
  Temperature = comdata3.substring(33, 37).toFloat();
  Utility_Fail = comdata3.substring(38, 39).toInt();
  Battery_Low = comdata3.substring(39, 40).toInt();
  Bypass_Active = comdata3.substring(40, 41).toInt();
  UPS_Failed = comdata3.substring(41, 42).toInt();
  UPS_Type = comdata3.substring(42, 43).toInt();
  Test_Progress = comdata3.substring(43, 44).toInt();
  Shutdown_Active = comdata3.substring(44, 45).toInt();

  String msgStr =  msgStr + "{\"I_voltage\":" + I_voltage + ",\"Fault_voltage\":" + Fault_voltage + ",\"O_voltage\":" + O_voltage + ",\"O_load\":" + O_load + ",\"I_frequency\":" + I_frequency\
                   + ",\"Battery_voltage\":" + Battery_voltage + ",\"Temperature\":" + Temperature + ",\"Utility_Fail\":" + Utility_Fail + ",\"Battery_Low\":" + Battery_Low + ",\"Bypass_Active\":" + Bypass_Active\
                   + ",\"UPS_Failed\":" + UPS_Failed + ",\"UPS_Type\":" + UPS_Type + ",\"Test_Progress\":" + Test_Progress + ",\"Shutdown_Active\":" + Shutdown_Active + "}";
  //"}";/*+*/
  String X = msgStr;
  msgStr = "";
  return  X;
}


void setup() {

  Serial.begin(115200);
  Serial2.begin(2400, SERIAL_8N1, RXD2, TXD2);

  // Connect to wifi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("連接到");
  Serial.println(ssid);
  Serial.print("IP 地址: ");
  Serial.println(WiFi.localIP());

  // MQTT setup
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(callback);

  // Led setup
  pinMode(LED_BUILTIN, OUTPUT);

  //MQTT_MAX_MESSAGE_SIZE預設為256
  Serial.println("改前:" + String(mqttClient.getBufferSize()));
  mqttClient.setBufferSize (350);
  Serial.println("改後:" + String(mqttClient.getBufferSize()));
}

void loop() {

  // 檢查我們是否仍然連接到 MQTT 代理
  if (!mqttClient.connected()) {
    mqttConnect();
  }

  // 讓 PubSubClient library 運行
  mqttClient.loop();

  // 在此處添加您的發布代碼 --------------------
  // 選項 1 -- 基於計時器發布
  // 僅每 5 秒發布一次

  unsigned long int current_millis_A = millis();              //current_millis_A = 2000
  if ((current_millis_A - previous_millis_A) >= interval_A) { //(2000 - 1000) >= 1000
    previous_millis_A = current_millis_A;

    //    Serial.println(String_To_JSON()); // 顯示自定的函式String_To_JSON
    //    Serial.println(Fault_voltage);



    String_To_JSON().toCharArray(json_output, 350);//將自定的函式String_To_JSON轉成陣列並大小設定為300

    //    Serial.print("Publish message: ");
    Serial.println(json_output);// 將字串傳入自定的函式String_To_JSON
    mqttClient.publish(topicToPub, json_output); // 發布MQTT主題與訊息
    //      msgStr = "";


  }
  //-----------------------------------------------
}
