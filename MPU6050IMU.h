#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "mbed.h"
#include "rtos.h"
#include "Initializable.h"

#define MPU6050_ADDRESS 0x68 << 1
#define XA_OFFSET_H 0x06 // User-defined trim values for accelerometer
#define XA_OFFSET_L_TC 0x07
#define YA_OFFSET_H 0x08
#define YA_OFFSET_L_TC 0x09
#define ZA_OFFSET_H 0x0A
#define ZA_OFFSET_L_TC 0x0B
#define XG_OFFS_USRH 0x13 // User-defined trim values for gyroscope; supported in MPU-6050?
#define XG_OFFS_USRL 0x14
#define YG_OFFS_USRH 0x15
#define YG_OFFS_USRL 0x16
#define ZG_OFFS_USRH 0x17
#define ZG_OFFS_USRL 0x18
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define FIFO_EN 0x23
#define I2C_MST_CTRL 0x24
#define MPU6050_INT_PIN_CFG 0x37

#define INT_ENABLE 0x38
#define INT_STATUS 0x3A
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define USER_CTRL 0x6A
#define PWR_MGMT_1 0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2 0x6C

#define FIFO_COUNTH 0x72
#define FIFO_COUNTL 0x73
#define FIFO_R_W 0x74
#define MPU6050_WHO_AM_I_REG 0x75

#define MPU6050_SLP_BIT 6
#define MPU6050_BYPASS_BIT 1

// #define MPU6050_BW_256 0
// #define MPU6050_BW_188 1
// #define MPU6050_BW_98 2
// #define MPU6050_BW_42 3
// #define MPU6050_BW_20 4
// #define MPU6050_BW_10 5
// #define MPU6050_BW_5 6

#define MPU6050_ACCELERO_RANGE_2G 0
#define MPU6050_ACCELERO_RANGE_4G 1
#define MPU6050_ACCELERO_RANGE_8G 2
#define MPU6050_ACCELERO_RANGE_16G 3

#define MPU6050_GYRO_RANGE_250 0
#define MPU6050_GYRO_RANGE_500 1
#define MPU6050_GYRO_RANGE_1000 2
#define MPU6050_GYRO_RANGE_2000 3

class MPU6050 : public Initializable{
public:
    MPU6050(I2C *i2c);
    MPU6050(PinName sda, PinName scl);
    ~MPU6050(void);
    uint8_t testConnection(void);
    void readAccelData(int16_t *destAccel);
    void readGyroData(int16_t *destGyro);
    void updateData(void);

    float accelBias[3], accelData[3];
    float gyroBias[3], gyroData[3];

protected:
    void initialization_sequence() override;
    void reset_sequence() override;

private:
    I2C *mpuI2C;

    void init(void);
    void calibrate(void);
    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
    uint8_t readByte(uint8_t address, uint8_t subAddress);
    void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest);

    float aRes, gRes;
};

#endif