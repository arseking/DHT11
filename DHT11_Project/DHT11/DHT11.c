#include <reg52.h>
#include "Delay.h" 
#include "DHT11.h"
#include "Type.h" 
 
sbit Data=P1^0; //数据线
 
u8 rec_dat[9]; //储存数据
 
/** 
 * 函数名称：	DHT11_start
 *
 * 功能说明：	主机（单片机）给DHT11发送起始信号；
 *
 * 参数说明：	无
 *
 * 函数返回：	无
 */
void DHT11_start()
{
	/* 发送开始信号 */
	Data=0;
	Delay_ms(20);   //拉低 20ms
	
	/* 释放总线，等待响应 */
	Data=1;
	Delay_us(2);    //拉高至少20us，DHT11才能响应
}
 
/** 
 * 函数名称：	DHT11_rec_byte
 *
 * 功能说明：	主机（单片机）接收DHT11一字节数据；
 *
 * 参数说明：	无
 *
 * 函数返回：	dat - 从DHT11获取的一个字节数据
 * 
 */
u8 DHT11_rec_byte()
{
	u8 i,dat=0;
 
	/* 从高到低依次接收8位数据 */
	for(i=0;i<8;i++)
	{          
		while(Data);   													//等待低电平，DHT11开始传输数据
		while(!Data);  													//等待54us低电平过去
		
		/* 通过高电平时间长度判断数据是0是1 */
		Delay_us(6);   													//延时50us（数据0是24us，数据1是71us）
		dat<<=1;			 													//移位（dat保留前面获取的数据，并低位补零）
		if(Data==1) 														//延时50us后仍为高电平，则数据是1
		{
			dat+=1;																//dat通过+1给地位写入1
		}
		while(Data);   													//等待数据线拉低
	}  
	return dat;
}
 
/** 
 * 函数名称：	DHT11_receive
 *
 * 功能说明：	主机（单片机）接收DHT11完整的温湿度数据；
 *
 * 参数说明：	无
 *
 * 函数返回：	无
 */
void DHT11_receive()
{
	u8 R_H,R_L,T_L,T_H,RH,RL,TH,TL,revise;
	
	DHT11_start();						//发送起始信号：
	
	/* 判断DHT11是否响应 */
	if(Data==0)
	{
		/* DHT11响应 */
		while(!Data);   			 					//等待拉高     
		Delay_us(13);  					 				//延时92us，等待传输数据	
 
		/* 接收温湿度数据 */
		R_H=DHT11_rec_byte();    				//接收湿度高八位  
		R_L=DHT11_rec_byte();    				//接收湿度低八位  
		T_H=DHT11_rec_byte();    				//接收温度高八位  
		T_L=DHT11_rec_byte();    				//接收温度低八位
		revise=DHT11_rec_byte();				//接收校正位
 
		/* 校验温湿度数据 */
		if((R_H+R_L+T_H+T_L)==revise)		//温湿度4字节数据相加是否等于校验位数据
		{
				RH=R_H;
				RL=R_L;
				TH=T_H;
				TL=T_L;
		} 
 
		/*湿度数据*/
		rec_dat[0]='0'+(RH/10);
		rec_dat[1]='0'+(RH%10);
		rec_dat[2]=' ';
		rec_dat[3]=' ';
 
		/*温度数据*/
		rec_dat[4]='0'+(TH/10);
		rec_dat[5]='0'+(TH%10); 
		rec_dat[6]=' ';
		rec_dat[7]=' ';
	}
}