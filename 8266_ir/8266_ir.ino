#define BLINKER_PRINT Serial
#define BLINKER_MIOT_OUTLET
#define BLINKER_WIFI
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Blinker.h>

char auth[] = "";//这里面填写设备的密钥
char ssid[] = "ASUS";//这里填写你的wifi名字
char pswd[] = "887385206";//这里填写你的WiFi密码
//空调开
uint16_t power_on[199] = {4334, 4348,  536, 1620,  534, 552,  562, 1592,  564, 1592,  534, 554,  534, 554,  566, 1590,  534, 554,  534, 554,  536, 1618,  534, 556,  534, 554,  566, 1586,  534, 1622,  532, 556,  534, 1620,  536, 1618,  564, 526,  536, 552,  534, 1622,  534, 1620,  534, 1620,  534, 1620,  562, 1590,  562, 526,  534, 1618,  566, 1590,  562, 526,  534, 554,  534, 556,  534, 556,  534, 554,  534, 1620,  564, 1590,  566, 524,  562, 1590,  534, 556,  562, 526,  534, 556,  532, 556,  534, 556,  534, 554,  536, 1618,  534, 554,  564, 1590,  536, 1618,  536, 1618,  536, 1618,  534, 5174,  4360, 4324,  532, 1620,  590, 498,  594, 1560,  536, 1620,  534, 554,  562, 526,  536, 1618,  536, 554,  536, 554,  562, 1592,  534, 554,  534, 556,  562, 1592,  564, 1590,  536, 554,  534, 1620,  534, 1620,  536, 554,  564, 524,  536, 1620,  534, 1618,  536, 1620,  532, 1620,  562, 1592,  536, 554,  534, 1620,  564, 1590,  536, 556,  534, 554,  534, 554,  534, 554,  564, 526,  534, 1618,  562, 1592,  564, 524,  536, 1618,  534, 554,  536, 554,  534, 554,  536, 554,  560, 528,  564, 526,  536, 1618,  536, 554,  562, 1592,  534, 1620,  536, 1618,  534, 1620,  534};  // UNKNOWN 1C03CDD8
//空调关
uint16_t power_off[199] = {4332, 4374,  534, 1620,  586, 502,  536, 1592,  562, 1620,  588, 500,  588, 502,  586, 1542,  610, 504,  534, 556,  534, 1620,  536, 554,  532, 558,  534, 1620,  588, 1566,  588, 502,  590, 1564,  534, 556,  518, 1636,  588, 1566,  566, 1588,  586, 1566,  588, 504,  532, 1620,  588, 1566,  588, 1566,  588, 502,  588, 502,  586, 502,  586, 504,  534, 1620,  586, 502,  534, 554,  584, 1570,  532, 1596,  614, 1566,  588, 502,  532, 554,  588, 502,  588, 500,  588, 502,  532, 556,  586, 504,  588, 502,  586, 1568,  586, 1566,  534, 1620,  588, 1540,  560, 1620,  586, 5122,  4386, 4296,  534, 1622,  586, 504,  534, 1618,  588, 1566,  534, 530,  612, 504,  534, 1620,  588, 502,  588, 500,  588, 1566,  588, 502,  588, 502,  588, 1566,  534, 1622,  584, 504,  534, 1594,  560, 554,  586, 1568,  588, 1568,  586, 1566,  534, 1620,  586, 502,  534, 1620,  588, 1568,  534, 1620,  588, 500,  616, 474,  532, 558,  588, 498,  588, 1566,  588, 504,  588, 500,  586, 1568,  586, 1568,  588, 1568,  586, 502,  586, 502,  532, 556,  588, 502,  586, 500,  588, 502,  584, 478,  612, 502,  586, 1566,  588, 1542,  558, 1620,  534, 1620,  588, 1566,  564};  // COOLIX B27BE0

//定义红外发射的管脚
const uint16_t kIrLed = 14;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.
bool airPower = false;
/*美的空调红外键值读取*/

// 新建Blinker组件对象
BlinkerButton Button1("btn-pwron");
BlinkerButton Button2("btn-pwroff");

void setup()
{
  //初始化串口
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
 //配网步骤：WIFI接入
  Blinker.begin(auth,ssid,pswd); 
  Button1.attach(Button1_callback);
  Button2.attach(Button2_callback);
  BlinkerMIOT.attachPowerState(miotPowerState);   //语音指令与控制函数对应（开关指令）
  BlinkerMIOT.attachQuery(miotQuery);    //语音指令与控制函数对应(查询指令)
  //红外初始化
  irsend.begin();

}
void loop()
{
  Blinker.run(); 
}

//回调函数
void Button1_callback(const String &state)
{

  BLINKER_LOG("get button state: ", state);
  //检测到普通按钮按下（btn-pwron）
  if (state == BLINKER_CMD_BUTTON_TAP)
  {
    //发送红外指令
    Serial.println("打开空调");
    irsend.sendRaw(power_on, 199, 38);  // Send a raw data capture at 38kHz.
    Button1.print("2楼空调开");
    airPower = true;
  }

}
//回调函数
void Button2_callback(const String &state)
{
  BLINKER_LOG("get button state: ", state);
  //检测到btn-pwroff按钮按下
  if (state == BLINKER_CMD_BUTTON_TAP)
  {
    Serial.println("关闭空调");
    irsend.sendRaw(power_off, 199, 38);  // Send a raw data capture at 38kHz.
    Button2.print("2楼空调关");
    airPower = false;
  }
}

void miotPowerState(const String & state)    //小爱语音开关控制进入函数
{ 
    if (state == BLINKER_CMD_ON){     //判断语音内容 为打开
      Button1.print("on");          //点灯科技软件对应按键被打开
      BLINKER_LOG("空调打开");   
      BlinkerMIOT.powerState("on");   //反馈小爱同学按键状态已打开
      BlinkerMIOT.print();            //发送反馈
      irsend.sendRaw(power_on, 199, 38);     //发射电视开关键的红外数据（同上）
      delay(20); 
      airPower = true;                  //开关状态保存，用于小爱同学与BLINKER同步
    }
    else if (state == BLINKER_CMD_OFF){  //判断语音内容 为关闭
      Button2.print("off");            //点灯科技软件对应按键被关闭
      BLINKER_LOG("电视关闭");
      BlinkerMIOT.powerState("off"); //反馈小爱同学按键状态已关闭
      BlinkerMIOT.print();           //发送反馈
      irsend.sendRaw(power_off, 199, 38);    //发射电视开关键的红外数据（同上）
      delay(20); 
      airPower = false;                //开关状态保存，用于小爱同学与BLINKER同步
    }
}

void miotQuery(int32_t queryCode)//小爱语音查询状态函数
{
    BlinkerMIOT.powerState(airPower ? "on" : "off");  //可查询开关状态
    BlinkerMIOT.print();
}
