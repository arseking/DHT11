#include <reg52.h>
#include "UART_Printf.h"  

/* 串口流控制符 */
#define XON  0x11		// 启动
#define XOFF 0x13   // 中断

/** 
 * @brief 	串口初始化函数
 * @param 	无
 * @return 	无
 * @note  	
 *   1、该函数是按12M晶振配置的
 *   2、该函数只启用串口发送，没有启用串口接收
 */	
 void UART_Init()
{
	/* 串口设置 */
	SCON = 0X40;												// 设置为工作方式1（并禁止接收、清除发送和接收中断标志位）
	PCON = 0X80;												// 波特率加倍（SMOD=1）
	
	/* 定时器1设置 */
	TMOD &= 0x0F;												// 清除定时器1工作模式配置（保留定时器0工作模式配置）
	TMOD |= 0X20;												// 设置定时器1计数器工作方式2（8位自动重载模式）
	/* 4800波特率，无误码率 */
	TH1 = 0XF3;													// 设定定时初值
	TL1 = 0XF3;													// 设定定时器重装值
	/* 9600波特率，存在个别数据误码 */
	//TH1=0XF9;
	//TL1=0XF9;
	TR1 = 1;														//启动定时器1
}

/** 
 * @brief 	putchar(升级版)
 * @param 	c - 要发送的字符
 * @return 	无
 * @note
 *   功能1：将“\n”扩展为CR LF并处理（当要发送'\n'时,系统自动在前面添加上回车符，这样用户只需写“\n”就能换行）
 *   功能2：支持XON/XOFF(Ctrl+S/Ctrl+Q)协议（检查串口缓冲区是否接收到XOFF,如果是,它会等待接收端发送XON,才能继续输出数据）
 */	
char putchar (char c)
{	
	/* 检测\n字符 */
  if(c == '\n')
	{
    if(RI)											// 判断SBUF是否接受到了信息
		{
      if(SBUF == XOFF)					// 判断SBUF中的信息是否为中断信号
			{
        do
				{
          RI = 0;        				// 继续接收信息
          while (!RI);    			// 判断是否接收到了信息
        }
        while (SBUF != XON);		// 判断接收的信息是否为启动信息
        RI = 0;                 // 继续接收信息
      }
    }  
    SBUF = 0x0d;								// 把字符'\r'写入发送寄存器
    while (!TI);								// 等待发送数据完成
    TI = 0;      								// 清除发送中断标志位
  }
	/* 发送字符c */
  if (RI)												// 下面的if函数又是判断SBUF中是否接收了中断信号,与上面的程序一样
	{
    if (SBUF == XOFF)
		{
      do 
			{
        RI = 0;
        while (!RI);
      }
      while (SBUF != XON);
      RI = 0; 
    }
  }
  SBUF = c;											// 把字符c写入发送寄存器
  while (!TI);
  TI = 0;
}

#if 0 
//其他版本，不启用

/** 
 * @brief 	putchar(基础版)
 * @param 	c - 要发送的字符
 * @return 	无
 * @note
 *   功能：将“\n”扩展为CR LF并处理（当要发送'\n'时,系统自动在前面添加上回车符，这样用户只需写“\n”就能换行）
 */
char putchar (char c)
{
	/* 检测\n字符 */
  if (c == '\n')
	{
		/* 发送字符'\r' */
		SBUF = 0x0d;					// 把字符'\r'写入发送寄存器
    while (!TI);					// 等待发送数据完成
    TI = 0;								// 清除发送中断标志位
  }
	/* 发送字符c */
	SBUF = c;								// 把字符c写入发送寄存器
	while(!TI)
	TI = 0;
}

/** 
 * @brief 	putchar(迷你版)
 * @param 	c - 要发送的字符
 * @return 	无
 * @note
 *   功能：实现串口输出字符
 */
char putchar (char c)
{
	SBUF = c;             // 把字符c写入发送寄存器
	while(!TI);           // 等待发送数据完成
	TI = 0;               // 清除发送中断标志位
}

#endif
