#ifndef SCD4X_I2C_H
#define SCD4X_I2C_H


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//#include "sensirion_config.h"
//#define NO_ERROR 0
/**
 * scd4x_start_periodic_measurement() - start periodic measurement, signal
 * update interval is 5 seconds.
 *
 * @note This command is only available in idle mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_start_periodic_measurement(void);

/**
 * scd4x_read_measurement_ticks() - read sensor output. The measurement data can
 * only be read out once per signal update interval as the buffer is emptied
 * upon read-out. If no data is available in the buffer, the sensor returns a
 * NACK. To avoid a NACK response the get_data_ready_status can be issued to
 * check data status. The I2C master can abort the read transfer with a NACK
 * followed by a STOP condition after any data byte if the user is not
 * interested in subsequent data.
 *
 * @note This command is only available in measurement mode. The firmware
 * updates the measurement values depending on the measurement mode.
 *
 * @param co2 COâ‚‚ concentration in ppm
 *
 * @param temperature Convert value to Â°C by: -45 Â°C + 175 Â°C * value/2^16
 *
 * @param humidity Convert value to %RH by: 100%RH * value/2^16
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_read_measurement_ticks(uint16_t* co2, uint16_t* temperature,
                                     uint16_t* humidity);

/**
 * scd4x_read_measurement() - read sensor output and convert.
 * See @ref scd4x_read_measurement_ticks() for more details.
 *
 * @note This command is only available in measurement mode. The firmware
 * updates the measurement values depending on the measurement mode.
 *
 * @param co2 COâ‚‚ concentration in ppm
 *
 * @param temperature_m_deg_c Temperature in milli degrees celsius (Â°C * 1000)
 *
 * @param humidity_m_percent_rh Relative humidity in milli percent RH
 * (%RH * 1000)
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_read_measurement(uint16_t* co2, int32_t* temperature_m_deg_c,
                               int32_t* humidity_m_percent_rh);

/**
 * scd4x_stop_periodic_measurement() - Stop periodic measurement and return to
 * idle mode for sensor configuration or to safe energy.
 *
 * @note This command is only available in measurement mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_stop_periodic_measurement(void);

/**
 * scd4x_get_temperature_offset_ticks() - The temperature offset represents the
 * difference between the measured temperature by the SCD4x and the actual
 * ambient temperature. Per default, the temperature offset is set to 4Â°C.
 *
 * @note Only available in idle mode.
 *
 * @param t_offset Temperature offset. Convert value to Â°C by: 175 * value /
 * 2^16
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_get_temperature_offset_ticks(uint16_t* t_offset);

/**
 * scd4x_get_temperature_offset() - The temperature offset represents the
 * difference between the measured temperature by the SCD4x and the actual
 * ambient temperature. Per default, the temperature offset is set to 4Â°C.
 *
 * @note Only available in idle mode.
 *
 * @param t_offset_m_deg_c Temperature offset in milli degrees Celsius.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_get_temperature_offset(int32_t* t_offset_m_deg_c);

/**
 * scd4x_set_temperature_offset_ticks() - Setting the temperature offset of the
 * SCD4x inside the customer device correctly allows the user to leverage the RH
 * and T output signal. Note that the temperature offset can depend on various
 * factors such as the SCD4x measurement mode, self-heating of close components,
 * the ambient temperature and air flow. Thus, the SCD4x temperature offset
 * should  be determined inside the customer device under its typical operation
 * and in thermal equilibrium.
 *
 * @note Only available in idle mode.
 *
 * @param t_offset Temperature offset. Convert Â°C to value by: T * 2^16 / 175.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_set_temperature_offset_ticks(uint16_t t_offset);

/**
 * scd4x_set_temperature_offset() - Setting the temperature offset of the SCD4x
 * inside the customer device correctly allows the user to leverage the RH and T
 * output signal. Note that the temperature offset can depend on various factors
 * such as the SCD4x measurement mode, self-heating of close components, the
 * ambient temperature and air flow. Thus, the SCD4x temperature offset should
 * be determined inside the customer device under its typical operation and in
 * thermal equilibrium.
 *
 * @note Only available in idle mode.
 *
 * @param t_offset_m_deg_c Temperature offset in milli degrees Celsius.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_set_temperature_offset(int32_t t_offset_m_deg_c);

/**
 * scd4x_get_sensor_altitude() - Get configured sensor altitude in meters above
 * sea level. Per default, the sensor altitude is set to 0 meter above
 * sea-level.
 *
 * @note Only available in idle mode.
 *
 * @param sensor_altitude Sensor altitude in meters.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_get_sensor_altitude(uint16_t* sensor_altitude);

/**
 * scd4x_set_sensor_altitude() - Set sensor altitude in meters above sea level.
 * Note that setting a sensor altitude to the sensor overrides any pressure
 * compensation based on a previously set ambient pressure.
 *
 * @note Only available in idle mode.
 *
 * @param sensor_altitude Sensor altitude in meters.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_set_sensor_altitude(uint16_t sensor_altitude);

/**
 * scd4x_set_ambient_pressure() - The set_ambient_pressure command can be sent
 * during periodic measurements to enable continuous pressure compensation. Note
 * that setting an ambient pressure to the sensor overrides any pressure
 * compensation based on a previously set sensor altitude.
 *
 * @note Available during measurements.
 *
 * @param ambient_pressure Ambient pressure in hPa. Convert value to Pa by:
 * value * 100.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_set_ambient_pressure(uint16_t ambient_pressure);

/**
 * scd4x_perform_forced_recalibration() - To successfully conduct an accurate
forced recalibration, the following steps need to be carried out:
1. Operate the SCD4x in a periodic measurement mode for > 3 minutes in an
environment with homogenous and constant COâ‚‚ concentration.
2. Stop periodic measurement. Wait 500 ms.
3. Subsequently issue the perform_forced_recalibration command and optionally
read out the baseline correction. A return value of 0xffff indicates that the
forced recalibration failed.
 *
 * @param target_co2_concentration Target COâ‚‚ concentration in ppm.
 *
 * @param frc_correction FRC correction value in COâ‚‚ ppm or 0xFFFF if the
command failed. Convert value to COâ‚‚ ppm with: value - 0x8000
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_perform_forced_recalibration(uint16_t target_co2_concentration,
                                           uint16_t* frc_correction);

/**
 * scd4x_get_automatic_self_calibration() - By default, the ASC is enabled.
 *
 * @param asc_enabled 1 if ASC is enabled, 0 if ASC is disabled
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_get_automatic_self_calibration(uint16_t* asc_enabled);

/**
 * scd4x_set_automatic_self_calibration() - By default, the ASC is enabled.
 *
 * @param asc_enabled 1 to enable ASC, 0 to disable ASC
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_set_automatic_self_calibration(uint16_t asc_enabled);

/**
 * scd4x_start_low_power_periodic_measurement() - Start low power periodic
 * measurement, signal update interval is 30 seconds.
 *
 * @note This command is only available in idle mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_start_low_power_periodic_measurement(void);

/**
 * scd4x_get_data_ready_status() - Check whether new measurement data is
 * available for read-out.
 *
 * @param data_ready If last 11 bits are 0 data not ready, else data ready
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_get_data_ready_status(uint16_t* data_ready);

/**
 * scd4x_persist_settings() - Configuration settings such as the temperature
 * offset, sensor altitude and the ASC enabled/disabled parameter are by default
 * stored in the volatile memory (RAM) only and will be lost after a
 * power-cycle. The persist_settings command stores the current configuration in
 * the EEPROM of the SCD4x, making them resistant to power-cycling. To avoid
 * unnecessary wear of the EEPROM, the persist_settings command should only be
 * sent when persistence is required and if actual changes to the configuration
 * have been made. Note that field calibration history (i.e. FRC and ASC) is
 * stored in the EEPROM automatically.
 *
 * @note
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_persist_settings(void);

/**
 * scd4x_get_serial_number() - Reading out the serial number can be used to
 * identify the chip and to verify the presence of the sensor. The get serial
 * number command returns 3 words.  Together, the 3 words constitute a unique
 * serial number with a length of 48 bits (big endian format).
 *
 * @param serial_0 First word of the 48 bit serial number
 *
 * @param serial_1 Second word of the 48 bit serial number
 *
 * @param serial_2 Third word of the 48 bit serial number
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_get_serial_number(uint16_t* serial_0, uint16_t* serial_1,
                                uint16_t* serial_2);

/**
 * scd4x_perform_self_test() - The perform_self_test feature can be used as an
 * end-of-line test to confirm sensor functionality.
 *
 * @param sensor_status 0 means no malfunction detected
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_perform_self_test(uint16_t* sensor_status);

/**
 * scd4x_perform_factory_reset() - Initiates the reset of all configurations
 * stored in the EEPROM and erases the FRC and ASC algorithm history.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_perform_factory_reset(void);

/**
 * scd4x_reinit() - The reinit command reinitializes the sensor by reloading
 * user settings from EEPROM. Before sending the reinit command, the stop
 * measurement command must be issued. If reinit command does not trigger the
 * desired re-initialization, a power-cycle should be applied to the SCD4x.
 *
 * @note Only available in idle mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_reinit(void);

/**
 * scd4x_measure_single_shot() - On-demand measurement of COâ‚‚ concentration,
 * relative humidity and temperature. The sensor output is read with the
 * read_measurement command.
 *
 * @note Only available in idle mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_measure_single_shot(void);

/**
 * scd4x_measure_single_shot_rht_only() - On-demand measurement of relative
 * humidity and temperature only.
 *
 * @note Only available in idle mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_measure_single_shot_rht_only(void);

/**
 * scd4x_power_down() - Put the sensor from idle to sleep mode to reduce current
 * consumption.
 *
 * @note Only available in idle mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_power_down(void);

/**
 * scd4x_wake_up() - Wake up sensor from sleep mode to idle mode.
 *
 * @note Only available in sleep mode.
 *
 * @return 0 on success, an error code otherwise
 */
int16_t scd4x_wake_up(void);


#endif /* SCD4X_I2C_H */
