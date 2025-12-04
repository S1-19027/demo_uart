#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stdio.h"
/*******************************************************************************
//STM32F207VGT6 FOR ENC28J60
//SPI2初始化/IO初始化等
//SPI configuration
// JUST FOR STM32F2XX
*******************************************************************************/
void ENC25Q80_SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

     /*!< Enable the SPI2 clock */        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
 
    /*!< Enable GPIO clocks */
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO , ENABLE); 	
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //Disable jtag	,Enable SWD
		GPIO_PinRemapConfig(GPIO_Remap_SPI1 , ENABLE); //Disable jtag	,Enable SWD
    /*!< SPI2 pins configuration */
    
   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // also 100Mhz
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);// PB10/14/15-SCK,MISO,MOSI
      
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // CS for enc28j60
    GPIO_Init(GPIOC, &GPIO_InitStructure);
  
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//选择了串行时钟的稳态:时钟悬空高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//数据捕获于第二个时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//(SPI_NSS_Soft)此时NSS引脚可以配置成普通GPIO去控制从设备
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//Fclk/2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /* Initialize the SPI_FirstBit member */
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
 
}
#define W25X_DeviceID 0xAB
#define SPI_FLASH_CS_LOW() GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define SPI_FLASH_CS_HIGH() GPIO_SetBits(GPIOC,GPIO_Pin_3)

/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI读写一个字节（发送完成后返回本次通讯读取的数据）
* Input          : unsigned char data 
* Output         : None
* Return         : unsigned char 
*******************************************************************************/
//发送一个字节
u8 SPI2_ReadWriteByte(u8 data)
{
	SPI_I2S_SendData(SPI2,data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI2);
}

u8 SPI2_ReadStatus()
{
	u8 status;
	SPI_FLASH_CS_LOW(); 
	SPI2_ReadWriteByte(0x05); 
	status = SPI2_ReadWriteByte(0); 
	SPI_FLASH_CS_HIGH(); 
	return status ;
}

void SPI2_WriteEnable()
{
	SPI_FLASH_CS_LOW();
	SPI2_ReadWriteByte(0x06); 
	SPI_FLASH_CS_HIGH(); 
}
void SPI2_WriteDisable()
{
	SPI_FLASH_CS_LOW();
	SPI2_ReadWriteByte(0x04); 
	SPI_FLASH_CS_HIGH(); 
}

u32 SPI_FLASH_ReadDeviceID(void) 
{ 
	u32 Temp = 0;

	//获取设备ID
	//使用的时候就拉低/ 
	SPI_FLASH_CS_LOW(); 
	/* Send “RDID ” instruction */ 
	SPI2_ReadWriteByte(0xAB); 
	SPI2_ReadWriteByte(0); 
	SPI2_ReadWriteByte(0); 
	SPI2_ReadWriteByte(0); 
	/* Read a byte from the FLASH */ 
	Temp = SPI2_ReadWriteByte(0); 
	/* Deselect the FLASH: Chip Select high */ 
	SPI_FLASH_CS_HIGH(); 

	//方法2，获取厂商ID和设备ID
	SPI_FLASH_CS_LOW(); 
	/* Send “RDID ” instruction */ 
	SPI2_ReadWriteByte(0x90); 
	SPI2_ReadWriteByte(0); 
	SPI2_ReadWriteByte(0); 
	SPI2_ReadWriteByte(0); 
	/* Read a byte from the FLASH */ 
	Temp = (Temp << 8) + SPI2_ReadWriteByte(0); 
	Temp = (Temp << 8) + SPI2_ReadWriteByte(0); 
	/* Deselect the FLASH: Chip Select high */ 
	SPI_FLASH_CS_HIGH(); 	
	
return Temp; 
} 

void EraseSector(u32 addr)
{
	SPI2_WriteEnable();
	SPI_FLASH_CS_LOW(); 
	SPI2_ReadWriteByte(0x20); 
	SPI2_ReadWriteByte(addr >> 16); 
	SPI2_ReadWriteByte(addr >> 8); 
	SPI2_ReadWriteByte(addr); 
	SPI_FLASH_CS_HIGH(); 		
	
	while(SPI2_ReadStatus() & 0x01);
	
}



void ProgramPage(u32 addr,u8 *buf)
{
	int i ;
	SPI2_WriteEnable();
	SPI_FLASH_CS_LOW(); 
	SPI2_ReadWriteByte(0x02); 
	SPI2_ReadWriteByte(addr >> 16); 
	SPI2_ReadWriteByte(addr >> 8); 
	SPI2_ReadWriteByte(addr); 
	for (i = 0 ; i < 256 ;i ++  )
	{
		SPI2_ReadWriteByte(buf[i]); //此处仅为示例，按地址序号写入从0-255的值
	}
	SPI_FLASH_CS_HIGH(); 	
	while(SPI2_ReadStatus() & 0x01);	
}
void ReadPage(u32 addr,u8 *buf)
{
	int i ;
	SPI_FLASH_CS_LOW(); 
	SPI2_ReadWriteByte(0x03); 
	SPI2_ReadWriteByte(addr >> 16); 
	SPI2_ReadWriteByte(addr >> 8); 
	SPI2_ReadWriteByte(addr); 
	for (i = 0 ; i < 256 ;i ++  )
	{
		buf[i] = SPI2_ReadWriteByte(0); //此处仅为示例，按地址序号写入从0-255的值
	}
	SPI_FLASH_CS_HIGH(); 		
}


