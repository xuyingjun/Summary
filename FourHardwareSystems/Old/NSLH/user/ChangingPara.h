//定义房间号，确定端口号和DeviceId
#define ModbusBaseAddr 0x0000
#define NSLH1L103

//-----------------------------------------------------------------------------
#if defined (NSLH1L101)
//AO
	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x65								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值  

//RTD
#define RTDchannel                  0                   //测水温

#endif


#if defined (NSLH1L102)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        5                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x66								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值   

//RTD
#define RTDchannel                  2                   //测水温

#endif

#if defined (NSLH1L103)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x67								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值  

//RTD
#define RTDchannel                  2                   //测水温

#endif

#if defined (NSLH1L104)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x68								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值  

//RTD
#define RTDchannel                  2                   //测水温

#endif

#if defined (NSLH1L105)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x69								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值 

//RTD
#define RTDchannel                  2                   //测水温

#endif

#if defined (NSLH1L106)
	//AO
	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    0                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x6A								 //通讯地址
	//AI
	#define DIFFPRE                   	3										   //压差
	#define CHWVfb                      11			                 //冷水阀反馈值   
	
	//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号

//RTD
#define RTDchannel                  2                   //测水温

#endif

//-------------------------------------------------------------------------------------------------
#if defined (NSLH2L101)
//AO
	#define PTBFan1                     4                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x65								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值  

//RTD
#define RTDchannel                  2                   //测水温

#endif


#if defined (NSLH2L102)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x66								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值  

//RTD
#define RTDchannel                  2                   //测水温

#endif

#if defined (NSLH2L103)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	//#define CHWV                        2                    //控制冷水阀开度
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x67								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                     	0			               //冷水阀反馈值  

//RTD
#define RTDchannel                  2                   //测水温

#endif

#if defined (NSLH2L104)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x68								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      2			               //冷水阀反馈值  

//RTD
#define RTDchannel                  2                   //测水温

#endif

#if defined (NSLH2L105)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x69								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值  

//RTD
#define RTDchannel                  2                   //测水温

#endif

#if defined (NSLH2L106)
//AO

	#define PTBFan1                     0                    //控制PTB风扇
	#define ERVFan	                    7                    //控制ERV风扇
	#define CHWV                        2                    //控制冷水阀开度
	#define PTBFan2                    	3                    //控制PTB风扇，大房间有6台PTB，分两个端口控制
	#define DeviceId  									0x6A								 //通讯地址
		//DO
#define NSLHAlarm                   2                     //报警信息
#define NSLHERVON										1											//开ERV的信号
//AI
#define DIFFPRE                   	3										   //压差
#define CHWVfb                      11			               //冷水阀反馈值 

//RTD
#define RTDchannel                  2                   //测水温

#endif
