#define TXD2 17
#define RXD2 16

String I_voltage , Fault_voltage, O_voltage, O_load, I_frequency, Battery_voltage, Temperature, Utility_Fail, Battery_Low, Bypass_Active, UPS_Failed, UPS_Type, Test_Progress, Shutdown_Active   ;
void setup()
{
  Serial.begin(115200);
  Serial2.begin(2400, SERIAL_8N1, RXD2, TXD2);
}
void loop()
{
  String comdata, comdata2;

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
    comdata2 = Serial2.readString();
//    Serial.println(comdata2);

    I_voltage = comdata2.substring(1, 6);
    Serial.println(I_voltage);

    Fault_voltage = comdata2.substring(7, 12);
    Serial.println(Fault_voltage);

    O_voltage = comdata2.substring(13, 18);
    Serial.println(O_voltage);

    O_load = comdata2.substring(19, 22);
    Serial.println(O_load);

    I_frequency = comdata2.substring(23, 27);
    Serial.println(I_frequency);

    Battery_voltage = comdata2.substring(28, 32);
    Serial.println(Battery_voltage);

    Temperature = comdata2.substring(33, 37);
    Serial.println(Temperature);

    Utility_Fail = comdata2.substring(38, 39);
    Serial.println(Utility_Fail);

    Battery_Low = comdata2.substring(39, 40);
    Serial.println(Battery_Low);

    Bypass_Active = comdata2.substring(40, 41);
    Serial.println(Bypass_Active);

    UPS_Failed = comdata2.substring(41, 42);
    Serial.println(UPS_Failed);

    UPS_Type = comdata2.substring(42, 43);
    Serial.println(UPS_Type);

    Test_Progress = comdata2.substring(43, 44);
    Serial.println(Test_Progress);

    Shutdown_Active = comdata2.substring(44, 45);
    Serial.println(Shutdown_Active);
  }
}
