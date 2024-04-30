#include "controls.h"

#ifdef OUTPUT

extern Str_out_point far	outputs[MAX__outputs];

//uint8_t far control_auto[MAX__outputs];
//uint8_t far switch_status_back[MAX__outputs];

void control_output(void)
{
	Str_out_point* _outputs;
	uint8_t point = 0;
	uint32_t val;
//	uint8_t loop;
	uint32_t value;
	point = 0;
	_outputs = outputs;


	while( point < MAX__outputs )
	{
		if (point < get_max_output())
		{			
			if (point < get_max_internal_output())
			{		
				_outputs->sub_id = 0;
				_outputs->sub_product = 0;
				_outputs->sub_number = 0;
				_outputs->decom = 0;
			}
			if (_outputs->range == not_used_output)
			{
				_outputs->value = 0L;
				val = 0;
			}
//			else
//			{	
//				if ( _outputs->digital_analog == 1 ) //  analog
//				{	
//					if (point >= get_max_internal_output())  // // external ouput 					
//					{  // range is 0-10v
//						if (_outputs->sub_product != 44/*PM_T38AI8AO6DO*/)
//						{
//							if (_outputs->read_remote == 1)
//							{
//								val = 10000000 / 4095;
//								val = val * get_output_raw(point) / 1000;
//								_outputs->read_remote = 0;
//							}
//							else
//							{		
//								val = swap_double(_outputs->value);	
//	//							val = val * 4095 / 10000;
//								// ?????????
//								if (val < get_output_raw(point))
//								{
//									if (get_output_raw(point) - val == 1)
//										set_output_raw(point,val + 1);//output_raw[point] = val + 1;
//									if (get_output_raw(point) - val == 2)
//										set_output_raw(point,val + 2);//output_raw[point] = val + 2;
//								}
//								else
//									set_output_raw(point,val);//output_raw[point] = val;
//								
//							}
//						}
//					}
//				}
//			}
//		}

			if (check_external_out_on_line(point) == 1)
			{	
				map_extern_output(point);
			}
			else
			{
				_outputs->sub_id = 0;
				_outputs->sub_product = 0;
				_outputs->sub_number = 0;
			
				if (point >= get_max_internal_output())
				{
					_outputs->decom = 1;  // no used
				}
			}
		}
		else
		{
			_outputs->sub_id = 0;
			_outputs->sub_product = 0;
			_outputs->sub_number = 0;
	
			_outputs->decom = 1;  // no used
		}
		point++;
		_outputs++;
	}
}
#endif
