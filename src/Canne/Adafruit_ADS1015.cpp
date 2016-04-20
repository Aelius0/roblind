#include "Adafruit_ADS1015.h"
#include<ctime>


/**************************************************************************/
/*!
    @brief  Instantiates a new ADS1015 class w/appropriate properties
*/
/**************************************************************************/
Adafruit_ADS1015::Adafruit_ADS1015(unsigned char i2cAddress)
{
   //m_fd=wiringPiI2CSetup(0x48);//adress ads1015
   m_fd = wiringPiI2CSetup(0x48);
   m_i2cAddress = i2cAddress;
   m_conversionDelay = ADS1015_CONVERSIONDELAY;
   m_bitShift = 4;
   m_gain = GAIN_ONE; /* +/- 2.144V range (limited to VDD +0.3V max!) */
}

/**************************************************************************/
/*!
    @brief  Instantiates a new ADS1115 class w/appropriate properties
*/
/**************************************************************************/
Adafruit_ADS1115::Adafruit_ADS1115(unsigned char i2cAddress)
{
   m_i2cAddress = i2cAddress;
   m_conversionDelay = ADS1115_CONVERSIONDELAY;
   m_bitShift = 0;
   m_gain = GAIN_TWO; /* +/- 2.144V range (limited to VDD +0.3V max!) */
}


/**************************************************************************/
/*!
    @brief  Sets the gain and input voltage range
*/
/**************************************************************************/
void Adafruit_ADS1015::setGain(adsGain_t gain)
{
  m_gain = gain;
}

/**************************************************************************/
/*!
    @brief  Gets a gain and input voltage range
*/
/**************************************************************************/
adsGain_t Adafruit_ADS1015::getGain()
{
  return m_gain;
}

/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel
*/
/**************************************************************************/
 short Adafruit_ADS1015::readADC_SingleEnded(unsigned char channel)
 {
  unsigned short valeur=0;

  if (channel > 3)
  {
    return 0;
  }
  
  // Start with default values
  unsigned short config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= m_gain;

  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;
  unsigned short poidsFort=config>>8;
  unsigned short poidsFaible=config&0x00FF;
  config=poidsFaible<<8;
  config=config|poidsFort;



  // Write config register to the ADC

  wiringPiI2CWriteReg16(m_fd, ADS1015_REG_POINTER_CONFIG ,config);
    usleep(10000);
  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1015

 valeur= (unsigned short)wiringPiI2CReadReg16(m_fd, ADS1015_REG_POINTER_CONVERT);
 poidsFaible=valeur>>8;
 poidsFort=valeur<<8;
 valeur=poidsFaible|poidsFort;
  valeur>>= m_bitShift;
  if(valeur>0x0800)
  {
      valeur*=-1;
      valeur-=1;
      valeur&=0x0FFF;

  }
  valeur*=2;    //en mV
  return (short) valeur;
}

/**************************************************************************/
/*! 
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN0) and N (AIN1) input.  Generates
            a signed value since the difference can be either
            positive or negative.
*/
/**************************************************************************/
unsigned short Adafruit_ADS1015::readADC_Differential_0_1() {
  // Start with default values
  unsigned short config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= m_gain;
                    
  // Set channels
  config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1;          // AIN0 = P, AIN1 = N

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  wiringPiI2CWriteReg16(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
   usleep(100);

  // Read the conversion results
  unsigned short res = wiringPiI2CReadReg16(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
  if (m_bitShift == 0)
  {
    return (short)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (short)res;
  }
}

/**************************************************************************/
/*! 
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN2) and N (AIN3) input.  Generates
            a signed value since the difference can be either
            positive or negative.
*/
/**************************************************************************/
unsigned short Adafruit_ADS1015::readADC_Differential_2_3() {
  // Start with default values
  unsigned short config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= m_gain;

  // Set channels
  config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3;          // AIN2 = P, AIN3 = N

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  wiringPiI2CWriteReg16(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  usleep(1000);

  // Read the conversion results
  unsigned short res = wiringPiI2CReadReg16(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
  if (m_bitShift == 0)
  {
    return (short)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (short)res;
  }
}

/**************************************************************************/
/*!
    @brief  Sets up the comparator to operate in basic mode, causing the
            ALERT/RDY pin to assert (go from high to low) when the ADC
            value exceeds the specified threshold.

            This will also set the ADC in continuous conversion mode.
*/
/**************************************************************************/
void Adafruit_ADS1015::startComparator_SingleEnded(unsigned char channel, unsigned short threshold)
{
  // Start with default values
  unsigned short config = ADS1015_REG_CONFIG_CQUE_1CONV   | // Comparator enabled and asserts on 1 match
                    ADS1015_REG_CONFIG_CLAT_LATCH   | // Latching mode
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_CONTIN  | // Continuous conversion mode
                    ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

  // Set PGA/voltage range
  config |= m_gain;
                    
  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set the high threshold register
  // Shift 12-bit results left 4 bits for the ADS1015
  wiringPiI2CWriteReg16(m_i2cAddress, ADS1015_REG_POINTER_HITHRESH, threshold << m_bitShift);

  // Write config register to the ADC
  wiringPiI2CWriteReg16(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
}

/**************************************************************************/
/*!
    @brief  In order to clear the comparator, we need to read the
            conversion results.  This function reads the last conversion
            results without changing the config value.
*/
/**************************************************************************/
unsigned short Adafruit_ADS1015::getLastConversionResults()
{
  // Wait for the conversion to complete
  sleep(m_conversionDelay);

  // Read the conversion results
  unsigned short res = wiringPiI2CReadReg16(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
  if (m_bitShift == 0)
  {
    return (short)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (short)res;
  }
}

