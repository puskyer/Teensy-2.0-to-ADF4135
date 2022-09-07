#include "ADF4351.h"

#define SET_LE()		GPIO_SetBits(GPIOD, GPIO_Pin_8)	//LE
#define CLR_LE()		GPIO_ResetBits(GPIOD, GPIO_Pin_8)

#define	SET_SCL()		GPIO_SetBits(GPIOD, GPIO_Pin_10)	//CLK
#define	CLR_SCL()		GPIO_ResetBits(GPIOD, GPIO_Pin_10)

#define SET_DATA()		GPIO_SetBits(GPIOD, GPIO_Pin_11)	//DAT
#define CLR_DATA()		GPIO_ResetBits(GPIOD, GPIO_Pin_11)

#define SET_CE()		GPIO_SetBits(GPIOD, GPIO_Pin_9)	//CE
#define CLR_CE()		GPIO_ResetBits(GPIOD, GPIO_Pin_9)
	
u8 buf[4];


//写入32个字节
//Write 32 bytes
void ADF4351_Write(u32 adf_dat)
{
	u8 i;

	CLR_LE();
	for (i = 0; i < 32; i++)
	{
		if ((adf_dat & 0x80000000) == 0x80000000)
			SET_DATA();
		else
			CLR_DATA();
		CLR_SCL();
		SET_SCL();
		CLR_SCL();
		adf_dat <<= 1;
	}
	SET_LE();
	delay(1);
	CLR_LE();
}

void delay (int length)
{
	int i;
	i = length * 200 ;
	while (i >0)
	i--;
}

void ADF4351_Initiate(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE); ;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 		
		GPIO_Init(GPIOD, &GPIO_InitStructure);   
	   SET_CE();
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_1);	//PDRF恒为0
 
}




//---------------------------------
//void WriteToADF4350(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the ADF4350 via the SPI port. 
//--------------------------------------------------------------------------------


void WriteToADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
    unsigned	char	i = 0;
	unsigned	char	j = 0;
	
	
	delay(1);
	CLR_SCL();
	CLR_LE();
	delay(1);

	for(i=count;i>0;i--)
 	{
	 	ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_DATA();	  //Send one to SDO pin
			}
			else
			{
				CLR_DATA();	  //Send zero to SDO pin
			}
			delay(1);
			SET_SCL();
			delay(1);
			ValueToWrite <<= 1;	//Rotate data
			CLR_SCL();
		}
	}
	CLR_DATA();
	delay(1);
	SET_LE();
	delay(1);
	CLR_LE();
}


//---------------------------------
//void ReadFromADF4350(unsigned char count,unsigned char *buf)
//---------------------------------
//Function that reads from the ADF4350 via the SPI port. 
//--------------------------------------------------------------------------------
void ReadFromADF4350(unsigned char count, unsigned char *buf)
{
	unsigned	char	i = 0;
	unsigned	char	j = 0;
	unsigned	int  	iTemp = 0;
	unsigned	char  	RotateData = 0;


	
	delay(1);
	CLR_SCL();
	CLR_LE();
	delay(1);

	for(j=count; j>0; j--)
	{
		for(i=0; i<8; i++)
		{
			RotateData <<= 1;		//Rotate data
			delay(1);
//			iTemp = GP4DAT;			//Read DATA of ADF4350
			SET_SCL();	
			if(0x00000020 == (iTemp & 0x00000020))
			{
				RotateData |= 1;	
			}
			delay(1);
			CLR_SCL();
		}
		*(buf + j - 1)= RotateData;
	}	 
	delay(1);
	SET_LE();
	delay(1);
	CLR_LE();
} 



void Frequency_35MHz(void)
{

	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				//write communication register 0x00580005 to control the progress 
 	buf[0] = 0x05;				//to write Register 5 to set digital lock detector
	WriteToADF4350(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0xDc;	//EC			//(DB23=1)The signal is taken from the VCO directly;(DB22-20:6H)the RF divider is 64;(DB19-12:50H)R is 80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	WriteToADF4350(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;
	WriteToADF4350(4,buf);	

	buf[3] = 0x00;
	buf[2] = 0x00;				//(DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x4E;				//(DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				//(DB12-9:7H)set Icp 2.50 mA;
	WriteToADF4350(4,buf);		//(DB23-14:1H)R counter is 1

	buf[3] = 0x08;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x29;			   //(DB26-15:6H)PHASE word is 1,neither the phase resync 
	WriteToADF4350(4,buf);	   //nor the spurious optimization functions are being used
							   //(DB27=1)prescaler value is 8/9

	buf[3] = 0x00;
	buf[2] = 0x2C;
	buf[1] = 0x80;
 	buf[0] = 0x18;				//(DB14-3:0H)FRAC value is 0;
	WriteToADF4350(4,buf);		//(DB30-15:140H)INT value is 320;

}



	/*-------------------------------------200MHz---------------------------------------------
	Reference frequency: 20MHz;Output frequency: 200MHz;VCO frequency: 3200MHz;Prescaler: 8/9;
	RF divider: 16;VCO channel spacing frequency: 200KHz;PFD frequency: 10MHz;
	INT: 320;FRAC: 0;MOD: 100;R: 1;Lock Clk Div: 6; bank clk div: 200; Phase: 1  
	----------------------------------------------------------------------------------------*/
	/*RFout = [INT + (FRAC/MOD)] * (Fpfd/RF Divider)*/
	/*Fpfd = REFIN * [(1 + D)/(R * (1 + T))]*/
	/*Fvco = RF divider * Output frequency   2.2G-4.4G*/

void ADF_SetFre(void){

	u32 adf_data;
	u16 adf_R=1;		//RF参考分频系数   RF reference divider
  u8 adf_D=0;		//RF REFin倍频器位(0 or 1)    RF REFin multiplier bit (0 or 1)
  u8 adf_T=0;		//参考二分频位,产生占空比50%,减少周跳 With reference to the two-point frequency bit, it produces a duty cycle of 50% and reduces the circumference
  u16 adf_Locktime=160;
  u16 adf_MOD=1;
  u16 adf_INT=256;
  u16 adf_FARC=0;
  u16 adf_PHASE=1;
  u8 pinduan;
	

	CLR_SCL();
	CLR_LE();
	
	
	//配置寄存器5   Configuration register 5
	adf_data = 0x00580000;	//数字锁存   LD引脚的工作方式为数字锁存   D23 D22=01 The digital latch LD pin works as the digital latch D23 D22=01
	adf_data =adf_data | ADF_R5;	
	ADF4351_Write(adf_data);
	
	
	//配置寄存器4  Configuration register 4
	adf_data = 0x00800038;
	/*(DB23=1)The signal is taken from the VCO directly,信号直接从VCO获得       The signal is obtained directly from the VCO
	可修改RF divider, R的值(DB22-20)the RF divider is 16;                      The values of RF divider, R can be modified
	(DB11=0)VCO powerd up;	辅助RF输出禁用; 频段选择时钟,分频至125k, 分频值160*      Auxiliary RF output disabled; Band Select Clock, Divided to 125k, Divided Value 160* /
	adf_data = adf_data | (RF_div32 << 20);		//RF divider is 16
	adf_data = adf_data | (160 << 12);		//频段选择时钟    Band select clock
	adf_data = adf_data | ADF_R4;	//(DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5dBm 
	ADF4351_Write(adf_data);
	
	
	//配置寄存器3  Configuration register 3
	adf_data = 0x00848000;	
	/*选择高频段（D23=1）, APB6ns(D22=0,=建议小数分频使用),,(D21=0,小数分频使用) 使能CSR(D18=1),(D16 D15=01)快速锁定
	可修改clock divider value的值*/
	/*Select the high-frequency band (D23=1), APB6ns (D22=0, =recommended for fractional-by-decimal use), (D21=0, fractional-by-decimal use) enable CSR(D18=1), (D16 D15=01) fast lock
	Modifiable value of the clock divider value */
	adf_data = adf_data | (adf_Locktime << 3);
	adf_data = adf_data | ADF_R3;	
	ADF4351_Write(adf_data);
	
	//配置寄存器2   Configuration register 2
	adf_data = 0x61002040;
	//低杂散输出, 禁用参考倍频, 二分频触发器使能(减少周跳必须)
	//使能双缓冲器, 设置电荷磅电流0.31, 小数N分频(40), 设置R分频器的值为1
	//设置鉴相器的极性, (DB6)同向滤波器1,反向滤波器0,这里用同向滤波器
	/*
	Low Spurious Output, Reference Multiplier Disabled, 2-Way Flip-Flop Enable (Circumference Hop Reduction Must)
	Enable the double buffer, set the charge pound current to 0.31, divide-by-N (40), and set the value of the R divider to 1
	Set the polarity of the phase detector, (DB6) co-direction filter 1, reverse filter 0, here use a co-direction filter
	*/
	adf_data = adf_data | (adf_D << 25);
	adf_data = adf_data | (adf_T << 24);
	adf_data = adf_data | (adf_R << 14);	
	adf_data = adf_data | ADF_R2;	
	ADF4351_Write(adf_data);
	
	//配置寄存器1    Configuration register 1
	adf_data = 0x01008000;
	//禁用相位调整,预分频器的值为8/9
    //相位字为1
    /*
	Disable phase adjustment and the value of the prescaler is 8/9
    The phase word is 1
	*/
	adf_data = adf_data | (adf_PHASE << 15);
	adf_data = adf_data | (adf_MOD << 3);
	adf_data = adf_data | ADF_R1;	
	ADF4351_Write(adf_data);
	
	//配置寄存器0     Configuration register 0
	adf_data = 0x00000000;
	adf_data = adf_data | (adf_INT << 15);
	adf_data = adf_data | (adf_FARC << 3);
	adf_data= adf_data | ADF_R0;	
	ADF4351_Write(adf_data);
	
	
	
	
	
	
}
