#include "DHT11.h"

#define Delay_ms(x)  HAL_Delay(x)

u8 crc;
DHT11_INFO dht11;
u8 error = 0;
//#define MY_ONELINE


void gpio_out(void) // ����GPIOΪ��©���
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // ����GPIOΪ��©���
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DQ_PORT, &GPIO_InitStruct);
}
void gpio_in(void)// ����GPIOΪ������������ģʽ
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // ����GPIOΪ������������ģʽ
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DQ_PORT, &GPIO_InitStruct);
}
u8 Oneline_Start(){
    error = 0;
    gpio_out();  // ����GPIOΪ���
    ONELINE_OUT(0); 
    HAL_Delay(20); // ���͵�ƽ20ms
    ONELINE_OUT(1); 
    delay_us(50);   // �ȴ�50us
    gpio_in(); // ����GPIOΪ����
    if(!ONELINE_IN){ //�豸����
        while(!ONELINE_IN); //������Ӧ�͵�ƽME
        while(ONELINE_IN); //����DHT11����Ԥ��EF
        return 1;
    }
    else return 0;
}
u8 Oneline_bit(){ // ��ȡbit����
    u8 flag; u8 retry = 0;
    while(!ONELINE_IN && retry++ < 100) delay_us(1); //����bit��ʼ�͵�ƽ
    if(retry >= 100) return 2; // �͵�ƽʱ���������ȡʧ��
    delay_us(30);  
    flag = ONELINE_IN; // ��ʱ30us�� ��ȡ��ƽ�ߵ�
    retry = 0;
    while(ONELINE_IN && retry++ < 100) delay_us(1); // �����ߵ�ƽʱ��
    return flag;
}
u8 Oneline_Read(){ // ��ȡbyte����
    u8 i, bit, byte=0;
    if(error) return 0;
    for(i=0;i<8;i++){ // ѭ����ȡbit����
        bit = Oneline_bit();
        if(bit > 1) {
            error = 1;
            return 0;
        }
        byte <<= 1; byte |= bit;
    }
    return byte;
}
void DHT11_Read(){// ��ȡ��ʪ����������
    u8 tem, tem_deci, humid, humid_deci, crc ,crc_res;
    if(Oneline_Start()){
        humid = Oneline_Read();
        humid_deci = Oneline_Read();
        tem = Oneline_Read();
        tem_deci = Oneline_Read();
        crc = Oneline_Read();
        crc_res = crc == (humid+humid_deci+tem+tem_deci); // ��У��
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
