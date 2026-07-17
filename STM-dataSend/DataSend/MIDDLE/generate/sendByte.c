uint32_t  temp_send;  
char ByteSend[5]={0};//发送的字节  
uint i;  
//主函数  
int main(void)  
{  
  while(1)  
  {  
    ByteSend[0] = 0xAA;//帧头，ByteSend的类型为uchar或char均可         
    ByteSend[1] = (temp_send>>24) & 0xFF;//  
    ByteSend[2] = (temp_send>>16) & 0xFF;  
    ByteSend[3] = (temp_send>>8) & 0xFF;  
    ByteSend[4] = temp_send & 0xFF;   
    for(i=0;i<5;i++)  
     {   
       UART4_Put_Char(ByteSend[i]);  
      }         
  }  
  
}  
//子函数  
/**************************实现函数********************************************
*函数原型:  void UART4_Put_Char(unsigned char DataToSend)
*功　　能:  RS232发送一个字节
输入参数：
  unsigned char DataToSend   要发送的字节数据
输出参数：没有  
*******************************************************************************/  
void sendByte(unsigned char DataToSend)  
{  
  TxBuffer[count++] = DataToSend;   
  USART_ITConfig(UART4, USART_IT_TXE, ENABLE);   
} 