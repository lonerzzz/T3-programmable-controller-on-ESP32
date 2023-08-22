#include "scd4x_i2c.h"
#include "sensirion_common.h"
//#include "sensirion_i2c.h"
//#include "sensirion_i2c_hal.h"
#include "i2c_task.h"

#define SCD4X_I2C_ADDRESS 98

int16_t scd4x_start_periodic_measurement() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x21B1);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}
extern uint16_t Test[50];
int16_t scd4x_read_measurement_ticks(uint16_t* co2, uint16_t* temperature,
                                     uint16_t* humidity) {
    int16_t error = 0;
    uint8_t buffer[9];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0xEC05);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {Test[24]++;
        return error;
    }

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, buffer, 6);

    if (error) {Test[25]++;
        return error;
    }

    *co2 = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    *temperature = sensirion_common_bytes_to_uint16_t(&buffer[2]);
    *humidity = sensirion_common_bytes_to_uint16_t(&buffer[4]);
    return NO_ERROR;
}

int16_t scd4x_read_measurement(uint16_t* co2, int32_t* temperature_m_deg_c,
                               int32_t* humidity_m_percent_rh) {
    int16_t error;
    uint16_t temperature;
    uint16_t humidity;

    error = scd4x_read_measurement_ticks(co2, &temperature, &humidity);
    if (error) {
        return error;
    }
    *temperature_m_deg_c = ((21875 * (int32_t)temperature) >> 13) - 45000;
    *humidity_m_percent_rh = ((12500 * (int32_t)humidity) >> 13);
    return NO_ERROR;
}

int16_t scd4x_stop_periodic_measurement() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3F86);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(500000);
    return NO_ERROR;
}

int16_t scd4x_get_temperature_offset_ticks(uint16_t* t_offset) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x2318);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *t_offset = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_get_temperature_offset(int32_t* t_offset_m_deg_c) {
    int16_t error;
    uint16_t t_offset;

    error = scd4x_get_temperature_offset_ticks(&t_offset);
    if (error) {
        return error;
    }
    *t_offset_m_deg_c = ((21875 * (int32_t)t_offset) >> 13);
    return NO_ERROR;
}

int16_t scd4x_set_temperature_offset_ticks(uint16_t t_offset) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x241D);

    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset, t_offset);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}

int16_t scd4x_set_temperature_offset(int32_t t_offset_m_deg_c) {
    uint16_t t_offset = (uint16_t)((t_offset_m_deg_c * 12271) >> 15);
    return scd4x_set_temperature_offset_ticks(t_offset);
}

int16_t scd4x_get_sensor_altitude(uint16_t* sensor_altitude) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x2322);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *sensor_altitude = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_set_sensor_altitude(uint16_t sensor_altitude) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x2427);

    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset,
                                                  sensor_altitude);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}

int16_t scd4x_set_ambient_pressure(uint16_t ambient_pressure) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0xE000);

    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset,
                                                  ambient_pressure);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}

int16_t scd4x_perform_forced_recalibration(uint16_t target_co2_concentration,
                                           uint16_t* frc_correction) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    sensirion_i2c_hal_sleep_usec(500000);
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x362F);

    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset,
                                                  target_co2_concentration);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);

    if (error) {

        return error;
    }

    sensirion_i2c_hal_sleep_usec(400000);

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *frc_correction = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_get_automatic_self_calibration(uint16_t* asc_enabled) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x2313);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *asc_enabled = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_set_automatic_self_calibration(uint16_t asc_enabled) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    sensirion_i2c_hal_sleep_usec(1000);
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x2416);

    offset =
        sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset, asc_enabled);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}

int16_t scd4x_start_low_power_periodic_measurement() {
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x21AC);

    return sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
}

int16_t scd4x_get_data_ready_status(uint16_t* data_ready) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0xE4B8);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *data_ready = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_persist_settings() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3615);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(800000);
    return NO_ERROR;
}

int16_t scd4x_get_serial_number(uint16_t* serial_0, uint16_t* serial_1,
                                uint16_t* serial_2) {
    int16_t error;
    uint8_t buffer[9];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3682);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(1000);

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, &buffer[0], 6);
    if (error) {
        return error;
    }
    *serial_0 = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    *serial_1 = sensirion_common_bytes_to_uint16_t(&buffer[2]);
    *serial_2 = sensirion_common_bytes_to_uint16_t(&buffer[4]);
    return NO_ERROR;
}

int16_t scd4x_perform_self_test(uint16_t* sensor_status) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3639);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(10000000);

    error = sensirion_i2c_read_data_inplace(SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *sensor_status = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_perform_factory_reset() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3632);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(800000);
    return NO_ERROR;
}

int16_t scd4x_reinit() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x3646);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(20000);
    return NO_ERROR;
}

int16_t scd4x_measure_single_shot() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x219D);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(5000000);
    return NO_ERROR;
}

int16_t scd4x_measure_single_shot_rht_only() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x2196);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(50000);
    return NO_ERROR;
}

int16_t scd4x_power_down() {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x36E0);

    error = sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(1000);
    return NO_ERROR;
}

int16_t scd4x_wake_up() {
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, 0x36F6);

    // Sensor does not acknowledge the wake-up call, error is ignored
    (void)sensirion_i2c_write_data(SCD4X_I2C_ADDRESS, &buffer[0], offset);
    sensirion_i2c_hal_sleep_usec(20000);
    return NO_ERROR;
}
