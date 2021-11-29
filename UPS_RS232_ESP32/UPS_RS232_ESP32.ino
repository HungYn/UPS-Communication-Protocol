#define TXD2 17
#define RXD2 16
void setup()
{
  Serial.begin(2400);
  Serial2.begin(2400, SERIAL_8N1, RXD2, TXD2);
}
void loop()
{
  String comdata;
  while (Serial.available() > 0)
  {
    comdata = Serial.readString(); //Serial.readString 讀取文字  //Serial.read 讀字元
    if (comdata == "1")
    {
      Serial2.write("Q1\r");
      Serial.println("開啟");
    }
    else if (comdata == "2")
    {
      Serial2.write("T\r");
      Serial.println("自我測試10秒");
    }
    else if (comdata == "3")
    {
      Serial2.write("CT\r");
      Serial.println("結束自我測試");
    }
    else
    {
      Serial2.write((comdata + "\r").c_str());
      Serial.println(comdata);
    }
  }
  if (Serial2.available() > 0)
  {
    Serial.print("傳回內容：");
    Serial.println(Serial2.readString());
  }
}
