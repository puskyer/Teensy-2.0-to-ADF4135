//
//*****************************************************************
// IDE: Arduino 1.8.13 or up without spec. Library
// License: 

/*     Hardware connection Teensy++ 2.0 (5V TTL Logic) to ADF4351 PLL-Board (3.3V TTL logic)
       RF output: 33Mhz - 4.4Ghz ~ 3 dbm to ADL5350 (LF to 4 GHz High Linearity Y-Mixer)
       Teensy++ 2.0 + ADF4351 + ADL5350 draw xxxmA on 5V USB
       Teensy++ 2.0 +5v @ 50 mA       
       ADF4351      +5v @ 50 mA
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
          7|[ ]PD7      5   4        PE6[ ]|18  INT6  |-(_3k_)-------!  |         |   ADF4351  |   |   ADL5350   |
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
           |_______________________________|                     This is requried because ADF4351 is a ttl 3.3 and 
                                                                 the Teensy++ 2.0 is TTL 5v
ADF4351
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

// ADF4351 - LE setup time min 20ns
// ADF4351 - DATA to CLK setup time min 10ns
// ADF4351 - DATA to CLK hold time min 10ns
// ADF4351 - CLK high duration min 25ns
// ADF4351 - CLK low duration  min 25ns
// ADF4351 - CLK to LE setup time min 10ns
// ADF4351 - LE Pulse width is min 20ns

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

// Teensy++ 2.0 SPI to ADF4351
const int ADF4351_LEPin = 20;         //Teensy++ 2.0 SS pin  PB0-  enable ADF4350
const int ADF4351_DataPin = 22;        //Teensy++ 2.0 MOSI pin PB2
const int ADF4351_CLKPin = 21;         //Teensy++ 2.0 SCLK pin PB1
const int ADF4351_LDPin = 26;          //Teensy++ 2.0 PWN(OC1B) pin PB6
const int ledPin = 6;                  // Teensy++ 2.0 has the LED on pin 6
// 
// Below are the ADF4350 settings for a 1 GHz +5dBm output from IanWraithv/24DownConvert.
//ADF4351 Reg's
unsigned long Reg[6] {     
  0x00500000,
  0x08008011,
  0x00004E42,
  0x000004B3,
  0x00ac803c,
  0x00580005
};

byte tenHz, hundredHz, ones, tens, hundreds, thousands, tenthousands, hundredthousands, millions; 
#define LE_DELAY 1
#define Data_write_DELAY 2500
#define  LD_Wait_Delay 20

void setup() {

  Serial.begin(115200);// USB to PC for Debug only
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  
  pinMode(ADF4351_LDPin, INPUT); 
  pinMode (ADF4351_LEPin, OUTPUT);
  digitalWrite(ADF4351_LEPin, HIGH);  // disable ADF4351
  pinMode(ADF4351_CLKPin, OUTPUT);
  digitalWrite(ADF4351_CLKPin, LOW);  // disable CLK
  pinMode(ADF4351_DataPin, OUTPUT);
  digitalWrite(ADF4351_DataPin, LOW);  // Clear data line

  delay(500);                          // wait 
  digitalWrite(ADF4351_LEPin, LOW);    // enable ADF4351
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.begin();
  delay(500);

}

void loop() {
  
     SetADFFreq();
     int count = 0;
     while (digitalRead(ADF4351_LDPin) == 0) {
          Serial.println("Waitng for LD");
          blinkLed(1000);
          count = count + 1;
          Serial.println(count);
          if (count == LD_Wait_Delay) {  // we waited to x seconds lets try setting up the ADF4351 again
            blinkLed(500);
            count = 0;
            digitalWrite(ADF4351_LEPin, HIGH);  // disable ADF4351
            delayMicroseconds(LE_DELAY+10);     // lets wait for the device to stabalize       
            digitalWrite(ADF4351_LEPin, LOW);   // enable it again ADF4351            
            SetADFFreq();
          }
     }
     while (digitalRead(ADF4351_LDPin) == 1) {
          Serial.println("We have LD");
          blinkLed(2000);
     }     
}


void blinkLed(int leddelay)
{
  digitalWrite(ledPin, HIGH);       // set the LED on
  delay(leddelay);                  // wait for a second
  digitalWrite(ledPin, LOW);        // set the LED off
  delay(leddelay);                  // wait for a second
}

void SetADFFreq()
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
  for (int i = 0; i < 4; i++)
    buf[i] = (byte)(Reg[idx] >> (i * 8));
  WriteADF(buf[3], buf[2], buf[1], buf[0]);
}

int WriteADF(byte a1, byte a2, byte a3, byte a4) {
  // write over SPI to ADF4350
  blinkLed(250);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(ADF4351_LEPin, LOW);
  // delay to for clock
  delayMicroseconds(LE_DELAY);         // wait for clock to start a cycle - ADF4351 - LE setup time min 20ns
                                      // - ADF4351 - DATA to CLK setup time min 10ns
                                      // - ADF4351 - DATA to CLK hold time min 10ns
                                      // - ADF4351 - CLK high duration min 25ns
                                      // - ADF4351 - CLK low duration  min 25ns
                                      // - ADF4351 - LE Pulse width min 20ns
  SPI.transfer(a1);
  SPI.transfer(a2);
  SPI.transfer(a3);
  SPI.transfer(a4);
  delayMicroseconds(LE_DELAY);               // wait for clock to finish a cycle  - ADF4351 - CLK to LE setup time min 10ns
  Toggle();
  SPI.endTransaction();
}
int Toggle() {

  digitalWrite(ADF4351_LEPin, HIGH);
  delayMicroseconds(LE_DELAY);               // wait for clock to finish a cycle - ADF4351 - LE Pulse width is min 20ns
  digitalWrite(ADF4351_LEPin, LOW);
}
