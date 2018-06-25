#define  ERVPort    0              //AO�ܽ� ����ERV�ٶ�
#define  DP         1              //AI�ܽţ���ȡ������ѹ
#define  ERV_FB     2              //ERV�ٶȷ���ֵ
#define  K          0.01           //FanSpeed��Pressure����ϵ��
float Para[3] = {0.01,0.05,15};    //PID������kp,ki������ʱ����15s,Ҳ����ʱ����
float AI_Input[12] = {0.0};        //AI�˿���������
float Pressure = 0.0;              //������ѹ
float feedback = 0.0;              //ERV����ֵ
float FanSpeed = 0.0;
float CO2Sv = 500,CO2Value = 600;

int main(void)
{
  Tim4Configure();
  ERV_OnOff();

  APortOut(ERVPort,FanSpeed);
}
//�����㷨������ʱʱ����30000*36000/72000000=15s��ʱ����Ϊ15s
void Tim4Configure(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_DeInit(TIM4);                    					            //��λ��ʱ��4
	TIM_TimeBaseStructure.TIM_Period = 30000;                 //ÿT�����һ���ӳ���
	TIM_TimeBaseStructure.TIM_Prescaler =(36000-1);                 //��Ƶ36000
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //������������
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                //�ظ�����
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);              //ʹ��TIM4�ж�Դ
	TIM_Cmd(TIM4, ENABLE);
}

//��ʱ�жϺ�������ÿ��15s����һ��ERV�ٶ���Ϣ
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		FanSpeed = ERV_PID(CO2Sv);
	}
}


//ERV�㷨���ƣ���Ҫ�������ֿ��Ʒ�����1.ON-OFF���ƣ�2.PID�����㷨��3.CO2�仯�ʿ��Ʒ�

//ON-OFF���ƣ�����900ppm��ERV��������600ppm,ERV�أ�
void ERV_OnOff(void)
{
  APortIn(DP,AI_Input);     //��ȡ������ѹ
  Pressure = AI_Input[DP];
  FanSpeed = K*Pressure;
  if(CO2Value>900)
    APortOut(ERVPort,10);                     //����900,ERVȫ��
  if((CO2Value>=600)&&(CO2Value<=900))        //[600,900]֮�䣬���ݷ�����ѹ����
    APortOut(ERVPort,FanSpeed);
  if(CO2Value<600)                            //����600��ERV�ر�
    APortOut(ERVPort,0);
}

//PI��������CO2Ũ�ȷ�Χ��[600,900]
float ERV_PID(float SetValue)
{
  float OutValue = 0.0;
  float error = 0.0;

  APortIn(ERV_FB,AI_Input);     //��ȡERV�ٶȷ���ֵ
  feedback = AI_Input[ERV_FB];
  if (feedback>10.0)
    feedback = 10.0;

  if��SetValue>900||CO2Value>900��
    OutValue = 10.0;
  if(SetValue<600||CO2Value<600)
    OutValue = 0.0;

  error = SetValue - CO2Value;                //co2Ũ���趨ֵ��ʵ��ֵ֮��
  OutValue = feedback - (Para[0]*error+Para[1]*error*Para[2]);  //PI������
  if(OutValue>10.0)
    OutValue = 10.0;
  if(OutValue<0.0)
    OutValue = 0.0;

  return OutValue;
}

//CO2�仯�ʿ��Ʒ�
#define Number  30                   //��������
#define RoomVolume  300              //�������
#define K1          0.01             //����ϵ��
void ERV_Rate(void)
{
  float buffer[Numer];
  float delta_error[Number-1];
  float sum=0.0;
  float average=0.0;
  float vf=0.0;
  //�������� vf=(47.8*R*V*N)/19100; R:ǰ������CO2�������ݲ�ֵ��V�����������N������Ƶ�ʣ�ÿ���������
  for(int i=0;i<Number;i++)
  {
    buffer[i]=get_CO2();    //��ȡŨ��ֵ
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

  APortIn(ERV_FB,AI_Input);     //��ȡERV�ٶȷ���ֵ
  feedback = AI_Input[ERV_FB];
  if (feedback>10.0)
    feedback = 10.0;

  FanSpeed = feedback+K1*vf;
  if(FanSpeed>10.0)
    FanSpeed = 10.0;
  if(FanSpeed<0.0)
    FanSpeed = 0.0;
}
