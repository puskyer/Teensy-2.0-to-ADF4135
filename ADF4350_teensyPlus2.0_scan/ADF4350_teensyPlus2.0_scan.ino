//
//*****************************************************************
//  borrowed code from ADF4351 PLL-Synthesizer by OE6OCG  thank you
//  and hardware ideas from  Ian Wraith - 24DownConvert  
//
// IDE: Arduino 1.8.13 or up without spec. Library
// License: 

/*     Hardware connection Teensy++ 2.0 (5V TTL Logic) to ADF4350 PLL-Board (3.3V TTL logic)
       RF output: 33Mhz - 4.4Ghz ~ 3 dbm to ADL5350 (LF to 4 GHz High Linearity Y-Mixer)
       Teensy++ 2.0 + ADF4350 + ADL5350 draw xxxmA on 5V USB
       Teensy++ 2.0 +5v @ 50 mA       
       ADF4350      +5v @ 50 mA
       ADL5350      +5v @ 50 mA



Arduino                  +---+               Arduino                                                  
pins       +-------------|USB|-------------+ Pins                                        ________________
        GND|[ ]GND       +---+       Vcc[ ]|+5v                                         |                |
PWN      27|[ ]PB7    5V TTL logic   PB6[ ]|26  PWN ---(Diode)                          |                |   | RFIN 
PWN INT0  0|[ ]PD0-SCL               PB5[ ]|25  PWN                                _____|______     _____|___|(Antenna)
PWN INT1  1|[ ]PD1-SDA               PB4[ ]|24  PWN                               |            |   |      LOIN   |
 RX INT2  2|[ ]PD2              MISO-PB3[ ]|23                                    |    +5V  [ ]|   |             |
 TX INT3  3|[ ]PD3              MOSI-PB2[ ]|22  ---(_1.5k_)-------.-------Data----| center pin |   |      GND [ ]|
          4|[ ]PD4     [ ] [ ]  SCLK-PB1[ ]|21  ---(_1.5k_)-------|--.----Clock---|            |   |      +5V [ ]|
          5|[ ]PD5      P   P     SS-PB0[ ]|20  ---(_1.5k_)-------|--|--.--LE-----| 3.3V TTL   |   |             |
onboardLED6|[ ]PD6      E   E        PE7[ ]|19  INT7  |-(_3k_)----!  |  |         |    Logic   |   |             |
          7|[ ]PD7      5   4        PE6[ ]|18  INT6  |-(_3k_)-------!  |         |   ADF4350  |   |   ADL5350   |
          8|[ ]PE0                   GND[ ]|GND ------.-(_3k_)----------!         |  PLL Board |   |             |
          9|[ ]PE1                  AREF[ ]|AREF                |--------LD-------|____________|   |_____________| 
         10|[ ]PC0                   PF0[ ]|38  A0              |                                        |RFOUT 
         11|[ ]PC1     PA4[ ] [ ]PA0 PF1[ ]|39  A1              !-(_47k_)--(Vcc +3.3v)                   |(TO SDR) 
         12|[ ]PC2     PA5[ ] [ ]PA1 PF2[ ]|40  A2              |                                        |
         13|[ ]PC3     PA6[ ] [ ]PA2 PF3[ ]|41  A3              |      (_47k_)--(Vcc +5v)
PWN      14|[ ]PC4     PA7[ ] [ ]PA3 PF4[ ]|42  A4              |      |
PWN      15|[ ]PC5                   PF5[ ]|43  A5              !-->|-----(PB6/Pin 26)
PWN      16|[ ]PC6    5V  GND RST    PF6[ ]|44  A6              (Diode)|
         17|[ ]PC7    [ ] [ ] [ ]    PF7[ ]|45  A7                     (_1.2k_)--(GND)
           |_______________________________|                     This is requried because ADF4350 is a ttl 3.3 and 
                                                                 the Teensy++ 2.0 is TTL 5v
ADF4350
LD - Lock Detect Output Pin. This pin outputs a logic high to indicate PLL lock. A logic low output indicates loss of PLL lock.
Clk - Serial Clock Input. Data is clocked into the 32-bit shift register on the CLK rising edge. This input is a high
impedance CMOS input.
Data - Serial Data Input. The serial data is loaded MSB first with the three LSBs as the control bits. This input is a high
impedance CMOS input.
LE - Load Enable, CMOS Input. When LE goes high, the data stored in the shift register is loaded into the register
that is selected by the three LSBs.
CE - Chip Enable. A logic low on this pin powers down the device and puts the charge pump into three-state mode.
A logic high on this pin powers up the device depending on the status of the power-down bits.

Description:
The ADL5350 is a high linearity, up-and-down converting mixer capable of operating over a broad input frequency range. It is well suited for demanding cellular base station mixer designs that require high sensitivity and effective blocker immunity. Based on a GaAs pHEMT, single-ended mixer architecture, the ADL5350 provides excellent input linearity and low noise figure without the need for a high power level LO drive.

In 850 MHz/900 MHz receive applications, the ADL5350 provides a typical conversion loss of only 6.7 dB. The input IP3 is typically greater than 25 dBm, with an input compression point of 19 dBm. The integrated LO amplifier allows a low LO drive level, typically only 4 dBm for most applications.

The high input linearity of the ADL5350 makes the device an excellent mixer for communications systems that require high blocker immunity, such as GSM 850 MHz/900 MHz and 800 MHz CDMA2000. At 2 GHz, a slightly greater supply current is required to obtain similar performance.

The single-ended broadband RF/IF port allows the device to be customized for a desired band of operation using simple external filter networks. The LO-to-RF isolation is based on the LO rejection of the RF port filter network. Greater isolation can be achieved by using higher order filter networks, as described in the Applications Information section.

Features:
- Power Supply Voltage: +5V DC
- Broadband radio frequency (RF), intermediate frequency (IF), and local oscillator (LO) ports
- Conversion Loss: 6.8 dB
- Noise Figure: 6.5 dB
- High Input IP3: 25 dBm
- High Input P1dB: 19 dBm
- Low LO drive level
- Single-ended Design: no balance-unbalanced converter needed

Timeing Charactoristics
AVdd = DVdd = Vvco = SDVdd =Vp = 3.3V +/- 10%
AGND = DGND = 0V
1.8V and 3V Logic Levels used
Ta = T min to T max

// ADF4350 - LE setup time min 20ns
// ADF4350 - DATA to CLK setup time min 10ns
// ADF4350 - DATA to CLK hold time min 10ns
// ADF4350 - CLK high duration min 25ns
// ADF4350 - CLK low duration  min 25ns
// ADF4350 - CLK to LE setup time min 10ns
// ADF4350 - LE Pulse width is min 20ns

 code from main.c from  IanWraithv/24DownConvert.

  // Below are the ADF4350 settings for a 1 GHz +5dBm output
  uint32_t ar0=0x500000;
  uint32_t ar1=0x8008011;
  uint32_t ar2=0x4e42;
  uint32_t ar3=0x4b3;
  uint32_t ar4=0xac803c;
  uint32_t ar5=0x580005;
  
  // Below are the ADF4350 settings for a 1001.25 MHz +5dBm output
  //uint32_t ar0=0x500008;
  //uint32_t ar1=0x8008029;
  //uint32_t ar2=0x4e42;
  //uint32_t ar3=0x4b3;
  //uint32_t ar4=0xac803c;
  //uint32_t ar5=0x580005;

  // Send the setup data to the ADF4350
  SendSPIDataADF4350(ar5);
  SendSPIDataADF4350(ar4);
  SendSPIDataADF4350(ar3);
  SendSPIDataADF4350(ar2);
  SendSPIDataADF4350(ar1);

  SendSPIDataADF4350(ar0);


Clock to zero when idle
First clock transition is the data edge
MSB sent first
fpCLK/256

This means you have to pay special attention to the device's datasheet when writing your 
code.
Generally speaking, there are four modes of transmission. 
These modes control whether data is shifted in and out on the rising or 
falling edge of the data clock signal (called the clock phase), 
and whether the clock is idle when high or low (called the clock polarity). 
The four modes combine polarity and phase according to this table: 



Mode        Clock Polarity (CPOL)    Clock Phase (CPHA)   Output Edge     Data Capture
SPI_MODE0   0                        0                    Falling         Rising 
SPI_MODE1   0                        1                    Rising          Falling 
SPI_MODE2   1                        0                    Rising          Falling 
SPI_MODE3   1                        1                    Falling         Rising


*/
//*****************************************************************
//*****************************************************************

#include <Wire.h>
#include <SPI.h>

// Teensy++ 2.0 SPI to ADF4350
const int ADF4350_LEPin = 20;         //Teensy++ 2.0 SS pin  PB0-  enable ADF4350
const int ADF4350_DataPin = 22;        //Teensy++ 2.0 MOSI pin PB2
const int ADF4350_CLKPin = 21;         //Teensy++ 2.0 SCLK pin PB1
const int ADF4350_LDPin = 26;          //Teensy++ 2.0 PWN(OC1B) pin PB6
const int ledPin = 6;                  // Teensy++ 2.0 has the LED on pin 6

long Freq = 44220000;  //Startfrequenz generel 100Hz aulösung
long refin = 2500000; // Refrenquarz = 25Mhz
long ChanStep = 625; //Kanalraster = 6,25Khz kleinstes raster
long channel[13]; // Kanal-speicher
long Step[5]; // Kanalraster-speicher
int CHnum = 0; // eingestellter Kanal
int StepNum = 0; // eingestelltes Kanalraster

// 
// Below are the ADF4350 settings for a 1 GHz +5dBm output from IanWraithv/24DownConvert.
//ADF4350 Reg's


// Below are the ADF4350 settings for +2dBm and +5dBm output
// R4 =  0x00AC8034         //+2dBm output
// R4 =  0x00ac803c        //+5dBm output
// Below are the ADF4350 settings for a 1,000 GHz
// R0 =   0x00500000
// R1 =   0x08008011
// the above gets me 999.9562MHz
// Below are the ADF4350 settings for a 1,000.1 GHz
// R0 = 0x00500010
// R1 = 0x080083E9
// the above gets me 1.000053GHz
// Below are the ADF4350 settings for a 1,000 GHz
// R0 = 0x00500000
// R1 = 0x08008011        // 22Mhz channel spacing
// the above gets me 1.0000GHz
// Below are the ADF4350 settings for a 1,000 GHz
// 5Mhz channel spacing
// +2 db RF power output
// 20 Mhz Referance Freq
// R0 = 0x00640000
// R1 = 0x08008011        
// R4 = 0xAA0034          
// the above gets me 1.0000GHz


unsigned long Reg[6] {     
  0x00500000,
  0x08008011,
  0x00004E42,
  0x000004B3,
  0x00ac803c,        
  0x00580005
};

#define LE_DELAY 1
#define Data_write_DELAY 2500
#define  LD_Wait_Delay 20

byte tenHz, hundredHz, ones, tens, hundreds, thousands, tenthousands, hundredthousands, millions;
int pad_key     = 0;
int adc_key_in_4  = 0;
int adc_key_in_5  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define pad_led_24  24
#define pad_led_25  25 

void setup() {

  Serial.begin(115200);// USB to PC for Debug only
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(pad_led_24, OUTPUT);
  pinMode(pad_led_25, OUTPUT);
  digitalWrite(pad_led_25, HIGH);   // set the LED on  
  
  pinMode(ADF4350_LDPin, INPUT); 
  pinMode (ADF4350_LEPin, OUTPUT);
  pinMode(ADF4350_CLKPin, OUTPUT);
  //digitalWrite(ADF4350_CLKPin, LOW);  // disable CLK
  pinMode(ADF4350_DataPin, OUTPUT);
  //digitalWrite(ADF4350_DataPin, LOW);  // Clear data line
  reset_ADF4350();
  SPI.begin();
  delay(500);

  Step[0] = 625; // 6,25 khz, 5khz geht nicht im Int-N Mode(MOD > 4095) bei 25Mhz Ref.
  Step[1] = 1000; // 10 khz
  Step[2] = 1250; // 12.5 khz
  Step[3] = 2500; // 25 khz
  Step[4] = 100000; // 1 Mhz Step

  channel[0] = 3400000; // Band select FIXFREQUENZEN
  channel[1] = 5150000; //51.5 Mhz
  channel[2] = 14455000;
  channel[3] = 14500000;
  channel[4] = 14560000;
  channel[5] = 43000000;
  channel[6] = 43127500;
  channel[7] = 43887500;
  channel[8] = 124500000;
  channel[9] = 129820000;
  channel[10] = 234500000;
  channel[11] = 300000000;
  channel[12] = 440000000; // 4.4Ghz
 
}

void loop() {

     blinkLed(2000);
     // before starting to program the chip lets wait for it to stabalize after a poweron
     SetADFFreq(Freq);
     reset_ADF4350();
     int count = 0;
     while (digitalRead(ADF4350_LDPin) == 0) {
          Serial.println("Waitng for LD");
          blinkLed(1000);
          count = count + 1;
          Serial.println(count);
          if (count == LD_Wait_Delay) {  // we waited to x seconds lets try setting up the ADF4350 again
            blinkLed(500);
            count = 0;
            reset_ADF4350();           
            SetADFFreq(Freq);
            delayMicroseconds(500);               // once the new peramiters are in, restart the ADF4350
            reset_ADF4350();
          }
     }
     while (digitalRead(ADF4350_LDPin) == 1) {
          Serial.println("We have LD");
          blinkLed(2000);
          pad_key = read_PAD_buttons();
            switch (pad_key)
            {
              case btnRIGHT:
                { 
                  long FreqHi;
                  Freq = channel[CHnum];
                  if (ChanStep == 100000)
                  {
                    FreqHi = Freq + 10000000; // +100Mhz
                  }
                  else
                  { FreqHi = Freq + 500000; // +5Mhz};
                  };
                  while (Freq < FreqHi) {
                    Freq += ChanStep;
                    Serial.print(Freq);
                    SetADFFreq(Freq);
                  }
          
                  if (ChanStep == 100000)
                  {
                    FreqHi = FreqHi - 10000000; // -100Mhz
                  }
                  else
                  { FreqHi = FreqHi - 500000; // -5Mhz};
                  };
          
                  while (FreqHi < Freq) {
                    Freq -= ChanStep;
                    Serial.print(Freq);                    
                    SetADFFreq(Freq);
                  }
                  break;
                }
              case btnLEFT:
                {
                  StepNum += 1;
                  if (StepNum > 4) StepNum = 0;
                  ChanStep = Step[StepNum];
                  Serial.print(Freq);                  
                  SetADFFreq(Freq);
                  delay(200);
                  break;
                }
              case btnUP:
                {
                  Freq += ChanStep;
                  Serial.print(Freq);
                  SetADFFreq(Freq);
                  break;
                }
              case btnDOWN:
                {
                  Freq -= ChanStep;
                  Serial.print(Freq);
                  SetADFFreq(Freq);
                  break;
                }
              case btnSELECT:
                {
                  CHnum += 1;
                  if (CHnum > 12) CHnum = 0;
                  Freq = channel[CHnum];
                  Serial.print(Freq);
                  SetADFFreq(Freq);
                  delay(200);
                  break;
                }
              case btnNONE:
                {
                  display_ADC(adc_key_in_4, adc_key_in_5);
                  break;
                }
            }
            adc_key_in_4 = analogRead(A0);
            adc_key_in_5 = analogRead(A1); 
            delay(20);
            display_ADC(adc_key_in_4, adc_key_in_5);
          while (adc_key_in_4 > 900 and adc_key_in_5 > 900) {
              adc_key_in_4 = analogRead(A0);
              adc_key_in_5 = analogRead(A1); 
              delay(20);
            }          
     }     
}

void reset_ADF4350()
{
  digitalWrite(ADF4350_LEPin, HIGH);  // disable ADF4350
  delayMicroseconds(500);             // wait for things to stabalize
  digitalWrite(ADF4350_LEPin, LOW);   // enable ADF4350
  delayMicroseconds(500);            // wait for things to stabalize  
}

int read_PAD_buttons()
{
  adc_key_in_4 = analogRead(A0);
  adc_key_in_5 = analogRead(A1);  
  if (adc_key_in_4 > 900) return btnNONE;
  if (adc_key_in_5 > 900) return btnNONE;  
  if (adc_key_in_5 < 10)   return btnRIGHT;
  if (adc_key_in_4 > 130 and adc_key_in_4 < 160)  return btnUP;
  if (adc_key_in_5 > 280 and adc_key_in_5 < 400)  return btnDOWN;
  if (adc_key_in_4 < 10)  return btnLEFT;
  if (adc_key_in_5 > 130 and adc_key_in_5 < 160)  return btnSELECT;
  display_ADC(adc_key_in_4, adc_key_in_5);  
  return btnNONE;
}

void display_ADC(int parm0, int parm1) {
  Serial.print ("ADC A0=");
  Serial.print (parm0);
  Serial.print ("\r\n");
  Serial.print ("ADC A1=");
  Serial.print (parm1);
  Serial.print ("\r\n"); 
  
}


void blinkLed(int leddelay)
{
  digitalWrite(ledPin, HIGH);         // set the LED on
  delay(leddelay);                    // wait for a second
  digitalWrite(ledPin, LOW);          // set the LED off
  delay(leddelay/2);                  // wait for 1/2 the leddelay
}

void blinkPadLed_24(int leddelay)
{
  digitalWrite(pad_led_24, HIGH);       // set the LED on
  delay(leddelay);                      // wait for a second
  digitalWrite(pad_led_24, LOW);        // set the LED off
  delay(leddelay/2);                    // wait for 1/2 the leddelay
}

void blinkPadLed_25(int leddelay)
{
  digitalWrite(pad_led_25, HIGH);       // set the LED on
  delay(leddelay);                      // wait for a second
  digitalWrite(pad_led_25, LOW);        // set the LED off
  delay(leddelay/2);                    // wait for 1/2 the leddelay
}


void SetADFFreq(long Freq)
{
  Serial.print("Writing ADF reg[5] = ");
  Serial.println(Reg[5], HEX);
  Serial.print("Writing ADF reg[4] = ");
  Serial.println(Reg[4], HEX);
  Serial.print("Writing ADF reg[3] = ");  
  Serial.println(Reg[3], HEX);
  Serial.print("Writing ADF reg[2] = ");  
  Serial.println(Reg[2], HEX);
  Serial.print("Writing ADF reg[1] = ");  
  Serial.println(Reg[1], HEX);
  Serial.print("Writing ADF reg[0] = ");
  Serial.println(Reg[0], HEX);

  showFreq(Freq);

  ConvertFreq(Freq, Reg);
  WriteADF2(5);
  delayMicroseconds(Data_write_DELAY);
  WriteADF2(4);
  delayMicroseconds(Data_write_DELAY);
  WriteADF2(3);
  delayMicroseconds(Data_write_DELAY);
  WriteADF2(2);
  delayMicroseconds(Data_write_DELAY);
  WriteADF2(1);
  delayMicroseconds(Data_write_DELAY); 
  WriteADF2(0);
  delayMicroseconds(Data_write_DELAY);
}

void WriteADF2(int idx)
{ // make 4 byte from integer for SPI-Transfer
  byte buf[4];
  for (int i = 0; i < 4; i++){
    buf[i] = (byte)(Reg[idx] >> (i * 8));
  }
  WriteADF(buf[3], buf[2], buf[1], buf[0]);
}

int WriteADF(byte a1, byte a2, byte a3, byte a4) {
  // write over SPI to ADF4350
  blinkLed(250);
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(ADF4350_LEPin, LOW);
  // delay to for clock
  delayMicroseconds(LE_DELAY);         // wait for clock to start a cycle - ADF4350 - LE setup time min 20ns
                                      // - ADF4350 - DATA to CLK setup time min 10ns
                                      // - ADF4350 - DATA to CLK hold time min 10ns
                                      // - ADF4350 - CLK high duration min 25ns
                                      // - ADF4350 - CLK low duration  min 25ns
                                      // - ADF4350 - LE Pulse width min 20ns
  SPI.transfer(a1);
  SPI.transfer(a2);
  SPI.transfer(a3);
  SPI.transfer(a4);
  delayMicroseconds(LE_DELAY);               // wait for clock to finish a cycle  - ADF4350 - CLK to LE setup time min 10ns
  Toggle();
  SPI.endTransaction();
}
int Toggle() {

  digitalWrite(ADF4350_LEPin, HIGH);
  delayMicroseconds(LE_DELAY);               // wait for clock to finish a cycle - ADF4350 - LE Pulse width is min 20ns
  digitalWrite(ADF4350_LEPin, LOW);
}


void ConvertFreq(long freq, unsigned long R[])
{
  // PLL-Reg-R0         =  32bit
  // Registerselect        3bit
  // int F_Frac = 4;       // 12bit
  // int N_Int = 92;       // 16bit
  // reserved           // 1bit

  // PLL-Reg-R1         =  32bit
  // Registerselect        3bit
  //int M_Mod = 5;        // 12bit
  int P_Phase = 1;     // 12bit bei 2x12bit hintereinander pow()-bug !!
  int Prescal = 0;     // 1bit geht nicht ???
  int PhaseAdj = 0;    // 1bit geht auch nicht ???
  // reserved           // 3bit

  // PLL-Reg-R2         =  32bit
  // Registerselect        3bit
  int U1_CountRes = 0; // 1bit
  int U2_Cp3state = 0; // 1bit
  int U3_PwrDown = 0;  // 1bit
  int U4_PDpola = 1;    // 1bit
  int U5_LPD = 0;       // 1bit
  int U6_LPF = 1;       // 1bit 1=Integer, 0=Frac not spported yet
  int CP_ChgPump = 7;     // 4bit
  int D1_DoublBuf = 0; // 1bit
  //  int R_Counter = 1;   // 10bit
  //  int RD1_Rdiv2 = 0;    // 1bit
  //  int RD2refdoubl = 0; // 1bit
  int M_Muxout = 0;     // 3bit
  int LoNoisSpur = 0;      // 2bit
  // reserved           // 1bit

  // PLL-Reg-R3         =  32bit
  // Registerselect        3bit
  int D_Clk_div = 150; // 12bit
  int C_Clk_mode = 0;   // 2bit
  //  reserved          // 1bit
  int F1_Csr = 0;       // 1bit
  //  reserved          // 2bit
  int F2_ChgChan = 0;   // 1bit
  int F3_ADB = 0;       // 1bit
  int F4_BandSel = 0;  // 1bit
  //  reserved          // 8bit

  // PLL-Reg-R4         =  32bit
  // Registerselect        3bit
  int D_out_PWR = 0 ;    // 2bit
  int D_RF_ena = 1;     // 1bit
  int D_auxOutPwr = 0;  // 2bit
  int D_auxOutEna = 0;  // 1bit
  int D_auxOutSel = 0;  // 1bit
  int D_MTLD = 0;       // 1bit
  int D_VcoPwrDown = 0; // 1bit 1=VCO off

  //  int B_BandSelClk = 200; // 8bit

  int D_RfDivSel = 3;    // 3bit 3=70cm 4=2m
  int D_FeedBck = 1;     // 1bit
  // reserved           // 8bit

  // PLL-Reg-R5         =  32bit
  // Registerselect     // 3bit
  // reserved           // 16bit
  // reserved     11    // 2bit
  // reserved           // 1bit
  int D_LdPinMod = 1;    // 2bit muss 1 sein
  // reserved           // 8bit

  // Referenz Freg Calc
  //  long refin = 250000; // Refrenquarz = 25000000hz
  int R_Counter = 1;   // 10bit
  int RD1_Rdiv2 = 0;    // 1bit
  int RD2refdoubl = 0; // 1bit
  int B_BandSelClk = 200; // 8bit
  //  int F4_BandSel = 0;  // 1bit

  // int F4_BandSel = 10.0 * B_BandSelClk / PFDFreq;

  long RFout = Freq;   // VCO-Frequenz
  // calc bandselect und RF-div
  int outdiv = 1;

  if (RFout >= 220000000) {
    outdiv = 1;
    D_RfDivSel = 0;
  }
  if (RFout < 220000000) {
    outdiv = 2;
    D_RfDivSel = 1;
  }
  if (RFout < 110000000) {
    outdiv = 4;
    D_RfDivSel = 2;
  }
  if (RFout < 55000000) {
    outdiv = 8;
    D_RfDivSel = 3;
  }
  if (RFout < 27500000) {
    outdiv = 16;
    D_RfDivSel = 4;
  }
  if (RFout < 13800000) {
    outdiv = 32;
    D_RfDivSel = 5;
  }
  if (RFout < 6900000) {
    outdiv = 64;
    D_RfDivSel = 6;
  }

  float PFDFreq = refin * ((1.0 + RD2refdoubl) / (R_Counter * (1.0 + RD1_Rdiv2))); //Referenzfrequenz
  float N = ((RFout) * outdiv) / PFDFreq;
  int N_Int = N;
  long M_Mod = PFDFreq * (100000 / ChanStep) / 100000;
  int F_Frac = round((N - N_Int) * M_Mod);

  R[0] = (unsigned long)(0 + F_Frac * pow(2, 3) + N_Int * pow(2, 15));
  R[1] = (unsigned long)(1 + M_Mod * pow(2, 3) + P_Phase * pow(2, 15) + Prescal * pow(2, 27) + PhaseAdj * pow(2, 28));
  //  R[1] = (R[1])+1; // Registerselect adjust ?? because unpossible 2x12bit in pow() funktion
  R[2] = (unsigned long)(2 + U1_CountRes * pow(2, 3) + U2_Cp3state * pow(2, 4) + U3_PwrDown * pow(2, 5) + U4_PDpola * pow(2, 6) + U5_LPD * pow(2, 7) + U6_LPF * pow(2, 8) + CP_ChgPump * pow(2, 9) + D1_DoublBuf * pow(2, 13) + R_Counter * pow(2, 14) + RD1_Rdiv2 * pow(2, 24) + RD2refdoubl * pow(2, 25) + M_Muxout * pow(2, 26) + LoNoisSpur * pow(2, 29));
  R[3] = (unsigned long)(3 + D_Clk_div * pow(2, 3) + C_Clk_mode * pow(2, 15) + 0 * pow(2, 17) + F1_Csr * pow(2, 18) + 0 * pow(2, 19) + F2_ChgChan * pow(2, 21) +  F3_ADB * pow(2, 22) + F4_BandSel * pow(2, 23) + 0 * pow(2, 24));
  R[4] = (unsigned long)(4 + D_out_PWR * pow(2, 3) + D_RF_ena * pow(2, 5) + D_auxOutPwr * pow(2, 6) + D_auxOutEna * pow(2, 8) + D_auxOutSel * pow(2, 9) + D_MTLD * pow(2, 10) + D_VcoPwrDown * pow(2, 11) + B_BandSelClk * pow(2, 12) + D_RfDivSel * pow(2, 20) + D_FeedBck * pow(2, 23));
  R[5] = (unsigned long)(5 + 0 * pow(2, 3) + 3 * pow(2, 19) + 0 * pow(2, 21) + D_LdPinMod * pow(2, 22));
}


void showFreq(long FREQ) {
  millions = int(FREQ / 100000000);
  hundredthousands = ((FREQ / 10000000) % 10);
  tenthousands = ((FREQ / 1000000) % 10);
  thousands = ((FREQ / 100000) % 10);
  hundreds = ((FREQ / 10000) % 10);
  tens = ((FREQ / 1000) % 10);
  ones = ((FREQ / 100) % 10);
  hundredHz = ((FREQ / 10) % 10);
  tenHz = ((FREQ) % 10);
  if (millions > 0) {
    Serial.print(millions);
    Serial.print(".");
  }
  Serial.print(hundredthousands);
  Serial.print(tenthousands);
  Serial.print(thousands);
  Serial.print(",");
  Serial.print(hundreds);
  Serial.print(tens);
  Serial.print(ones);
  Serial.print(".");
  Serial.print(hundredHz);
  Serial.print(tenHz);
};
