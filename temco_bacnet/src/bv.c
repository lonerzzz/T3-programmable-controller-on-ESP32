/**************************************************************************
*
* Copyright (C) 2006 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/

/* Binary Value Objects - customize for your use */

#include <stdbool.h>
#include <stdint.h>
#include "bacnet.h"
#include <stdio.h>
//#include "hardware.h"
#include "bacdef.h"
#include "bacdcode.h"
#include "bacenum.h"
#include "wp.h"
#include "bv.h"

#if 1// BAC_COMMON

#if BAC_BV
uint8_t BVS;

//#if (MAX_BINARY_VALUES > 10)
//#error Modify the Binary_Value_Name to handle multiple digits
//#endif

//static BACNET_BINARY_PV Present_Value[MAX_BINARY_VALUES];

/* These three arrays are used by the ReadPropertyMultiple handler */
#if ARM
static const 
#endif
int far  Binary_Value_Properties_Required[] = {
    PROP_OBJECT_IDENTIFIER,
    PROP_OBJECT_NAME,
    PROP_OBJECT_TYPE,
    PROP_PRESENT_VALUE,
    PROP_STATUS_FLAGS,
    PROP_EVENT_STATE,
//    PROP_OUT_OF_SERVICE,
    PROP_POLARITY,
    -1
};

#if ARM
static const 
#endif
int far Binary_Value_Properties_Optional[] = {
    PROP_DESCRIPTION,
    -1
};

#if ARM
static const 
#endif
int far Binary_Value_Properties_Proprietary[] = {
    -1
};

void Binary_Value_Property_Lists(
    const int **pRequired,
    const int **pOptional,
    const int **pProprietary)
{
	
#if ASIX
	Binary_Value_Properties_Required[0] = PROP_OBJECT_IDENTIFIER;
	Binary_Value_Properties_Required[1] = PROP_OBJECT_NAME;
	Binary_Value_Properties_Required[2] = PROP_OBJECT_TYPE;
	Binary_Value_Properties_Required[3] = PROP_PRESENT_VALUE;
	Binary_Value_Properties_Required[4] = PROP_STATUS_FLAGS;
	Binary_Value_Properties_Required[5] = PROP_EVENT_STATE;
//	Binary_Value_Properties_Required[6] = PROP_OUT_OF_SERVICE;
	Binary_Value_Properties_Required[6] = PROP_POLARITY;
	
	Binary_Value_Properties_Optional[0] = -1;
	Binary_Value_Properties_Proprietary[0] = -1;
#endif
	
	
    if (pRequired) {
        *pRequired = Binary_Value_Properties_Required;
    }
    if (pOptional) {
        *pOptional = Binary_Value_Properties_Optional;
    }
    if (pProprietary) {
        *pProprietary = Binary_Value_Properties_Proprietary;
    }

    return;
}


/* we simply have 0-n object instances. */
bool Binary_Value_Valid_Instance(
    uint32_t object_instance)
{
    if ((object_instance < MAX_BVS + OBJECT_BASE) /*&& (object_instance >= OBJECT_BASE)*/)
        return true;

    return false;
}

/* we simply have 0-n object instances. */
unsigned Binary_Value_Count(
    void)
{
    return BVS;//MAX_BINARY_VALUES;
}

/* we simply have 0-n object instances. */
uint32_t Binary_Value_Index_To_Instance(
    unsigned index)
{
#ifdef T3_CON
		return BV_Index_To_Instance[index] + OBJECT_BASE;
#else
    return index + OBJECT_BASE;
#endif
}

/* we simply have 0-n object instances.  */
unsigned Binary_Value_Instance_To_Index(
    uint32_t object_instance)
{
#ifdef T3_CON

	return BV_Instance_To_Index[object_instance - OBJECT_BASE];
#else
  return object_instance - OBJECT_BASE;
#endif
}


bool Binary_Value_Object_Name(
    uint32_t object_instance,
    BACNET_CHARACTER_STRING * object_name)
{
    bool status = false;
		unsigned index = 0;

    index = Binary_Value_Instance_To_Index(object_instance);
	
    if (object_instance < MAX_BVS) {
        status = characterstring_init_ansi(object_name, get_label(BV,index));
    }

    return status;
}

BACNET_BINARY_PV Binary_Value_Present_Value(
    uint32_t object_instance)
{
    BACNET_BINARY_PV far value = BINARY_INACTIVE;

		unsigned index = 0;
    unsigned i = 0;

    index = object_instance;
//		value = Get_Output_Relinguish(BO,index);
	
		if (index < MAX_BVS) {
        for (i = 0; i < BACNET_MAX_PRIORITY; i++) {
            if (Get_bacnet_value_from_buf(BV,i,index) != BINARY_NULL) {
                value = Get_bacnet_value_from_buf(BV,i,index);
                break;
            }
        }
    }

    return value;
}

/* note: the object name must be unique within this device */
//char *Binary_Value_Name(
//    uint32_t object_instance)
//{
//    static char text_string[5] = "BV-0";        /* okay for single thread */

//    if (object_instance < MAX_BINARY_VALUES) {
//        text_string[3] = '0' + (uint8_t) object_instance;
//        return text_string;
//    }

//    return NULL;
//}

/* return apdu len, or -1 on error */
int Binary_Value_Encode_Property_APDU(
    uint8_t * apdu,
    uint32_t object_instance,
    BACNET_PROPERTY_ID property,
    uint32_t array_index,
    BACNET_ERROR_CLASS * error_class,
    BACNET_ERROR_CODE * error_code)
{
    int apdu_len = 0;   /* return value */
    BACNET_BIT_STRING far bit_string;
    BACNET_CHARACTER_STRING far char_string;
	
		unsigned object_index;
	
		object_index = Binary_Value_Instance_To_Index(object_instance);

//    BACNET_BINARY_PV present_value = BINARY_INACTIVE;
//    BACNET_POLARITY polarity = POLARITY_NORMAL;
    switch (property) {
        case PROP_OBJECT_IDENTIFIER:
            apdu_len =
                encode_application_object_id(&apdu[0], OBJECT_BINARY_VALUE,
                object_instance);
            break;
            /* note: Name and Description don't have to be the same.
               You could make Description writable and different */
        case PROP_OBJECT_NAME: 
            characterstring_init_ansi(&char_string,get_label(BV,object_index));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
				case PROP_DESCRIPTION:
            /* note: object name must be unique in our device */
						characterstring_init_ansi(&char_string,get_description(BV,object_index));
						apdu_len = encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_TYPE:
            apdu_len =
                encode_application_enumerated(&apdu[0], OBJECT_BINARY_VALUE);
            break;
        case PROP_PRESENT_VALUE:
            apdu_len =
                encode_application_enumerated(&apdu[0],
								Get_bacnet_value_from_buf(BV,0,object_index));
            break;
        case PROP_STATUS_FLAGS:
            /* note: see the details in the standard on how to use these */
            bitstring_init(&bit_string);
            bitstring_set_bit(&bit_string, STATUS_FLAG_IN_ALARM, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_FAULT, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OVERRIDDEN, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OUT_OF_SERVICE, false);
            apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
            break;
        case PROP_EVENT_STATE:
            /* note: see the details in the standard on how to use this */
            apdu_len =
                encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
            break;
        case PROP_OUT_OF_SERVICE:
            apdu_len = encode_application_boolean(&apdu[0], false);
            break;
        case PROP_POLARITY:
            // added by chelsea
            apdu_len = encode_application_enumerated(&apdu[0],Binary_Value_Polarity(object_index));
            break;
        default:
            *error_class = ERROR_CLASS_PROPERTY;
            *error_code = ERROR_CODE_UNKNOWN_PROPERTY;
            apdu_len = -1;
            break;
    }
    /*  only array properties can have array options */
    if ((apdu_len >= 0) && (array_index != BACNET_ARRAY_ALL)) {
        *error_class = ERROR_CLASS_PROPERTY;
        *error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
        apdu_len = -1;
    }

    return apdu_len;
}

/* returns true if successful */
bool Binary_Value_Write_Property(
    BACNET_WRITE_PROPERTY_DATA * wp_data,
    BACNET_ERROR_CLASS * error_class,
    BACNET_ERROR_CODE * error_code)
{
    bool status = false;        /* return value */
    unsigned int object_index = 0;
    int len = 0;
    BACNET_APPLICATION_DATA_VALUE far value;
		BACNET_BINARY_PV far level = BINARY_NULL;
		
    if (!Binary_Value_Valid_Instance(wp_data->object_instance)) {
        *error_class = ERROR_CLASS_OBJECT;
        *error_code = ERROR_CODE_UNKNOWN_OBJECT;
        return false;
    }
    /* decode the some of the request */
    len =
        bacapp_decode_application_data(wp_data->application_data,
        wp_data->application_data_len, &value);    /* FIXME: len < application_data_len: more data? */
    if (len < 0) {
        /* error while decoding - a value larger than we can handle */
        *error_class = ERROR_CLASS_PROPERTY;
        *error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
        return false;
    }
    if ((wp_data->object_property != PROP_PRIORITY_ARRAY) &&
        (wp_data->array_index != BACNET_ARRAY_ALL)) {
        /*  only array properties can have array options */
        *error_class = ERROR_CLASS_PROPERTY;
        *error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
        return false;
    }
				
		object_index = Binary_Value_Instance_To_Index(wp_data->object_instance);

    switch (wp_data->object_property) {
        case PROP_PRESENT_VALUE: 
            if (value.tag == BACNET_APPLICATION_TAG_ENUMERATED) {
                if ((value.type.Enumerated == BINARY_ACTIVE) ||
                    (value.type.Enumerated == BINARY_INACTIVE)) {
                    /* NOTE: this Binary value has no priority array */
//                    Present_Value[object_index] =
//                        (BACNET_BINARY_PV) value.type.Enumerated;
											
											level = (BACNET_BINARY_PV) value.type.Enumerated;
											wirte_bacnet_value_to_buf(BV,wp_data->priority,object_index,level);
											
                    /* Note: you could set the physical output here if we
                       are the highest priority.
                       However, if Out of Service is TRUE, then don't set the
                       physical output. */
											
											
											// TBD: chelsea
											
//									if (Present_Value[0] == BINARY_ACTIVE) {
//									} else {
//									}
                    status = true;
                } else {
                    *error_class = ERROR_CLASS_PROPERTY;
                    *error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                }
            } else {
                *error_class = ERROR_CLASS_PROPERTY;
                *error_code = ERROR_CODE_INVALID_DATA_TYPE;
            }
            break;
        case PROP_OUT_OF_SERVICE:
#if 1
            if (value.tag == BACNET_APPLICATION_TAG_BOOLEAN) {
//                object_index =
//                    Binary_Value_Instance_To_Index(wp_data->object_instance);
//                Binary_Value_Out_Of_Service[object_index] = value.type.Boolean;
							write_Out_Of_Service(BV,object_index,value.type.Boolean);
                status = true;
            } else {
                *error_class = ERROR_CLASS_PROPERTY;
                *error_code = ERROR_CODE_INVALID_DATA_TYPE;
            }
            break;
#endif
				case PROP_OBJECT_NAME:
				if (value.tag == BACNET_APPLICATION_TAG_CHARACTER_STRING) {

					write_bacnet_name_to_buf(BV,wp_data->priority,object_index,value.type.Character_String.value);
                status = true;
            } else {
                wp_data->error_class = ERROR_CLASS_PROPERTY;
                wp_data->error_code = ERROR_CODE_INVALID_DATA_TYPE;
            }						
				break;
				case PROP_DESCRIPTION:
						if (value.tag == BACNET_APPLICATION_TAG_CHARACTER_STRING) {
								write_bacnet_description_to_buf(BV,wp_data->priority,object_index,value.type.Character_String.value);
                status = true;
            } else {
                wp_data->error_class = ERROR_CLASS_PROPERTY;
                wp_data->error_code = ERROR_CODE_INVALID_DATA_TYPE;
            }						
				break;
				case PROP_POLARITY:
					status =
                WPValidateArgType(&value, BACNET_APPLICATION_TAG_ENUMERATED,
                &wp_data->error_class, &wp_data->error_code);
            if (status) { 
                if (value.type.Enumerated < MAX_POLARITY) {  
                    Binary_Value_Polarity_Set(wp_data->object_instance,
                        (BACNET_POLARITY) value.type.Enumerated);
                } else {
                    status = false;
                    wp_data->error_class = ERROR_CLASS_PROPERTY;
                    wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                }
            }
            break;	
				break;
        case PROP_OBJECT_IDENTIFIER:
        case PROP_OBJECT_TYPE:
        case PROP_STATUS_FLAGS:
        case PROP_EVENT_STATE:
        
            *error_class = ERROR_CLASS_PROPERTY;
            *error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
            break;
        default:
            *error_class = ERROR_CLASS_PROPERTY;
            *error_code = ERROR_CODE_UNKNOWN_PROPERTY;
            break;
    }

    return status;
}

#ifdef TEST
#include <assert.h>
#include <string.h>
#include "ctest.h"

void testBinary_Value(
    Test * pTest)
{
    uint8_t apdu[MAX_APDU] = { 0 };
    int len = 0;
    uint32_t len_value = 0;
    uint8_t tag_number = 0;
    BACNET_OBJECT_TYPE decoded_type = OBJECT_BINARY_VALUE;
    uint32_t decoded_instance = 0;
    uint32_t instance = 123;
    BACNET_ERROR_CLASS error_class;
    BACNET_ERROR_CODE error_code;


    len =
        Binary_Value_Encode_Property_APDU(&apdu[0], instance,
        PROP_OBJECT_IDENTIFIER, BACNET_ARRAY_ALL, &error_class, &error_code);
    ct_test(pTest, len != 0);
    len = decode_tag_number_and_value(&apdu[0], &tag_number, &len_value);
    ct_test(pTest, tag_number == BACNET_APPLICATION_TAG_OBJECT_ID);
    len =
        decode_object_id(&apdu[len], (int *) &decoded_type, &decoded_instance);
    ct_test(pTest, decoded_type == OBJECT_BINARY_VALUE);
    ct_test(pTest, decoded_instance == instance);

    return;
}

#ifdef TEST_BINARY_VALUE
int main(
    void)
{
    Test *pTest;
    bool rc;

    pTest = ct_create("BACnet Binary_Value", NULL);
    /* individual tests */
    rc = ct_addTestFunction(pTest, testBinary_Value);
    assert(rc);

    ct_setStream(pTest, stdout);
    ct_run(pTest);
    (void) ct_report(pTest);
    ct_destroy(pTest);

    return 0;
}
#endif /* TEST_BINARY_VALUE */
#endif /* TEST */

#endif
#endif
