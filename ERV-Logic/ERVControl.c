#define  ERVPort    0              //AO管脚 控制ERV速度
#define  DP         1              //AI管脚，获取房间正压
#define  ERV_FB     2              //ERV速度反馈值
#define  K          0.01           //FanSpeed与Pressure比例系数
float Para[3] = {0.01,0.05,15};    //PID参数，kp,ki参数和时间间隔15s,也就是时间间隔
float AI_Input[12] = {0.0};        //AI端口输入数据
float Pressure = 0.0;              //房间正压
float feedback = 0.0;              //ERV反馈值
float FanSpeed = 0.0;
float CO2Sv = 500,CO2Value = 600;

int main(void)
{
  Tim4Configure();
  ERV_OnOff();

  APortOut(ERVPort,FanSpeed);
}
//进行算法处理，定时时间是30000*36000/72000000=15s，时间间隔为15s
void Tim4Configure(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_DeInit(TIM4);                    					            //复位定时器4
	TIM_TimeBaseStructure.TIM_Period = 30000;                 //每T秒更新一次子程序
	TIM_TimeBaseStructure.TIM_Prescaler =(36000-1);                 //分频36000
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //时钟分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计数方向向上
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                //重复计数
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);              //使能TIM4中断源
	TIM_Cmd(TIM4, ENABLE);
}

//定时中断函数处理，每个15s更新一次ERV速度信息
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		FanSpeed = ERV_PID(CO2Sv);
	}
}


//ERV算法控制，主要集中三种控制方法：1.ON-OFF控制；2.PID控制算法；3.CO2变化率控制法

//ON-OFF控制：超过900ppm，ERV开；低于600ppm,ERV关；
void ERV_OnOff(void)
{
  APortIn(DP,AI_Input);     //获取房间正压
  Pressure = AI_Input[DP];
  FanSpeed = K*Pressure;
  if(CO2Value>900)
    APortOut(ERVPort,10);                     //超过900,ERV全开
  if((CO2Value>=600)&&(CO2Value<=900))        //[600,900]之间，根据房间正压调速
    APortOut(ERVPort,FanSpeed);
  if(CO2Value<600)                            //低于600，ERV关闭
    APortOut(ERVPort,0);
}

//PI控制器，CO2浓度范围是[600,900]
float ERV_PID(float SetValue)
{
  float OutValue = 0.0;
  float error = 0.0;

  APortIn(ERV_FB,AI_Input);     //获取ERV速度反馈值
  feedback = AI_Input[ERV_FB];
  if (feedback>10.0)
    feedback = 10.0;

  if（SetValue>900||CO2Value>900）
    OutValue = 10.0;
  if(SetValue<600||CO2Value<600)
    OutValue = 0.0;

  error = SetValue - CO2Value;                //co2浓度设定值与实际值之差
  OutValue = feedback - (Para[0]*error+Para[1]*error*Para[2]);  //PI控制器
  if(OutValue>10.0)
    OutValue = 10.0;
  if(OutValue<0.0)
    OutValue = 0.0;

  return OutValue;
}

//CO2变化率控制法
#define Number  30                   //采样个数
#define RoomVolume  300              //房间体积
#define K1          0.01             //比例系数
void ERV_Rate(void)
{
  float buffer[Numer];
  float delta_error[Number-1];
  float sum=0.0;
  float average=0.0;
  float vf=0.0;
  //所需流量 vf=(47.8*R*V*N)/19100; R:前后两次CO2采样数据差值；V：房间体积；N：采样频率，每秒采样个数
  for(int i=0;i<Number;i++)
  {
    buffer[i]=get_CO2();    //获取浓度值
    if(buffer[i]>900)
      FanSpeed = 10.0;
    if(buffer[i]<600)
      FanSpeed = 0.0;
  }

  for(int i=0;i<Number-1;i++)
  {
    delta_error[i]=buffer[i+1]-buffer[i];
    sum +=delta_error[i];
  }
  average = sum/(Number-1);

  vf = (47.8*average*RoomVolume*Number)/19100;

  APortIn(ERV_FB,AI_Input);     //获取ERV速度反馈值
  feedback = AI_Input[ERV_FB];
  if (feedback>10.0)
    feedback = 10.0;

  FanSpeed = feedback+K1*vf;
  if(FanSpeed>10.0)
    FanSpeed = 10.0;
  if(FanSpeed<0.0)
    FanSpeed = 0.0;
}
