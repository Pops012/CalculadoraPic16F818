// I2C_master.h
#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <xc.h>

#define I2C_SCL_TRIS TRISAbits.TRISA0
#define I2C_SDA_TRIS TRISAbits.TRISA1
#define I2C_SCL PORTAbits.RA0
#define I2C_SDA PORTAbits.RA1

void I2C_Master_Init(void);
void I2C_Master_Start(void);
void I2C_Master_Stop(void);
unsigned char I2C_Master_Write(unsigned char data);
unsigned char I2C_Master_Send(unsigned char data);
unsigned char I2C_Master_Send_Int(int data);

#endif