#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */
// Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */

// create a task that inits and reads ADS in a loop
void adc_task(void *pvParameter)
{
    Serial.println("Getting single-ended readings from AIN0..3");
    Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");

    // The ADC input range (or gain) can be changed via the following
    // functions, but be careful never to exceed VDD +0.3V max, or to
    // exceed the upper and lower limits if you adjust the input range!
    // Setting these values incorrectly may destroy your ADC!
    //                                                                ADS1015  ADS1115
    //                                                                -------  -------
    // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
    // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
    // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

    // set i2c speed to 3.4ghz
    Wire.setClock(3400000);

    if (!ads.begin())
    {
        Serial.println("Failed to initialize ADS.");
        while (1)
            ;
    }

    ads.setGain(GAIN_ONE); // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    ads.setDataRate(RATE_ADS1115_860SPS);

    uint8_t channel = 0;
    float adc_chanels[4] = {0, 0, 0, 0};
    while (1)
    {
        unsigned long start_time = millis();

        for (uint8_t ch = 0; ch < 4; ch++)
        {
            ads.startADCReading(MUX_BY_CHANNEL[ch], false);
            while (!ads.conversionComplete())
            {
                delay(1);
            }
            adc_chanels[ch] = ads.computeVolts(ads.getLastConversionResults());
        }
        uint32_t period = millis() - start_time;

        //print period every second
        static unsigned long last_time = 0;
        if (millis() - last_time > 1000)
        {
            Serial.printf("period: %d, value %f, %f, %f, %f\n", period, adc_chanels[0], adc_chanels[1], adc_chanels[2], adc_chanels[3]);
            last_time = millis();
        }
        delay(3);
    }
}