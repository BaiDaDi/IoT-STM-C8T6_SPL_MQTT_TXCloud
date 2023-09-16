#include "DHT11.h"

#define Delay_ms(x)  HAL_Delay(x)

u8 crc;
DHT11_INFO dht11;
u8 error = 0;
//#define MY_ONELINE


void gpio_out(void) // 设置GPIO为开漏输出
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // 设置GPIO为开漏输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DQ_PORT, &GPIO_InitStruct);
}
void gpio_in(void)// 设置GPIO为无上拉的输入模式
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // 设置GPIO为无上拉的输入模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DQ_PORT, &GPIO_InitStruct);
}
u8 Oneline_Start(){
    error = 0;
    gpio_out();  // 设置GPIO为输出
    ONELINE_OUT(0); 
    HAL_Delay(20); // 拉低电平20ms
    ONELINE_OUT(1); 
    delay_us(50);   // 等待50us
    gpio_in(); // 设置GPIO为输入
    if(!ONELINE_IN){ //设备存在
        while(!ONELINE_IN); //跳过响应低电平ME
        while(ONELINE_IN); //跳过DHT11拉高预备EF
        return 1;
    }
    else return 0;
}
u8 Oneline_bit(){ // 读取bit数据
    u8 flag; u8 retry = 0;
    while(!ONELINE_IN && retry++ < 100) delay_us(1); //跳过bit起始低电平
    if(retry >= 100) return 2; // 低电平时间过长，读取失败
    delay_us(30);  
    flag = ONELINE_IN; // 延时30us， 读取电平高低
    retry = 0;
    while(ONELINE_IN && retry++ < 100) delay_us(1); // 跳过高电平时间
    return flag;
}
u8 Oneline_Read(){ // 读取byte数据
    u8 i, bit, byte=0;
    if(error) return 0;
    for(i=0;i<8;i++){ // 循环读取bit数据
        bit = Oneline_bit();
        if(bit > 1) {
            error = 1;
            return 0;
        }
        byte <<= 1; byte |= bit;
    }
    return byte;
}
void DHT11_Read(){// 读取温湿度数据数据
    u8 tem, tem_deci, humid, humid_deci, crc ,crc_res;
    if(Oneline_Start()){
        humid = Oneline_Read();
        humid_deci = Oneline_Read();
        tem = Oneline_Read();
        tem_deci = Oneline_Read();
        crc = Oneline_Read();
        crc_res = crc == (humid+humid_deci+tem+tem_deci); // 和校验
        // if(!error && crc_res) print("tem:%d.%d, humid:%d.%d\n", tem, tem_deci, humid, humid_deci);
        if(!error && crc_res) {
            dht11.t = tem + tem_deci/10.0f;
            dht11.h = humid;
        }
    }
}
void DHT11_Init(){
    HAL_Delay(500);
}
