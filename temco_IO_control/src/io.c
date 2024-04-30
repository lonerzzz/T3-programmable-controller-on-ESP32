#include "main.h"
#include "controls.h"

void Set_Input_Type( uint8_t point )
{
#if !(ARM_TSTAT_WIFI)
	if ((Modbus.mini_type == MINI_BIG)  || (Modbus.mini_type == MINI_BIG_ARM)
		|| (Modbus.mini_type == MINI_SMALL) || (Modbus.mini_type == MINI_SMALL_ARM))
	{
		InputLed[ point ] &= 0x0f;		
		InputLed[ point ] |= (input_type[ point ] >= 1)?((input_type[ point ] - 1) << 4):(input_type[ point ] << 4);			
	}
	  
	if ((Modbus.mini_type == MINI_TINY && Modbus.hardRev < STM_TINY_REV)|| Modbus.mini_type == MINI_CM5)  
	{
#if (ASIX_MINI || ASIX_CM5)	
		if (input_type[ point ] > 0)
		{
			push_cmd_to_picstack( SET_INPUT1_TYPE + point, input_type[ point ] - 1 );  // only for tiny
		}
		else
		{					
			push_cmd_to_picstack( SET_INPUT1_TYPE + point, 3 );  // only for tiny
		}
#endif
	}
#endif		
}

uint16_t get_input_raw(uint8_t point)
{
	return input_raw[ point ];
}

void set_output_raw(uint8_t point,uint16_t value)
{
	output_raw[ point ] = value;
}

uint16_t get_output_raw(uint8_t point)
{
	return output_raw[ point ];
}

uint32_t conver_by_unit_5v(uint32_t sample)
{
	switch (Modbus.mini_type)
	{
		case MINI_BIG:
		case MINI_BIG_ARM:	return (Modbus.hardRev >= 22)?((5000L * sample) >> 10):((3000L * sample) >> 10);	// rev22  use input module
		case MINI_SMALL:
		case MINI_SMALL_ARM:	return (Modbus.hardRev >= 4)?((5000L * sample) >> 10):((3000L * sample) >> 10);	// rev4  use input module
		case MINI_TINY:		return (Modbus.hardRev >= STM_TINY_REV)?((5000L * sample) >> 10):((8300L * sample) >> 10); // input 模块有内阻，必须加调整
		case MINI_NEW_TINY:
		case MINI_TINY_ARM:
		case MINI_TINY_11I:	return (5000L * sample) >> 10;
		case MINI_CM5:		return (5000L * sample) >> 10; // rev4  use input module
	}
}

uint32_t conver_by_unit_10v( uint32_t sample )
{
	switch (Modbus.mini_type)
	{
		case MINI_BIG:
		case MINI_BIG_ARM:	return (Modbus.hardRev >= 22)?((10000L * sample) >> 10):((9000L * sample) >> 10);
		case MINI_SMALL:
		case MINI_SMALL_ARM:	return (Modbus.hardRev >= 6)?((10000L * sample) >> 10):((9000L * sample) >> 10);	// rev4  use input module
		case MINI_TINY:		return (Modbus.hardRev >= STM_TINY_REV)?((10000L * sample) >> 10):((9000L * sample) >> 10); // rev6  use input module
		case MINI_NEW_TINY:
		case MINI_TINY_ARM:
		case MINI_TINY_11I:	return (10000L * sample) >> 10;
		case MINI_CM5:		return (10000L * sample) >> 10;
	}
}

uint32_t conver_by_unit_custable( uint8_t point,uint32_t sample )
{
	switch (input_type[ point ])
	{
		case INPUT_V0_5:
		{
			switch (Modbus.mini_type)
			{
				case MINI_BIG:
				case MINI_BIG_ARM:	return (Modbus.hardRev >= 22)?((5000L * sample) >> 10):((3000L  * sample) >> 10);
				case MINI_SMALL:
				case MINI_SMALL_ARM:	return (Modbus.hardRev >= 4)?((5000L * sample) >> 10):((3000L  * sample) >> 10);	// rev4  use input module
				case MINI_TINY:		return (Modbus.hardRev >= STM_TINY_REV)?((5000L * sample) >> 10):((8300L * sample) >> 10); // input 模块有内阻，必须加调整	
				case MINI_NEW_TINY:
				case MINI_TINY_ARM:
				case MINI_TINY_11I:	return (5000L * sample) >> 10;
				case MINI_CM5:		return (5000L * sample) >> 10;
			}
		}
		case INPUT_I0_20ma:
		{
			return (20000L * sample) >> 10;
		}
		case INPUT_0_10V:
		{
			switch (Modbus.mini_type)
			{
				case MINI_BIG:
				case MINI_BIG_ARM:	return (Modbus.hardRev >= 22)?((10000L * sample) >> 10):(( 9000L * sample) >> 10);
				case MINI_SMALL:
				case MINI_SMALL_ARM:	return (Modbus.hardRev >= 6)?((10000L * sample) >> 10):((9000L * sample) >> 10);	// rev4  use input module
				case MINI_TINY:		return (Modbus.hardRev >= STM_TINY_REV)?((10000L * sample) >> 10):((9000L * sample  ) >> 10); // rev6  use input module	
				case MINI_NEW_TINY:
				case MINI_TINY_ARM:
				case MINI_TINY_11I:	return (10000L * sample) >> 10;
				case MINI_CM5:		return (10000L * sample) >> 10;
			}
		}
		case INPUT_THERM:
		case INPUT_NOUSED:
		{
			return (3000L * sample) >> 10;	//get_input_value_by_range( inputs[ point ].range, sample );
		}
	}
}

uint8_t get_max_input()
{	
	return base_in;
}

uint8_t get_max_output()
{	
	return base_out;
}

uint8_t get_max_internal_input()
{
	switch (Modbus.mini_type)
	{
		case MINI_BIG:
		case MINI_BIG_ARM: return BIG_MAX_AIS;
		case MINI_SMALL:
		case MINI_SMALL_ARM: return SMALL_MAX_AIS;
		case MINI_TINY: return TINY_MAX_AIS;
		case MINI_VAV: return VAV_MAX_AIS;
		case MINI_CM5: return CM5_MAX_AIS;
		case MINI_NEW_TINY:
		case MINI_TINY_ARM: return NEW_TINY_MAX_AIS;
		case MINI_TINY_11I: return TINY_11I_MAX_AIS;
		case MINI_TSTAT10: return TSTAT10_MAX_AIS;
		case MINI_T10P: return T10P_MAX_AIS;
		default: return 0;
	}
}

uint8_t get_max_internal_output()
{
	switch (Modbus.mini_type)
	{
		case MINI_BIG:
		case MINI_BIG_ARM: return BIG_MAX_AOS + BIG_MAX_DOS;
		case MINI_SMALL:
		case MINI_SMALL_ARM: return SMALL_MAX_AOS + SMALL_MAX_DOS;
		case MINI_TINY: return TINY_MAX_AOS + TINY_MAX_DOS;
		case MINI_VAV: return VAV_MAX_AOS + VAV_MAX_DOS;
		case MINI_CM5: return CM5_MAX_AOS + CM5_MAX_DOS;
		case MINI_NEW_TINY:
		case MINI_TINY_ARM: return NEW_TINY_MAX_AOS + NEW_TINY_MAX_DOS;
		case MINI_TINY_11I: return TINY_11I_MAX_AOS + TINY_11I_MAX_DOS;
		case MINI_TSTAT10: return TSTAT10_MAX_AOS + TSTAT10_MAX_DOS;
		case MINI_T10P: return T10P_MAX_AOS + T10P_MAX_DOS;
		default: return 0;
	}
}

uint8_t change_value_by_range(uint8_t channel)
{
#if ARM_TSTAT_WIFI
	if (Modbus.mini_type == MINI_T10P)
	{
		// if no internal temperature sensor, use temperature of humidity
		if (input_raw[HI_COMMON_CHANNEL] > 1000)
		{
			return (channel < HI_COMMON_CHANNEL)?1:0;  // 12
		}
		else
		{
			return (channel <= HI_COMMON_CHANNEL)?1:0;  // 12
		}
	}
	else if (Modbus.mini_type == MINI_TSTAT10)
	{
		// if no internal temperature sensor, use temperature of humidity
		if (input_raw[COMMON_CHANNEL] > 1000)
		{
			return (channel < COMMON_CHANNEL)?1:0;  // 8
		}
		else
		{
			return (channel <= COMMON_CHANNEL)?1:0;  // 8
		}
	}
	else
#endif
	{
		return 1;
	}
}

uint32_t get_high_spd_counter(uint8_t point)
{
	inputs[ point ].value = swap_double((high_spd_counter[ point ] + high_spd_counter_tempbuf[ point ]) * 1000);
	Test[30]++;
	return (high_spd_counter[ point ] + high_spd_counter_tempbuf[ point ]) * 1000;
}

#if (ARM_MINI || ARM_TSTAT_WIFI)
void Store_Pulse_Counter(uint8 flag)
{
	static u32 old_pulse[ HI_COMMON_CHANNEL ];
	char i;
	if ((run_time % 3600 == 0) || (flag == 1))  // 每小时检查一次是否需要保存pulse counter
	{
		for (i = 0;i < HI_COMMON_CHANNEL;i++)
		{
			if ((inputs[ i ].range == HI_spd_count) || (inputs[ i ].range == N0_2_32counts) || (inputs[ i ].range == RPM))
			{
				if (old_pulse[ i ] != (high_spd_counter[ i ] + high_spd_counter_tempbuf[ i ]))
				{
					old_pulse[ i ] = (high_spd_counter[ i ] + high_spd_counter_tempbuf[ i ]);
					write_page_en[1] = 1; // 保存input
					ChangeFlash = 2;
				}
					
			}
		}
	}
}

// 10s 调用一次
void calculate_RPM(void)
{
	static u32 old_count[HI_COMMON_CHANNEL];
	static u32 runtime[HI_COMMON_CHANNEL];
	static u16 count[HI_COMMON_CHANNEL]; 
	// 持续输入计数1分钟重新清掉rpm的时间和count
	char i;
	char channel;
	
	for (i=0; i<HI_COMMON_CHANNEL; i++)
	{
//		if ((inputs[ i ].range == HI_spd_count) || (inputs[ i ].range == N0_2_32counts) || (inputs[ i ].range == RPM))
		if (inputs[ i ].range == RPM)
		{
			if (old_count[ i ] <= high_spd_counter_tempbuf[ i ])
			{	
				Input_RPM[ i ] = (high_spd_counter_tempbuf[ i ] - old_count[ i ]) * 60L / (run_time - runtime[ i ]);	
				
				if (count[ i ]++ >= 5)  // 1分钟重新计算
				{
					runtime[ i ] = run_time;	
					old_count[ i ] = high_spd_counter_tempbuf[ i ];
					count[ i ] = 0;
				}	
			}		
			else
			{
				count[ i ] = 0;
				runtime[ i ] = run_time;	
				old_count[ i ] = high_spd_counter_tempbuf[ i ];
			}
		}		
	}		
}

uint32_t get_rpm( uint8_t point )
{	
	return Input_RPM[ point ] * 1000;
}

void Check_spd_count_led(void)
{
	char i;
	static u32 old_count2[ HI_COMMON_CHANNEL ];
	for (i=0; i<HI_COMMON_CHANNEL; i++)
	{		
		if ((inputs[ i ].range == HI_spd_count) || (inputs[ i ].range == N0_2_32counts) || (inputs[ i ].range == RPM))
		{
			if (old_count2[ i ] < high_spd_counter_tempbuf[ i ])
			{	
				flag_count_in[ i ] = 1;
				old_count2[ i ] = high_spd_counter_tempbuf[ i ];
			}		
			else
			{	
				flag_count_in[ i ] = 0;
			}	
		}
	}	
}
#endif

// old io.lib run it
void map_extern_output(uint8_t point)
{
}

int8_t check_external_in_on_line( uint8_t index )
{
	uint8_t i;
	for (i=0; i<sub_no; i++)
	{
		if ((index < sub_map[ i ].ai_start + sub_map[ i ].ai_len) && (index >= sub_map[ i ].ai_start))
		{
			break;
		}
	}

	return (i == sub_no)?-1:((current_online[scan_db[ i ].id / 8] & (1 << (scan_db[ i ].id % 8)))?1:0);
}

int8_t check_external_out_on_line( uint8_t index )
{
	uint8_t i;
	if (index < get_max_internal_output())
	{
		return -1;
	}
	for (i=0; i<sub_no; i++)
	{
		if ((index < sub_map[ i ].ao_start + sub_map[ i ].ao_len) && (index >= sub_map[ i ].ao_start))
		{
			break;
		}
		if ((index < sub_map[ i ].do_start + sub_map[ i ].do_len) && (index >= sub_map[ i ].do_start))
		{
			break;
		}
	}

	return (i == sub_no)?-1:((current_online[scan_db[ i ].id / 8] & (1 << (scan_db[ i ].id % 8)))?1:0);
}
