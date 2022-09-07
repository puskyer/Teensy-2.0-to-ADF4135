GPIO寄存器描述 《STM32参考手册中文-p75》
GPIO Register Description STM32 Reference Manual Chinese-p75

1、端口配置低寄存器(GPIOx_CRL)(x = A...E)
2、端口配置高寄存器(GPIOx_CRH)(x = A...E)
1. The port is configured with low register (GPIOx_CRL) (x = A... E)
2. The port is configured with a high register (GPIOx_CRH) (x = A... E)

3、端口输入数据寄存器(GPIOx_IDR)(x = A...E)
   这些位为只读并只能以字(16位)的形式读出。读出的值为对应I/O口的状态。
3. Port input data register (GPIOx_IDR) (x = A... E)
   These bits are read-only and can only be read out as words (16 bits). The readout value is the state of the corresponding I/O port.
   
4、端口输出数据寄存器(GPIOx_ODR)(x = A...E)
   只能以字(16bit)的形式操作，复位值全是0。写0即输出0，写1即输出1。
4. Port output data register (GPIOx_ODR) (x = A... E)
   It can only be operated in the form of a word (16bit), and the reset value is all 0. Write 0 to output 0, write 1 to output 1.
   
5、端口位设置/清除寄存器(GPIOx_BSRR)(x = A...E)
   高16bit写1用于清0，低16bit写1用于置位，同时写1的话低16bi有效。
5. Port bit setting/clear register (GPIOx_BSRR) (x = A... E)
   High 16bit write 1 is used for clear 0, low 16 bit write 1 is used for positioning, while writing 1 is effective for low 16bi.
   
6、端口位清除寄存器(GPIOx_BRR)(x = A...E)
   低16位写1用于置位。
6. Port bit clear register (GPIOx_BRR) (x = A... E)
   The low 16-bit write 1 is used for positioning.
   
7、端口配置锁定寄存器(GPIOx_LCKR)(x = A...E)
7. Port configuration lock register (GPIOx_LCKR) (x = A... E)

GPIO八种模式
typedef enum
{ GPIO_Mode_AIN = 0x0,            /* 模拟输入 */   
  GPIO_Mode_IN_FLOATING = 0x04,   /* 浮空输入，复位后的状态 */
  GPIO_Mode_IPD = 0x28,           /* 下拉输入 */
  GPIO_Mode_IPU = 0x48,           /* 上拉输入 */
  GPIO_Mode_Out_OD = 0x14,        /* 开漏输出 */
  GPIO_Mode_Out_PP = 0x10,        /* 推挽输出 */
  GPIO_Mode_AF_OD = 0x1C,         /* 复用开漏输出 */
  GPIO_Mode_AF_PP = 0x18          /* 复用推挽输出 */
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