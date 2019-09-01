#include "global.h"

const uint AD_Result[70] = {
   	3123,3086,3048,3009,2869,2930,2890,2848,2806,2764,
   	2720,2678,2634,2590,2545,2501,2456,2410,2365,2319,
   	2274,2229,2183,2138,2092,2048,2003,1958,1914,1870,
   	1826,1783,1741,1699,1657,1616,1574,1534,1497,1458,
   	1420,1383,1346,1310,1275,1240,1206,1173,1141,1109,
   	1081,1048,1018,990, 961,  934,907, 881, 856, 831,
   	807, 784, 761, 739,	718, 697, 676, 657, 638, 619,
};

//�жϺ����м����
uchar ABuf;
uchar StatusBuf;
uchar FSR_TEMP;

//ϵͳ�м����
uchar TimeUsCount; 	   	   	   	   	   	   	
uchar TimeMsCount;
uchar Time10MsCount;
uchar AD_Serch_Cnt;
uchar TemperatureData;
uchar DispL;
uchar DispH;
uchar ScanCount;   	   	   	   	   	   	   	//ɨ���ʱ
uchar CollingCount;							//��ȴ��ʱ
static uchar DelayCount;  	   	   	   	   	   	   	//��˸��ʱ
//ADת��
uchar Count,i;
uint ADC_Cach[11];
uint ADC_Temp;
uint ADC_Data; 

void main(void)
{
   	BoardInit();
   	
   	while (1)
   	{
   	   	//0-98��
   	   	//ADת��
   	   	if (AD_Convert_flg)
   	   	{
   	   	   	AD_Convert_flg = !AD_Convert_flg;
   	   	   	if(ADEOC == TRUE)
   	   	   	{
   	   	   	   	ADEOC = FAULT;
   	   	   	   	
   	   	   	   	ADC_Temp = ADRH;
   	   	   	   	ADC_Temp = (ADC_Temp << 4) | (ADRL & 0x0f);
   	   	   	   	ADC_Cach[Count] = ADC_Temp;
   	   	   	
   	   	   	   	Count++;   	   	
   	   	   	}
   	   	}
   	   	
   	   	if(Count > 11)
   	   	{
   	   	   	for(Count; Count > 0; Count--)
   	   	   	{
   	   	   	   	for(i = 0; i < Count; i++)
   	   	   	   	{
   	   	   	   	   	if(ADC_Cach[i] > ADC_Cach[i+1])
   	   	   	   	   	{
   	   	   	   	   	   	ADC_Temp = ADC_Cach[i];
   	   	   	   	   	   	ADC_Cach[i] = ADC_Cach[i+1];
   	   	   	   	   	   	ADC_Cach[i+1] = ADC_Temp;
   	   	   	   	   	}
   	   	   	   	}
   	   	   	}
   	   	   	ADC_Data = ADC_Cach[5];
   	   	}
   	   	
   	   	if (ADC_Data != 0)
   	   	{
   	   	   	for(AD_Serch_Cnt = 0; AD_Serch_Cnt < 70; AD_Serch_Cnt++)
   	   	   	{
   	   	   	   	if(ADC_Data > AD_Result[AD_Serch_Cnt])
   	   	   	   	{
   	   	   	   	   	ADC_Data = 0;
   	   	   	   	   	if(AD_Serch_Cnt <= 2)
   	   	   	   	   	{
   	   	   	   	   	   	TemperatureData = AD_Serch_Cnt;
   	   	   	   	   	   	break;
   	   	   	   	   	}
   	   	   	   	   	else
   	   	   	   	   	{
   	   	   	   	   	   	TemperatureData = AD_Serch_Cnt - 2;
   	   	   	   	   	   	break;
   	   	   	   	   	}  	
   	   	   	   	}
   	   	   	}
   	   	   	
   	   	   	DispH = TemperatureData / 10;
   	   	   	DispL = TemperatureData % 10;
   	   	}

   	    //��ȴ״̬��˸���� 	   	
   	   	if (sys_Colling_flg)
   	   	{
   	   	   	//500ms��תһ��
   	   	   	if (DelayCount < 50)
   	   	   	{
   	   	   	   	sys_status_natureair = TRUE;
   	   	   	}
   	   	   	else if (DelayCount >= 50)
   	   	   	{
   	   	   	   	sys_status_natureair = FAULT;
   	   	   	}
   	   	   	
   	   	   	if (DelayCount == 100)
   	   	   	{
   	   	   	   	DelayCount = 0;
				CollingCount++;
   	   	   	}
   	   	}
   	   	
   	   	if (sys_status_lithtflg == TRUE
		|| sys_status_nithtflg == TRUE
   	   	|| sys_status_airchange == TRUE 
   	   	|| sys_status_warmkey == TRUE
		|| sys_status_smartkey == TRUE
   	   	|| sys_status_natureair == TRUE)
   	   	{
   	   	   	sys_waitting_flg = FAULT;
   	   	}
   	   	else 
   	   	{
   	   	   	sys_waitting_flg = TRUE;
   	   	}  	
   	}
}

void Delay_10ms(uchar DlyCount)
{  	
   	while(DlyCount != 0)
   	{
   	   	if(sys_10ms_flag == TRUE)
   	   	{
   	   	   	sys_10ms_flag = FAULT;
   	   	   	DlyCount--;   
   	   	}
   	}
}

void  Clr_Ram(void)
{
       	__asm
ClrRam:
       	movai  	0x00
       	movra  	FSR

ClrRam_Loop:
       	clrr   	INDF
   	   	incr    FSR
   	   	movai  	0xff
       	rsubar 	FSR    	       	       	
       	movra  	_FSR_TEMP
       	incr   	_FSR_TEMP
       	djzr   	_FSR_TEMP      	       	
       	goto   	ClrRam_Loop
       	       	
    __endasm;
}

void BoardInit(void)
{
   	//���RAM
   	Clr_Ram();
   	//�������ģʽ����
   	//P00-NTC   P02-DATAIN
   	OEP0 = 0XFA;
   	OEP1 = 0XFF;
   	//�˿���������
   	IOP0 = 0X00;
   	IOP1 = 0X00;
   	//������������
   	PUP0 = 0X00;   	   	   	//P00,P01��©���
   	PDP1 = 0X00;   	   	   	//P13��©���
   	//��ͨ��������16mA
   	DRVCR = 0X80;

   	//ADC��ʼ��
   	ANSEL0 = 0X01; 	   	//ѡ��ANS0ͨ�� 	   	
   	ADCR0 =	0X09;  	   	//ʹ��12λADC
   	ADCR1 = 0XE3;  	   	//125KHzת��Ƶ�ʣ��ڲ�VDD�ο���ѹ
   	ADCR2 = 0X0F;  	   	//15��ADCʱ��
   	
   	//T1��ʼ��
   	T1LOAD = 250;
   	T1CR   = 0X8F;     	   	//HCLK,128��Ƶ,1MS

   	// T1��ʼ��
   	T0LOAD = 160;
   	T0CR   = 0X8B;     	   	//HCLK,8��Ƶ,40uS
   	
   	// �ⲿ�ж����ã����óɵ�ƽ���䴥��
   	// MCR |= 0X02;
   	
   	//ʹ���ж�
   	INTE  = 0X03;  	   	   	//ʹ��T1,T0
   	INTF = 0X00;
   	GIE   = TRUE;  	   	   	//�����ж�
   	
   	Seg8;
   	DIG1 = LOW;
   	DIG2 = LOW;
   	DIG3 = LOW;
   	
   	sys_start_flg = TRUE;
   	Delay_10ms(100);
   	sys_start_flg = FAULT;
   	sys_waitting_flg = TRUE;   	//��ʼ������״̬
}

void DataRcv(void)
{
   	static uchar   	Status;
   	static uint    	RcvTimeCount;
   	static uchar   	DataBitCount;  	   	   	   	   	//���յ�����λ����
   	static uchar   	RcvTemp;       	   	   	   	   	//���ջ���
   	static uint    	RcvData1;  	   	   	   	   	   	//���յ��ĵ�һ������
   	static uint    	RcvData2;  	   	   	   	   	   	//���յ��ĵڶ�������
   	uchar CmdDataHigh;
   	uchar CmdDataLow;
   	
   	switch (Status)
   	{
   	   	
   	   	case 0:
   	   	// 9.44ms�ߵ�ƽ  236
   	   	if (DATAIN == HIGH)
   	   	{
   	   	   	RcvTimeCount++; 
   	   	   	if (RcvTimeCount > 310)
   	   	   	{
   	   	   	   	RcvTimeCount = 0;
   	   	   	}
   	   	}
   	   	
   	   	if (DATAIN == LOW)
   	   	{
   	   	   	if (RcvTimeCount > 45 && RcvTimeCount < 300)
   	   	   	{
   	   	   	   	RcvTimeCount = 1;
   	   	   	   	Status++;
   	   	   	}
   	   	   	else
   	   	   	{
   	   	   	   	RcvTimeCount = 0;
   	   	   	}
   	   	}
   	   	
   	   	break;
   	   	
   	   	case 1:
   	   	
   	   	if (DATAIN == LOW)
   	   	{
   	   	   	RcvTimeCount++;
   	   	   	if (RcvTimeCount > 200)
   	   	   	{
   	   	   	   	RcvTimeCount = 0;
   	   	   	   	Status = 0;
   	   	   	}
   	   	}
   	   	
   	   	// 4.8ms�͵�ƽ  120
   	   	if (DATAIN == HIGH)
   	   	{
   	   	   	if (RcvTimeCount > 100 && RcvTimeCount < 190)
   	   	   	{
   	   	   	   	Status++;
   	   	   	   	RcvTimeCount = 1;
   	   	   	}
   	   	   	else 
   	   	   	{
   	   	   	   	RcvTimeCount = 0;
   	   	   	   	Status = 0;
   	   	   	}
   	   	}
   	   	
   	   	break;
   	   	
   	   	case 2:
   	   	
   	   	if (DATAIN == HIGH)
   	   	{
   	   	   	RcvTimeCount++;
   	   	   	if (RcvTimeCount > 50)
   	   	   	{
   	   	   	   	RcvTimeCount = 0;
   	   	   	   	Status = 0;
   	   	   	}
   	   	}
   	   	
   	   	// 640us�ߵ�ƽ  16
   	   	if (DATAIN == LOW)
   	   	{
   	   	   	if (RcvTimeCount > 10 && RcvTimeCount < 50) 
   	   	   	{
   	   	   	   	//��33λ�͵�ƽʱ�жϣ���ʱ����32
   	   	   	   	if (DataBitCount == 32)
   	   	   	   	{
   	   	   	   	   	RcvData2 = RcvData2 << 8;
   	   	   	   	   	RcvData2 &= 0XFF00;
   	   	   	   	   	RcvData2 |= RcvTemp;
   	   	   	   	   	
   	   	   	   	   	DataBitCount = 0;
   	   	   	   	   	RcvTimeCount = 0;
   	   	   	   	   	Status = 0;	
   	   	   	   	   	CmdDataHigh = RcvData2 >> 8;
   	   	   	   	   	CmdDataLow  = RcvData2;
   	   	   	   	   	
   	   	   	   	   	if (FramHead == RcvData1)
   	   	   	   	   	{  	   	   
   	   	   	   	   	   	switch (CmdDataHigh)
   	   	   	   	   	   	{
   	   	   	   	   	   	   	case ZMKEY:
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	if (CmdDataLow == ZMON)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_lithtflg = TRUE;
   	   	   	   	   	   	   	   	sys_status_nithtflg = FAULT;
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	else if (CmdDataLow == XYDON)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_nithtflg = TRUE;
   	   	   	   	   	   	   	   	sys_status_lithtflg = FAULT;
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	else if (CmdDataLow == SYSOFF)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_lithtflg = FAULT;
   	   	   	   	   	   	   	   	sys_status_nithtflg = FAULT;
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	break;
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	case HQKEY:
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	if (CmdDataLow == SHZON)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_airchange = TRUE;
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	else if (CmdDataLow == SYSOFF)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_airchange = FAULT;
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	break;
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	case SMKEY:
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	if (CmdDataLow == SHZON)
   	   	   	   	   	   	   	{
								if (!sys_Colling_flg)
								{
									if (sys_status_warmkey)
									{
										sys_status_warmkey = FAULT;
										sys_Colling_flg = TRUE;
										CollingCount = 0;
									}
									sys_status_smartkey = TRUE;
								} 	
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	else if (CmdDataLow == SYSOFF)
   	   	   	   	   	   	   	{
								
								if (CollingCount >= 14)
								{
									sys_status_smartkey = FAULT;
									sys_status_airchange = FAULT;
									sys_status_warmkey = FAULT;
									sys_status_nithtflg = FAULT;
									sys_status_lithtflg = FAULT;
									sys_Colling_flg = FAULT;
								}
								
								if (sys_status_warmkey)
								{
									sys_status_natureair = TRUE;
								}
								else 
								{
									sys_status_natureair = FAULT;
								}
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	break;
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	case QNKEY:
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	if (CmdDataLow == SHZON)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_warmkey = TRUE;
								sys_status_natureair = TRUE;
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	else if (CmdDataLow == QNON)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_warmkey = TRUE;
								sys_status_natureair = TRUE;
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	else if (CmdDataLow == SYSOFF)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_warmkey = FAULT;
								sys_Colling_flg = TRUE;
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	break;
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	case SHZON:
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	if (CmdDataLow == SHZON)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_natureair = TRUE;
								if (sys_Colling_flg)
								{
									sys_Colling_flg = FAULT;
								}
   	   	   	   	   	   	   	}
   	   	   	   	   	   	   	else if (CmdDataLow == SYSOFF)
   	   	   	   	   	   	   	{
   	   	   	   	   	   	   	   	sys_status_natureair = FAULT;
								if (sys_Colling_flg)
								{
									sys_Colling_flg = !sys_Colling_flg;
								}
   	   	   	   	   	   	   	}
							else if (CmdDataLow == 0XF7)
							{
								sys_Colling_flg = TRUE;
								sys_status_natureair = TRUE;
							}
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	break;
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	default :
   	   	   	   	   	   	   	
   	   	   	   	   	   	   	break;
   	   	   	   	   	   	}
   	   	   	   	   	}
   	   	   	   	}
   	   	   	   	else
   	   	   	   	{
   	   	   	   	   	RcvTimeCount = 1;
   	   	   	   	   	Status++;
   	   	   	   	}
   	   	   	   	
   	   	   	   	if (DataBitCount == 8)
   	   	   	   	{
   	   	   	   	   	RcvData1 = RcvTemp;
   	   	   	   	}
   	   	   	   	else if (DataBitCount == 16)
   	   	   	   	{
   	   	   	   	   	RcvData1 = RcvData1 << 8;
   	   	   	   	   	RcvData1 &= 0XFF00;
   	   	   	   	   	RcvData1 |= RcvTemp;
   	   	   	   	}
   	   	   	   	else if (DataBitCount == 24)
   	   	   	   	{
   	   	   	   	   	RcvData2 = RcvTemp;
   	   	   	   	}
   	   	   	}  	
   	   	}
   	   	
   	   	break;
   	   	
   	   	case 3:
   	   	
   	   	if (DATAIN == LOW)
   	   	{
   	   	   	RcvTimeCount++;
   	   	   	if (RcvTimeCount > 100)
   	   	   	{
   	   	   	   	RcvTimeCount = 0;
   	   	   	   	Status = 0;
   	   	   	}
   	   	}
   	   	
   	   	if (DATAIN == HIGH)
   	   	{
   	   	   	// 1.84ms�͵�ƽ   46
   	   	   	if (RcvTimeCount > 35 && RcvTimeCount < 80)
   	   	   	{
   	   	   	   	RcvTimeCount = 1;
   	   	   	   	DataBitCount++;
   	   	   	   	Status--;
   	   	   	   	RcvTemp <<= 1;
   	   	   	   	RcvTemp &= 0xfe;
   	   	   	}
   	   	   	// 640us�͵�ƽ  16
   	   	   	else if(RcvTimeCount > 9 && RcvTimeCount < 25)
   	   	   	{
   	   	   	   	RcvTimeCount = 1;
   	   	   	   	DataBitCount++;
   	   	   	   	Status--;
   	   	   	   	RcvTemp <<= 1;
   	   	   	   	RcvTemp |= 0x01;
   	   	   	}
   	   	}
   	   	
   	   	break;
   	   	
   	   	default :
   	   	
   	   	break;
   	}
} 

void int_isr(void) __interrupt
{
   	
   	__asm
   	   	   	movra   _ABuf
   	   	   	swapar  _STATUS
   	   	   	movra   _StatusBuf
   	__endasm;
//========= T0/T1�ж� ==============  

   	if (T0IF)  	   	   	   	   	   	//40us
   	{
   	   	T0IF = FAULT;
   	   	DataRcv();
   	} 
   	
    if (T1IF)          	   	   	   	//1ms
   	{
   	   	T1IF = FAULT;
   	   	TimeMsCount++;
   	   	ScanCount++;
   	   	
   	   	if(TimeMsCount >= 10)
   	   	{
   	   	   	if (TimeMsCount == 5 || TimeMsCount == 10)
   	   	   	{
   	   	   	   	AD_Convert_flg = TRUE;
   	   	   	}
   	   	   	if (sys_Colling_flg)
   	   	   	{
   	   	   	   	DelayCount++;
   	   	   	}
   	   	   	Time10MsCount++;
   	   	   	TimeMsCount = 0;   	
   	   	   	sys_10ms_flag = TRUE;
   	   	}  	
   	   	
   	   	if ((sys_waitting_flg == FAULT || sys_Colling_flg == TRUE) 
			&& sys_start_flg == FAULT)
   	   	{
   	   	   	if (ScanCount < 5)
   	   	   	{
   	   	   	   	SegOFF();
   	   	   	   	DIG2 = HIGH;
   	   	   	   	DIG3 = HIGH;
   	   	   	   	
   	   	   	   	switch (DispH)
   	   	   	   	{
   	   	   	   	   	case 0:
   	   	   	   	   	Seg0;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 1:
   	   	   	   	   	Seg1;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 2:
   	   	   	   	   	Seg2;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 3:
   	   	   	   	   	Seg3;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 4:
   	   	   	   	   	Seg4;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 5:
   	   	   	   	   	Seg5;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 6:
   	   	   	   	   	Seg6;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 7:
   	   	   	   	   	Seg7;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 8:
   	   	   	   	   	Seg8
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 9:
   	   	   	   	   	Seg9
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	default:
   	   	   	   	   	
   	   	   	   	   	break;
   	   	   	   	}
   	   	   	   	DIG1 = LOW;
   	   	   	}
   	   	   	else if ((ScanCount > 5) && (ScanCount < 10))
   	   	   	{
   	   	   	   	SegOFF();
   	   	   	   	DIG1 = HIGH;
   	   	   	   	DIG3 = HIGH;
   	   	   	   	
   	   	   	   	switch (DispL)
   	   	   	   	{
   	   	   	   	   	case 0:
   	   	   	   	   	Seg0;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 1:
   	   	   	   	   	Seg1;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 2:
   	   	   	   	   	Seg2;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 3:
   	   	   	   	   	Seg3;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 4:
   	   	   	   	   	Seg4;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 5:
   	   	   	   	   	Seg5;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 6:
   	   	   	   	   	Seg6;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 7:
   	   	   	   	   	Seg7;
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 8:
   	   	   	   	   	Seg8
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	case 9:
   	   	   	   	   	Seg9
   	   	   	   	   	break;
   	   	   	   	   	
   	   	   	   	   	default:
   	   	   	   	   	
   	   	   	   	   	break;
   	   	   	   	}
   	   	   	   	DIG2 = LOW;
   	   	   	}
   	   	   	else if (ScanCount > 10)
   	   	   	{
   	   	   	   	if (ScanCount == 15)
   	   	   	   	{
   	   	   	   	   	ScanCount = 0;
   	   	   	   	}
   	   	   	   	
   	   	   	   	SegOFF();
   	   	   	   	DIG1 = HIGH;
   	   	   	   	DIG2 = HIGH;
   	   	   	   	SegD;
   	   	   	   	
   	   	   	   	if ((sys_status_nithtflg == TRUE) || (sys_status_lithtflg == TRUE))
   	   	   	   	{
   	   	   	   	   	SegB;
   	   	   	   	}
   	   	   	   	

   	   	   	   	if (sys_status_airchange)
   	   	   	   	{
   	   	   	   	   	SegC;
   	   	   	   	}
   	   	   	   	
   	   	   	   	if (sys_status_natureair)
   	   	   	   	{
   	   	   	   	   	SegE;
   	   	   	   	}
   	   	   	   	
   	   	   	   	if (sys_status_warmkey)
   	   	   	   	{
   	   	   	   	   	SegF;
   	   	   	   	}

   	   	   	   	DIG3 = LOW;
   	   	   	}
   	   	   	
   	   	}
   	   	else if ((sys_waitting_flg == TRUE) && (sys_Colling_flg == FAULT))
   	   	{
   	   	   	SegOFF();
   	   	   	DigOFF();
   	   	}  	
   	}

//========= �ⲿ�ж� ==============

   	__asm
   	   	   	swapar  _StatusBuf
   	   	   	movra   _STATUS
   	   	   	swapr   _ABuf
   	   	   	swapar  _ABuf
   	__endasm;
}
