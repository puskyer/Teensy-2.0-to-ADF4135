GPIO�Ĵ������� ��STM32�ο��ֲ�����-p75��
GPIO Register Description STM32 Reference Manual Chinese-p75

1���˿����õͼĴ���(GPIOx_CRL)(x = A...E)
2���˿����ø߼Ĵ���(GPIOx_CRH)(x = A...E)
1. The port is configured with low register (GPIOx_CRL) (x = A... E)
2. The port is configured with a high register (GPIOx_CRH) (x = A... E)

3���˿��������ݼĴ���(GPIOx_IDR)(x = A...E)
   ��ЩλΪֻ����ֻ������(16λ)����ʽ������������ֵΪ��ӦI/O�ڵ�״̬��
3. Port input data register (GPIOx_IDR) (x = A... E)
   These bits are read-only and can only be read out as words (16 bits). The readout value is the state of the corresponding I/O port.
   
4���˿�������ݼĴ���(GPIOx_ODR)(x = A...E)
   ֻ������(16bit)����ʽ��������λֵȫ��0��д0�����0��д1�����1��
4. Port output data register (GPIOx_ODR) (x = A... E)
   It can only be operated in the form of a word (16bit), and the reset value is all 0. Write 0 to output 0, write 1 to output 1.
   
5���˿�λ����/����Ĵ���(GPIOx_BSRR)(x = A...E)
   ��16bitд1������0����16bitд1������λ��ͬʱд1�Ļ���16bi��Ч��
5. Port bit setting/clear register (GPIOx_BSRR) (x = A... E)
   High 16bit write 1 is used for clear 0, low 16 bit write 1 is used for positioning, while writing 1 is effective for low 16bi.
   
6���˿�λ����Ĵ���(GPIOx_BRR)(x = A...E)
   ��16λд1������λ��
6. Port bit clear register (GPIOx_BRR) (x = A... E)
   The low 16-bit write 1 is used for positioning.
   
7���˿����������Ĵ���(GPIOx_LCKR)(x = A...E)
7. Port configuration lock register (GPIOx_LCKR) (x = A... E)

GPIO����ģʽ
typedef enum
{ GPIO_Mode_AIN = 0x0,            /* ģ������ */   
  GPIO_Mode_IN_FLOATING = 0x04,   /* �������룬��λ���״̬ */
  GPIO_Mode_IPD = 0x28,           /* �������� */
  GPIO_Mode_IPU = 0x48,           /* �������� */
  GPIO_Mode_Out_OD = 0x14,        /* ��©��� */
  GPIO_Mode_Out_PP = 0x10,        /* ������� */
  GPIO_Mode_AF_OD = 0x1C,         /* ���ÿ�©��� */
  GPIO_Mode_AF_PP = 0x18          /* ����������� */
}GPIOMode_TypeDef;

GPIO eight modes
typedef enum
{ GPIO_Mode_AIN = 0x0, /* Analog Input */   
  GPIO_Mode_IN_FLOATING = 0x04, /* Floating input, state after reset */
  GPIO_Mode_IPD = 0x28, /* drop-down input */
  GPIO_Mode_IPU = 0x48, /* pull-up input */
  GPIO_Mode_Out_OD = 0x14, /* open-drain output */
  GPIO_Mode_Out_PP = 0x10, /* Push-pull output */
  GPIO_Mode_AF_OD = 0x1C, /* Multiplexed open-drain output */
  GPIO_Mode_AF_PP = 0x18 /* Multiplex push-pull output */
}GPIOMode_TypeDef;