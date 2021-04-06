#include "tcs34725.h"
#include "stm32h7xx.h"
#include "delay.h"
void RGBLEDInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	////////////////////////////////////RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	/////////////////////////////GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

static u16 _rdata, _gdata, _bdata, _cdata;

void TCS34725_I2C_Write(u8 slaveAddress, u8 *dataBuffer, u8 bytesNumber, u8 stopBit)
{

	unsigned char i = 0;

	TCS34725_I2C_Start();
	TCS34725_I2C_Send_Byte((slaveAddress << 1) | 0x00); // 7λ�Ի���ַ�����ƣ�ĩβ0����1���������д
	TCS34725_I2C_Wait_ACK();

	for (i = 0; i < bytesNumber; i++)
	{
		TCS34725_I2C_Send_Byte(*(dataBuffer + i));
		TCS34725_I2C_Wait_ACK();
	}
	if (stopBit == 1)
		TCS34725_I2C_Stop();
}

void TCS34725_I2C_Read(u8 slaveAddress, u8 *dataBuffer, u8 bytesNumber, u8 stopBit)
{
	unsigned char i = 0;
	TCS34725_I2C_Start();
	TCS34725_I2C_Send_Byte((slaveAddress << 1) | 0x01);
	TCS34725_I2C_Wait_ACK();
	for (i = 0; i < bytesNumber; i++)
	{
		if (i == bytesNumber - 1)
		{
			*(dataBuffer + i) = TCS34725_I2C_Read_Byte(0);
		}
		else
		{
			*(dataBuffer + i) = TCS34725_I2C_Read_Byte(1);
		}
	}
	if (stopBit == 1)
		TCS34725_I2C_Stop();
}

void TCS34725_Write(unsigned char subAddr, unsigned char *dataBuffer, unsigned char bytesNumber)
{
	unsigned char sendBuffer[10] = {
		0,
	};
	unsigned char byte = 0;

	sendBuffer[0] = subAddr | TCS34725_COMMAND_BIT;
	for (byte = 1; byte <= bytesNumber; byte++)
	{
		sendBuffer[byte] = dataBuffer[byte - 1];
	}
	TCS34725_I2C_Write(TCS34725_ADDRESS, sendBuffer, bytesNumber + 1, 1);
}

void TCS34725_Read(unsigned char subAddr, unsigned char *dataBuffer, unsigned char bytesNumber)
{
	subAddr |= TCS34725_COMMAND_BIT;

	TCS34725_I2C_Write(TCS34725_ADDRESS, (unsigned char *)&subAddr, 1, 0);
	TCS34725_I2C_Read(TCS34725_ADDRESS, dataBuffer, bytesNumber, 1);
}

// ��ʼ��
bool TCS34725_Init(void)
{
	unsigned char status[1] = {0};
	IIC_Init();
	RGBLEDInit();
	delay_ms(10);
	TCS34725_Read(TCS34725_ID, status, 1);
	if (status[0] == 0x44)
	{
		TCS34725_Setup();
		delay_ms(5);
		TCS34725_Disable();
		delay_ms(5);
		TCS34725_Enable();
		return true;
	}
	else
		return false;
}
/******************************************
������Ҫ���� ����ʱ��(ATIME �Ĵ���)����(CONTROL �Ĵ���)
ʱ��Խ��������Խ�󣬵õ�����ֵԽ�󡣹�ʽ����
The actual time can be calculated as follows:

ATIME = 256 - Integration Time / 2.4 ms

Inversely, the time can be calculated from the register value as follows:

Integration Time = 2.4 ms x (256 - ATIME)

For example, if a 100-ms integration time is needed, the device needs to be programmed to:

256 - (100 / 2.4) = 256 - 42 = 214 = 0xD6

Conversely, the programmed value of 0xC0 would correspond to:

(256 - 0xC0)x2.4 = 64 ? 2.4 = 154 ms.
********************************************/
// ����ʱ��	0xff=2.4ms 1024
//	0xf6 24ms					10240	  ����ʹ�ø���
// 	0xd5 101ms  			43008
//	0xc0  154ms        65535
//	0x00 700ms				65535		����ʱ������ѡ��
void TCS34725_SetIntegrationTime(uint8_t time)
{
	unsigned char cmd = time;

	TCS34725_Write(TCS34725_ATIME, &cmd, 1);
}
// �������� 00����1 01����4 10����16 11����60   ͨ��ʹ��16
void TCS34725_SetGain(uint8_t gain)
{
	unsigned char cmd = gain;

	TCS34725_Write(TCS34725_CONTROL, &cmd, 1);
}
// ���üĴ���
void TCS34725_Setup(void)
{
	TCS34725_SetIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
	TCS34725_SetGain(TCS34725_GAIN_16X);
}
// ʹ�ܣ�ʹ��֮����Ҫ2.4ms��ʼ��
void TCS34725_Enable(void)
{
	unsigned char cmd = TCS34725_ENABLE_PON;

	TCS34725_Write(TCS34725_ENABLE, &cmd, 1);

	cmd = TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN;
	TCS34725_Write(TCS34725_ENABLE, &cmd, 1);
	delay_ms(3);
}
// ʧ��
void TCS34725_Disable(void)
{
	unsigned char cmd = 0x00;

	TCS34725_Read(TCS34725_ENABLE, &cmd, 1);
	cmd = cmd & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
	TCS34725_Write(TCS34725_ENABLE, &cmd, 1);
}

// ��õ���ͨ������
uint16_t TCS34725_GetChannelData(unsigned char reg)
{
	unsigned char tmp[2] = {0, 0};
	uint16_t data = 0;

	TCS34725_Read(reg, tmp, 2);
	data = ((uint16_t)tmp[1] << 8) | tmp[0];

	return data;
}

// ��ø���ͨ������
bool TCS34725_GetRawData(void)
{
	unsigned char status[1] = {0};

	status[0] = TCS34725_STATUS_AVALID;

	TCS34725_Read(TCS34725_STATUS, status, 1);

	if (status[0] & TCS34725_STATUS_AVALID)
	{
		_cdata = TCS34725_GetChannelData(TCS34725_CDATAL);
		_rdata = TCS34725_GetChannelData(TCS34725_RDATAL);
		_gdata = TCS34725_GetChannelData(TCS34725_GDATAL);
		_bdata = TCS34725_GetChannelData(TCS34725_BDATAL);
		return true;
	}
	return false;
}

u16 GetRData(void)
{
	return _rdata;
}

u16 GetGData(void)
{
	return _gdata;
}

u16 GetBData(void)
{
	return _bdata;
}

u16 GetCData(void)
{
	return _cdata;
}
