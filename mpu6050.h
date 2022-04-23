#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "mbed.h"
#include "rtos.h"

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

class MPU6050{
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

private:
    I2C *mpuI2C;

    void init(void);
    void calibrate(void);
    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
    uint8_t readByte(uint8_t address, uint8_t subAddress);
    void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest);

    float aRes, gRes;
};

//-------------------------------------------------------------------------------------------------------------------------------
// Public Methods
//-------------------------------------------------------------------------------------------------------------------------------

MPU6050::MPU6050(I2C *i2c) : mpuI2C(i2c)
{
    init();
}

MPU6050::MPU6050(PinName sda, PinName scl)
{
    mpuI2C = new I2C(sda, scl);
    init();
}

MPU6050::~MPU6050(void)
{
    if (mpuI2C != NULL)
        delete mpuI2C;
}

uint8_t MPU6050::testConnection(void)
{
    return readByte(MPU6050_ADDRESS, MPU6050_WHO_AM_I_REG);
}

void MPU6050::readAccelData(int16_t *destAccel)
{
    uint8_t rawData[6];
    readBytes(MPU6050_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);
    destAccel[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]);
    destAccel[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]);
    destAccel[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]);
}
void MPU6050::readGyroData(int16_t *destGyro)
{
    uint8_t rawData[6];
    readBytes(MPU6050_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);
    destGyro[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]);
    destGyro[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]);
    destGyro[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]);
}

void MPU6050::updateData(void)
{
    if (readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01)
    {
        int16_t accelCount[3];
        int16_t gyroCount[3];
        readAccelData(accelCount);
        accelData[0] = (float)accelCount[0] * aRes - accelBias[0];
        accelData[1] = (float)accelCount[1] * aRes - accelBias[1];
        accelData[2] = (float)accelCount[2] * aRes - accelBias[2];

        readGyroData(gyroCount);
        gyroData[0] = (float)gyroCount[0] * gRes - gyroBias[0];
        gyroData[1] = (float)gyroCount[1] * gRes - gyroBias[1];
        gyroData[2] = (float)gyroCount[2] * gRes - gyroBias[2];
    }
}
//-------------------------------------------------------------------------------------------------------------------------------
//            Private Methods
//-------------------------------------------------------------------------------------------------------------------------------

void MPU6050::init(void)
{
    int Gscale = MPU6050_GYRO_RANGE_250;
    int Ascale = MPU6050_ACCELERO_RANGE_2G;
    aRes = 2.0 / 32768.0;
    gRes = 250.0 / 32768.0;

    mpuI2C->frequency(400000);

    // get stable time source
    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);
    ThisThread::sleep_for(100ms);

    writeByte(MPU6050_ADDRESS, PWR_MGMT_2, 0x01);
    writeByte(MPU6050_ADDRESS, CONFIG, 0x03);
    writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x04);

    uint8_t c = readByte(MPU6050_ADDRESS, GYRO_CONFIG);
    writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c & ~0xE0);       // Clear self-test bits [7:5]
    writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c & ~0x18);       // Clear AFS bits [4:3]
    writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c | Gscale << 3); // Set full scale range for the gyro

    c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
    writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0xE0);       // Clear self-test bits [7:5]
    writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x18);       // Clear AFS bits [4:3]
    writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c | Ascale << 3); // Set full scale range for the accelerometer

    calibrate();
}

void MPU6050::calibrate(void)
{
    uint8_t data[12];
    uint16_t packet_count, fifo_count;
    int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
    ThisThread::sleep_for(100ms);

    // get stable time source
    // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);
    writeByte(MPU6050_ADDRESS, PWR_MGMT_2, 0x00);
    ThisThread::sleep_for(200ms);

    // Configure device for bias calculation
    writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
    writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
    writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
    writeByte(MPU6050_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
    writeByte(MPU6050_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
    writeByte(MPU6050_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
    ThisThread::sleep_for(15ms);

    // Configure MPU6050 gyro and accelerometer for bias calculation
    writeByte(MPU6050_ADDRESS, CONFIG, 0x01);       // Set low-pass filter to 188 Hz
    writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x00);   // Set sample rate to 1 kHz
    writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
    writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

    uint16_t gyrosensitivity = 131;    // = 131 LSB/degrees/sec
    uint16_t accelsensitivity = 16384; // = 16384 LSB/g

    // Configure FIFO to capture accelerometer and gyro data for bias calculation
    writeByte(MPU6050_ADDRESS, USER_CTRL, 0x40); // Enable FIFO
    writeByte(MPU6050_ADDRESS, FIFO_EN, 0x78);   // Enable gyro and accelerometer sensors for FIFO  (max size 1024 bytes in MPU-6050)
    ThisThread::sleep_for(80ms);                                 // accumulate 80 samples in 80 milliseconds = 960 bytes

    // At end of sample accumulation, turn off FIFO sensor read
    writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);            // Disable gyro and accelerometer sensors for FIFO
    readBytes(MPU6050_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read FIFO sample count
    fifo_count = ((uint16_t)data[0] << 8) | data[1];
    packet_count = fifo_count / 12; // How many sets of full gyro and accelerometer data for averaging
    for (uint16_t i = 0; i < packet_count; i++)
    {
        readBytes(MPU6050_ADDRESS, FIFO_R_W, 12, &data[0]);                     // read data for averaging
        accel_bias[0] += (int32_t)(int16_t)(((int16_t)data[0] << 8) | data[1]); // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
        accel_bias[1] += (int32_t)(int16_t)(((int16_t)data[2] << 8) | data[3]);
        accel_bias[2] += (int32_t)(int16_t)(((int16_t)data[4] << 8) | data[5]);
        gyro_bias[0] += (int32_t)(int16_t)(((int16_t)data[6] << 8) | data[7]);
        gyro_bias[1] += (int32_t)(int16_t)(((int16_t)data[8] << 8) | data[9]);
        gyro_bias[2] += (int32_t)(int16_t)(((int16_t)data[10] << 8) | data[11]);
    }
    accel_bias[0] /= (int32_t)packet_count; // Normalize sums to get average count biases
    accel_bias[1] /= (int32_t)packet_count;
    accel_bias[2] /= (int32_t)packet_count;
    gyro_bias[0] /= (int32_t)packet_count;
    gyro_bias[1] /= (int32_t)packet_count;
    gyro_bias[2] /= (int32_t)packet_count;

    if (accel_bias[2] > 0L)
        accel_bias[2] -= (int32_t)accelsensitivity; // Remove gravity from the z-axis accelerometer bias calculation
    else
        accel_bias[2] += (int32_t)accelsensitivity;

    // Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
    data[0] = (-gyro_bias[0] / 4 >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
    data[1] = (-gyro_bias[0] / 4) & 0xFF;      // Biases are additive, so change sign on calculated average gyro biases
    data[2] = (-gyro_bias[1] / 4 >> 8) & 0xFF;
    data[3] = (-gyro_bias[1] / 4) & 0xFF;
    data[4] = (-gyro_bias[2] / 4 >> 8) & 0xFF;
    data[5] = (-gyro_bias[2] / 4) & 0xFF;

    // Push gyro biases to hardware registers
    writeByte(MPU6050_ADDRESS, XG_OFFS_USRH, data[0]);
    writeByte(MPU6050_ADDRESS, XG_OFFS_USRL, data[1]);
    writeByte(MPU6050_ADDRESS, YG_OFFS_USRH, data[2]);
    writeByte(MPU6050_ADDRESS, YG_OFFS_USRL, data[3]);
    writeByte(MPU6050_ADDRESS, ZG_OFFS_USRH, data[4]);
    writeByte(MPU6050_ADDRESS, ZG_OFFS_USRL, data[5]);

    gyroBias[0] = (float)gyro_bias[0] / (float)gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
    gyroBias[1] = (float)gyro_bias[1] / (float)gyrosensitivity;
    gyroBias[2] = (float)gyro_bias[2] / (float)gyrosensitivity;

    int32_t accel_bias_reg[3] = {0, 0, 0};                // A place to hold the factory accelerometer trim biases
    readBytes(MPU6050_ADDRESS, XA_OFFSET_H, 2, &data[0]); // Read factory accelerometer trim values
    accel_bias_reg[0] = (int16_t)((int16_t)data[0] << 8) | data[1];
    readBytes(MPU6050_ADDRESS, YA_OFFSET_H, 2, &data[0]);
    accel_bias_reg[1] = (int16_t)((int16_t)data[0] << 8) | data[1];
    readBytes(MPU6050_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
    accel_bias_reg[2] = (int16_t)((int16_t)data[0] << 8) | data[1];

    uint32_t mask = 1uL;             // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
    uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

    for (uint8_t i = 0; i < 3; i++)
    {
        if (accel_bias_reg[i] & mask)
            mask_bit[i] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
    }

    // Construct total accelerometer bias, including calculated average accelerometer bias from above
    accel_bias_reg[0] -= (accel_bias[0] / 8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
    accel_bias_reg[1] -= (accel_bias[1] / 8);
    accel_bias_reg[2] -= (accel_bias[2] / 8);

    data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
    data[1] = (accel_bias_reg[0]) & 0xFF;
    data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
    data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
    data[3] = (accel_bias_reg[1]) & 0xFF;
    data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
    data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
    data[5] = (accel_bias_reg[2]) & 0xFF;
    data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

    writeByte(MPU6050_ADDRESS, XA_OFFSET_H, data[0]);
    writeByte(MPU6050_ADDRESS, XA_OFFSET_L_TC, data[1]);
    writeByte(MPU6050_ADDRESS, YA_OFFSET_H, data[2]);
    writeByte(MPU6050_ADDRESS, YA_OFFSET_L_TC, data[3]);
    writeByte(MPU6050_ADDRESS, ZA_OFFSET_H, data[4]);
    writeByte(MPU6050_ADDRESS, ZA_OFFSET_L_TC, data[5]);

    accelBias[0] = (float)accel_bias[0] / (float)accelsensitivity;
    accelBias[1] = (float)accel_bias[1] / (float)accelsensitivity;
    accelBias[2] = (float)accel_bias[2] / (float)accelsensitivity;
}

void MPU6050::writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
    char data_write[2];
    data_write[0] = subAddress;
    data_write[1] = data;
    mpuI2C->write(address, data_write, 2, 0);
}
uint8_t MPU6050::readByte(uint8_t address, uint8_t subAddress)
{
    char data[1];
    char data_write[1];
    data_write[0] = subAddress;
    mpuI2C->write(address, data_write, 1, 1);
    mpuI2C->read(address, data, 1, 0);
    return data[0];
}

void MPU6050::readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest){
    char data[14];
    char data_write[1];
    data_write[0] = subAddress;
    mpuI2C->write(address, data_write, 1, 1);
    mpuI2C->read(address, data, count, 0);
    for (uint8_t i = 0; i < count; i++)
        dest[i] = data[i];
}

#endif