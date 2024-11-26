// I2C_slave.h
#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <xc.h>

void I2C_Slave_Init(unsigned char address);
volatile int received_int = 0;
volatile unsigned char byte_count = 0;
volatile unsigned char data_ready = 0;

#endif