//////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : oled12864.c
//  版 本 号   : v2.0
//  作    者   : hejunlin
//  生成日期   : 2022-06-28
//  最近修改   :
//  功能描述   : 0.96寸OLED 接口演示例程
//              说明:
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PB3（SCL）
//              SDA   接PB4（SDA）
//              ----------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////?

#include "oled12864.h"
#include "stdlib.h"
#include "oledfont.h"
//#include "delay.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Clr();
	OLED_SDIN_Set();

}

void IIC_WaitAck()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void IIC_WriteByte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}


}
/**********************************************
// IIC Write Command
**********************************************/
void IIC_WriteCommand(unsigned char IIC_Command)
{
   IIC_Start();
   IIC_WriteByte(0x78);            //Slave address,SA0=0
	IIC_WaitAck();
   IIC_WriteByte(0x00);			//write command
	IIC_WaitAck();
   IIC_WriteByte(IIC_Command);
	IIC_WaitAck();
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void IIC_WriteData(unsigned char IIC_Data)
{
   IIC_Start();
   IIC_WriteByte(0x78);			//D/C#=0; R/W#=0
	IIC_WaitAck();
   IIC_WriteByte(0x40);			//write data
	IIC_WaitAck();
   IIC_WriteByte(IIC_Data);
	IIC_WaitAck();
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		IIC_WriteData(dat);
	}
	else
	{
		IIC_WriteCommand(dat);
	}


}


/********************************************
// OLED_FillPicture
********************************************/
void OLED_FillPicture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{
		for(j=0;j<123;j++);
	}
}

//坐标设置

void OLED_SetPos(unsigned char x, unsigned char y)
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
}
//开启OLED显示
void OLED_DisplayOn(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void OLED_DisplayOff(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
	} //更新显示
}
void OLED_On(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA);
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
	unsigned char c=0,i=0;
		c=chr-' ';//得到偏移后的值
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_SetPos(x,y);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_SetPos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);

			}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
//				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				OLED_ShowChar(x+(size2/2)*t,y,'0',size2);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
	}
}
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
 unsigned int j=0;
 unsigned char x,y;

  if(y1%8==0) y=y1/8;
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
		for(x=x0;x<x1;x++)
	    {
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);
	    }
	}
}



// 快速绘制图像
void OLED_DrawBMPFast(unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	for (y = 0; y < 8; y++)
	{
		OLED_SetPos(0, y);
		IIC_Start();
		IIC_WriteByte(0x78);
		IIC_WaitAck();
		IIC_WriteByte(0x40);
		IIC_WaitAck();
		for (x = 0; x < 128; x++)
		{
			IIC_WriteByte(BMP[j++]);
			IIC_WaitAck();
		}
		IIC_Stop();
	}
}

void oled_drawbmp_block_clear(int bx, int by, int clear_size)
{
	unsigned int i;
	OLED_SetPos(bx, by);
	IIC_Start();
	IIC_WriteByte(0x78);
	IIC_WaitAck();
	IIC_WriteByte(0x40);
	IIC_WaitAck();

	for (i = 0; i < clear_size; i++)
	{
		if (bx + i>128) break;
		IIC_WriteByte(0x0);
		IIC_WaitAck();
	}
	IIC_Stop();
}

void OLED_DrawGround()
{
	static unsigned int pos = 0;
	unsigned char speed = 5;
	unsigned int ground_length = sizeof(GROUND);
	unsigned char x;

	OLED_SetPos(0, 7);
	IIC_Start();
	IIC_WriteByte(0x78);
	IIC_WaitAck();
	IIC_WriteByte(0x40);
	IIC_WaitAck();
	for (x = 0; x < 128; x++)
	{
		IIC_WriteByte(GROUND[(x+pos)%ground_length]);
		IIC_WaitAck();
	}
	IIC_Stop();

	pos = pos + speed;
	//if(pos>ground_length) pos=0;
}


// 绘制云朵
void OLED_DrawCloud()
{
	static int pos = 128;
	static char height=0;
	char speed = 3;
	unsigned int i=0;
	int x;
	int start_x = 0;
	int length = sizeof(CLOUD);
	unsigned char byte;

	//if (pos + length <= -speed) pos = 128;

	if (pos + length <= -speed)
	{
		pos = 128;
		height = rand()%3;
	}
	if(pos < 0)
	{
		start_x = -pos;
		OLED_SetPos(0, 1+height);
	}
	else
	{
		OLED_SetPos(pos, 1+height);
	}

	IIC_Start();
	IIC_WriteByte(0x78);
	IIC_WaitAck();
	IIC_WriteByte(0x40);
	IIC_WaitAck();
	for (x = start_x; x < length + speed; x++)
	{
		if (pos + x > 127) break;
		if (x < length) byte = CLOUD[x];
		else byte = 0x0;

		IIC_WriteByte(byte);
		IIC_WaitAck();
	}
	IIC_Stop();

	pos = pos - speed;
}

// 绘制小恐龙
void OLED_DrawDino()
{
	static unsigned char dino_dir = 0;
	unsigned int j=0;
	unsigned char x, y;
	unsigned char byte;

	dino_dir++;
	dino_dir = dino_dir%2;
	for(y=0; y<2; y++)
	{
		OLED_SetPos(16, 6+y);
		IIC_Start();
		IIC_WriteByte(0x78);
		IIC_WaitAck();
		IIC_WriteByte(0x40);
		IIC_WaitAck();
		for (x = 0; x < 16; x++)
		{
			j = y*16 + x;
			byte = DINO[dino_dir][j];

			IIC_WriteByte(byte);
			IIC_WaitAck();
		}
		IIC_Stop();
	}
}

// 绘制仙人掌障碍物
void OLED_DrawCactus()
{
	char speed = 5;
	static int pos = 128;
	int start_x = 0;
	int length = sizeof(CACTUS_2)/2;

	unsigned int j=0;
	unsigned char x, y;
	unsigned char byte;

	if (pos + length <= 0)
	{
		oled_drawbmp_block_clear(0, 6, speed);
		pos = 128;
	}

	for(y=0; y<2; y++)
	{
		if(pos < 0)
		{
			start_x = -pos;
			OLED_SetPos(0, 6+y);
		}
		else
		{
			OLED_SetPos(pos, 6+y);
		}

		IIC_Start();
		IIC_WriteByte(0x78);
		IIC_WaitAck();
		IIC_WriteByte(0x40);
		IIC_WaitAck();

		for (x = start_x; x < length; x++)
		{
			if (pos + x > 127) break;
			j = y*length + x;
			byte = CACTUS_2[j];
			IIC_WriteByte(byte);
			IIC_WaitAck();
		}
		IIC_Stop();
	}
	oled_drawbmp_block_clear(pos + length, 6, speed); // 清除残影
	pos = pos - speed;
}


// 绘制随机出现的仙人掌障碍物
int OLED_DrawCactusRandom(unsigned char ver, unsigned char reset)
{
	char speed = 5;
	static int pos = 128;
	int start_x = 0;
	int length = 0;

	unsigned int i=0, j=0;
	unsigned char x, y;
	unsigned char byte;
	if (reset == 1)
	{
		pos = 128;
		oled_drawbmp_block_clear(0, 6, speed);
		return 128;
	}
	if (ver == 0) length = 8; //sizeof(CACTUS_1) / 2;
	else if (ver == 1) length = 16; //sizeof(CACTUS_2) / 2;
	else if (ver == 2 || ver == 3) length = 24;

	for(y=0; y<2; y++)
	{
		if(pos < 0)
		{
			start_x = -pos;
			OLED_SetPos(0, 6+y);
		}
		else
		{
			OLED_SetPos(pos, 6+y);
		}

		IIC_Start();
		IIC_WriteByte(0x78);
		IIC_WaitAck();
		IIC_WriteByte(0x40);
		IIC_WaitAck();

		for (x = start_x; x < length; x++)
		{
			if (pos + x > 127) break;

			j = y*length + x;
			if (ver == 0) byte = CACTUS_1[j];
			else if (ver == 1) byte = CACTUS_2[j];
			else if(ver == 2) byte = CACTUS_3[j];
			else byte = CACTUS_4[j];

			IIC_WriteByte(byte);
			IIC_WaitAck();
		}
		IIC_Stop();
	}

	oled_drawbmp_block_clear(pos + length, 6, speed);

	pos = pos - speed;
	return pos + speed;
}




// 绘制跳跃小恐龙
int OLED_DrawDinoJump(char reset)
{
	char speed_arr[] = {1, 1, 3, 3, 4, 4, 5, 6, 7};
	static char speed_idx = sizeof(speed_arr)-1;
	static int height = 0;
	static char dir = 0;
	//char speed = 4;

	unsigned int j=0;
	unsigned char x, y;
	char offset = 0;
	unsigned char byte;
	if(reset == 1)
	{
		height = 0;
		dir = 0;
		speed_idx = sizeof(speed_arr)-1;
		return 0;
	}
	if (dir==0)
	{
		height += speed_arr[speed_idx];
		speed_idx --;
		if (speed_idx<0) speed_idx = 0;
	}
	if (dir==1)
	{
		height -= speed_arr[speed_idx];
		speed_idx ++;
		if (speed_idx>sizeof(speed_arr)-1) speed_idx = sizeof(speed_arr)-1;
	}
	if(height >= 31)
	{
		dir = 1;
		height = 31;
	}
	if(height <= 0)
	{
		dir = 0;
		height = 0;
	}
	if(height <= 7) offset = 0;
	else if(height <= 15) offset = 1;
	else if(height <= 23) offset = 2;
	else if(height <= 31) offset = 3;
	else offset = 4;

	for(y=0; y<3; y++) // 4
	{
		OLED_SetPos(16, 5- offset + y);

		IIC_Start();
		IIC_WriteByte(0x78);
		IIC_WaitAck();
		IIC_WriteByte(0x40);
		IIC_WaitAck();
		for (x = 0; x < 16; x++) // 32
		{
			j = y*16 + x; // 32
			byte = DINO_JUMP[height%8][j];

			IIC_WriteByte(byte);
			IIC_WaitAck();
		}
		IIC_Stop();
	}
	if (dir == 0) oled_drawbmp_block_clear(16, 8- offset, 16);
	if (dir == 1) oled_drawbmp_block_clear(16, 4- offset, 16);
	return height;
}

// 绘制重启
void OLED_DrawRestart()
{
	unsigned int j=0;
	unsigned char x, y;
	unsigned char byte;
	//OLED_SetPos(0, 0);
	for (y = 2; y < 5; y++)
	{
		OLED_SetPos(52, y);
		IIC_Start();
		IIC_WriteByte(0x78);
		IIC_WaitAck();
		IIC_WriteByte(0x40);
		IIC_WaitAck();
		for (x = 0; x < 24; x++)
		{
			byte = RESTART[j++];
			IIC_WriteByte(byte);
			IIC_WaitAck();
		}
		IIC_Stop();
	}
	OLED_ShowString(10, 3, "GAME", 16);
	OLED_ShowString(86, 3, "OVER", 16);
}
// 绘制封面
void OLED_DrawCover()
{
	OLED_DrawBMPFast(COVER);
}



//初始化SSD1306
void OLED_Init(void)
{
	Delay_50ms(20);
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//

	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//

	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//

	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//

	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//

	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//

	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//

	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	Delay_50ms(20);
	OLED_FillPicture(0x0);

}





























