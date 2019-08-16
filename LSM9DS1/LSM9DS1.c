#include "LSM9DS1.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"
#include "nucleo_f401re_bus.h"
extern I2C_HandleTypeDef hi2c3;

extern  HAL_StatusTypeDef status;
#define ADDR 0x3D
void init_LSM9DS1_I2C (void)
{
	//Axel - Gyro
	uint8_t data[1];
	data[0] = 0x81;
	status = HAL_I2C_Mem_Write(&hi2c3, 0xD6, 0x20, I2C_MEMADD_SIZE_8BIT , data, 0x01, 100);//CTRL_REG8 RESET 
	HAL_Delay(10);
	status = HAL_I2C_Mem_Read(&hi2c3, 0xD6, 0x0F, I2C_MEMADD_SIZE_8BIT , data, 0x01, 100);//WHO_AM_I
	data[0] = 0xC3;
	HAL_I2C_Mem_Write(&hi2c3, 0xD6, 0x10, I2C_MEMADD_SIZE_8BIT , data, 0x01, 100);//952Hz ODR
	data[0] = 0xC3;
	HAL_I2C_Mem_Write(&hi2c3, 0xD6, 0x12, I2C_MEMADD_SIZE_8BIT , data, 0x01, 100);//952Hz ODR
	data[0] = 0x38;
	HAL_I2C_Mem_Write(&hi2c3, 0xD6, 0x1E,I2C_MEMADD_SIZE_8BIT , data, 0x01, 100);//CTRL_REG4 Gyro EN
	data[0] = 0x38;
  HAL_I2C_Mem_Write(&hi2c3, 0xD6, 0x1F, I2C_MEMADD_SIZE_8BIT , data, 0x01, 100);//CTRL_REG5 Axel EN
	data[0] = 0xC0;
	status = HAL_I2C_Mem_Write(&hi2c3, 0xD6, 0x20, I2C_MEMADD_SIZE_8BIT , data, 0x01, 100);//CTRL_REG8 RESET 
	//data[0] = 0x01;
//	status = HAL_I2C_Mem_Write(&hi2c3, 0xD6, 0x0C, I2C_MEMADD_SIZE_8BIT , data, 0x01, 100);//CTRL_REG8 RESET 
  HAL_Delay(1);
	//Magn
	data[0] = 0x04;
	status = HAL_I2C_Mem_Write(&hi2c3, ADDR, 0x21, I2C_MEMADD_SIZE_8BIT, data, 0x01, 100);//CTRL_REG2_M RESET 
	HAL_Delay(10);
	data[0] = 0x04;
	status = HAL_I2C_Mem_Write(&hi2c3, ADDR, 0x21,I2C_MEMADD_SIZE_8BIT, data, 0x01, 100);//CTRL_REG2_M RESET 
	HAL_Delay(10);
	data[0] = 0x60;
	status = HAL_I2C_Mem_Write(&hi2c3, ADDR, 0x21,I2C_MEMADD_SIZE_8BIT, data, 0x01, 100);//CTRL_REG2_M 
	
	status = HAL_I2C_Mem_Read(&hi2c3, ADDR, 0x0F, I2C_MEMADD_SIZE_8BIT, data, 0x01, 100);//WHO_AM_I
//HAL_Delay(10);
	data[0] = 0x7E;
	status = HAL_I2C_Mem_Write(&hi2c3, ADDR, 0x20,I2C_MEMADD_SIZE_8BIT, data, 0x01, 100);//CTRL_REG1_M Axel 
	//HAL_Delay(10);
	
	data[0] = 0x00;
	status = HAL_I2C_Mem_Write(&hi2c3, ADDR, 0x22,I2C_MEMADD_SIZE_8BIT, data, 0x01, 100);//CTRL_REG3_M Continuous-conversion mode
	//HAL_Delay(10);
	data[0] = 0x0C;
	status = HAL_I2C_Mem_Write(&hi2c3, 0x3D, 0x23,I2C_MEMADD_SIZE_8BIT, data, 0x01, 100);//CTRL_REG4_M  Ultra-high performance mode
//	HAL_Delay(10);
}


