/*
 * File:   i2c.c
 * Author: vomnes
 *
 * Created on May 8, 2018, 2:00 PM
 */

#include "types.h"

void Start_I2CI()
{
    I2C1CONbits.SEN = 1; // Start condition enabled
}

void Idle_I2CI()
{
    /* Wait process complete */
    while (I2C1CONbits.SEN || I2C1STATbits.TBF || I2C1CONbits.PEN ||
	   I2C1CONbits.RCEN || I2C1CONbits.ACKEN) ;
}

s8 Master_Write_I2C1(u8 data)
{
    while (I2C1STATbits.TRSTAT); /* Wait master transmit is finished */
    I2C1TRN = data; /* Set data in Transmit Data Register */
    while (I2C1STATbits.TRSTAT || I2C1STATbits.TBF); /* Wait master transmit is finished */
    return (I2C1STATbits.ACKSTAT);/* Check if Acknowledge Status bit was received, no error  */
}

void Stop_I2CI()
{
    I2C1CONbits.PEN = 1;
}

s8 Master_Read_I2C1()
{
    while (I2C1STATbits.TRSTAT); /* Wait master transmit is finished */
    I2C1CONbits.RCEN = 1; /* Received bit enabled */
    while (I2C1CONbits.RCEN) ; /* Waiting for receive the 8 bits */
    I2C1STATbits.I2COV = 0;
    return I2C1RCV;
}

void I2C1_Init()
{
    I2C1CON = 0;
    I2C1CONbits.A10M = 0; // 7-bit slave address
    I2C1BRG = BAUD_RATE; // Set baud rate
    I2C1CONbits.ON = 1;
}

void I2C1_Send_Data(u8 data, u8 addr)
{
    StartI2CI();
    IdleI2CI(); // SEN
    MasterWriteI2C1((addr << 1) | WRITE_CMD);
    //while(MasterWriteI2C1((addr << 1) | WRITE_CMD));
    IdleI2CI(); // TBF - Transmit Buffer Full
    MasterWriteI2C1(data);
    //while(MasterWriteI2C1(data));
    StopI2CI();
    IdleI2CI(); // PEN - Stop Condition Enabled
}

u8 I2C1_Receive_Data(u8 addr)
{
    s8 data;
    StartI2CI();
    IdleI2CI(); // SEN
    MasterWriteI2C1((addr << 1) | READ_CMD);
    //while(MasterWriteI2C1((addr << 1) | READ_CMD));
    IdleI2CI(); // TBF - Transmit Buffer Full
    data = MasterReadI2C1();
    StopI2CI();
    IdleI2CI(); // TBF - Transmit Buffer Full
    return data;
}
