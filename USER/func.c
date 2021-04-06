u16 Get12AD16(u16 ad)
{
	float vv;
	u16 ad16;
	vv = ad * 1.2 / After_filter[0]; //?????
	ad16 = vv * 65536 / 3.3;
	return ad16;
}

void CompADC(void)
{
	int sum = 0;
	u8 i;
	u8 count;
	for (i = 0; i < M; i++)
	{
		for (count = 0; count < N; count++)
		{
			sum += AD_Value[count][i];
		}
		After_filter[i] = sum / N;
		sum = 0;
	}
}

void Set5VBase(void)
{
	u8 i = 0;
	u16 cc;
	CompADC();
	for (i = 0; i < 6; i++)
	{
		ADH[i] = Get12AD16(After_filter[i + 1]);
	}
}

void GpuSend(char *buf1)
{
	u8 i = 0;
	while (1)
	{
		if (buf1[i] != 0)
		{
			USART_SendData(USART1, buf1[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			{
			}; //??????
			i++;
		}
		else
			return;
	}
}

void CompVol(void) //Calc voltage
{
	u8 i;
	for (i = 1; i < 7; i++)
	{ //LV[i-1]=(After_filter[i]*9.5076923076923076923076923076923/After_filter[0]);
		//voltage=(After_filter[i]*1.2*/After_filter[0]);
		//V=voltage*(30.9/3.9)

		//-----------------------------
		LV[i - 1] = (Get12AD16(After_filter[i]) * 5.0) / ADH[i - 1];
	}
}

void CompLi(void) //Calc voltage
{
	u8 i;
	Li[0] = LV[0];
	for (i = 1; i < 7; i++)
	{
		Li[i] = LV[i] - LV[i - 1];
	}
}

//uc16 Liqx[10]={310,345,352,358,362,368,370,372,380,390,410};

void CompCellPer(void)
{ //0-3.1V  10-3.45  20-3.52 30-3.58V  40 3.62  50 -3.68  60 3.7   70 3.72  80 3.8
	//90 3.9  100- 4.1
	u8 i, j;
	u16 v;
	for (i = 0; i < 6; i++)
	{
		CellPer[i] = 0;
		v = Li[i] * 100;
		for (j = 0; j < 10; j++)
		{
			if (v >= Liqx[j])
				CellPer[i] = j;
		}
	}
}

//====================EEPROM=================
uc16 D_ADH[6] = {0x2eaa, 0x2eaa, 0x2eaa, 0x2eaa, 0x2eaa, 0x2eaa};
//--0~0x3F  normal variable storage
//-0x40~0x4B 5v adjust data
void LoadROM(void)
{
	u8 i;
	for (i = 0; i < 6; i++)
	{
		if (*(u16 *)(EEPROMADDR + 64 + i * 2) == 65535)
		{
			ADH[i] = D_ADH[i];
		}
		else
		{
			ADH[i] = *(u16 *)(EEPROMADDR + 64 + i * 2);
		}
	}
}

void SaveROM(void)
{
	u8 i;
	FLASH_Unlock();															//?? FLASH ???
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); //????????
	FLASH_ErasePage(EEPROMADDR);
	for (i = 0; i < 6; i++)
	{
		FLASH_ProgramHalfWord(EEPROMADDR + 64 + i * 2, ADH[i]);
	}
	FLASH_Lock(); //?? FLASH ???
}