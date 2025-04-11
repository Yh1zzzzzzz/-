#include "AD1674.h"
#include "main.h"

/**
 * @brief ��ʼ��AD1674 ADC
 */
void AD1674_Init(void)
{
    // ����CS����Ϊ�ߵ�ƽ����ѡ��״̬��
    HAL_GPIO_WritePin(AD1674_CS_GPIO_Port, AD1674_CS_Pin, GPIO_PIN_SET);
    
    // ����CE����Ϊ�ߵ�ƽ����ʹ��״̬��
    HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
    
    // ����RC����Ϊ�ߵ�ƽ����ʼ״̬��
    HAL_GPIO_WritePin(RC_GPIO_Port, RC_Pin, GPIO_PIN_SET);
    
    // ����A0����Ϊ�ߵ�ƽ����ʼ״̬������Ϊ12λģʽ��
    HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_SET);

    //  ����I2_8����Ϊ�ߵ�ƽ����ʼ״̬��
    HAL_GPIO_WritePin(I2_8_GPIO_Port, I2_8_Pin, GPIO_PIN_SET);
    
}

/**
 * @brief ����ADCת��
 */
void AD1674_StartConversion(void)
{
    // ѡ��оƬ
    HAL_GPIO_WritePin(AD1674_CS_GPIO_Port, AD1674_CS_Pin, GPIO_PIN_RESET);
    
    // оƬʹ��
    HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);
    
    // ����ת�� - �����½������嵽RC����
    HAL_GPIO_WritePin(RC_GPIO_Port, RC_Pin, GPIO_PIN_RESET);
    HAL_Delay(1); // ������ʱ
    HAL_GPIO_WritePin(RC_GPIO_Port, RC_Pin, GPIO_PIN_SET);
}

/**
 * @brief ���ת���Ƿ����
 * @return 1 - ת����ɣ�0 - ת����
 */
uint8_t AD1674_IsConversionDone(void)
{
    // ��STS����Ϊ�͵�ƽʱ����ʾת�����
    return (HAL_GPIO_ReadPin(STS_GPIO_Port, STS_Pin) == GPIO_PIN_RESET) ? 1 : 0;
}

/**
 * @brief ��ȡת�����
 * @return 12λADCֵ
 */
uint16_t AD1674_ReadValue(void)
{
    uint16_t adcValue = 0;
    
    // ��ȡ��8λ����
    adcValue |= HAL_GPIO_ReadPin(DB7_GPIO_Port, DB7_Pin) << 11;
    adcValue |= HAL_GPIO_ReadPin(DB6_GPIO_Port, DB6_Pin) << 10;
    adcValue |= HAL_GPIO_ReadPin(DB5_GPIO_Port, DB5_Pin) << 9;
    adcValue |= HAL_GPIO_ReadPin(DB4_GPIO_Port, DB4_Pin) << 8;
    adcValue |= HAL_GPIO_ReadPin(DB3_GPIO_Port, DB3_Pin) << 7;
    adcValue |= HAL_GPIO_ReadPin(DB2_GPIO_Port, DB2_Pin) << 6;
    adcValue |= HAL_GPIO_ReadPin(DB1_GPIO_Port, DB1_Pin) << 5;
    adcValue |= HAL_GPIO_ReadPin(DB0_GPIO_Port, DB0_Pin) << 4;
    
    // �л���ַ�Զ�ȡ��4λ
    HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_RESET);
    
    // ������ʱ
    HAL_Delay(1);
    
    // ��ȡ��4λ����
    adcValue |= HAL_GPIO_ReadPin(DB7_GPIO_Port, DB7_Pin) << 3;
    adcValue |= HAL_GPIO_ReadPin(DB6_GPIO_Port, DB6_Pin) << 2;
    adcValue |= HAL_GPIO_ReadPin(DB5_GPIO_Port, DB5_Pin) << 1;
    adcValue |= HAL_GPIO_ReadPin(DB4_GPIO_Port, DB4_Pin);
    
    // �ָ�A0Ϊ�ߵ�ƽ
    HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_SET);
    
    // ȡ��оƬʹ��
    HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
    
    // ȡ��оƬѡ��
    HAL_GPIO_WritePin(AD1674_CS_GPIO_Port, AD1674_CS_Pin, GPIO_PIN_SET);
    
    return adcValue;
}

/**
 * @brief ����ADC��ȡ���� - ����ת�����ȴ����
 * @return 12λADCֵ
 */
uint16_t AD1674_ReadADC(void)
{
    // ����ת��
    AD1674_StartConversion();
    
    // �ȴ�ת�����
    while (!AD1674_IsConversionDone());
    
    // ��ȡ�����ؽ��
    return AD1674_ReadValue();
}