

#ifndef	_SCAN_H_

#define	_SCAN_H_


#include "types.h"
#include "esp_attr.h"

#pragma pack(1)
typedef struct _SCAN_DATABASE_
{
	U8_T id;
	U32_T sn;
	U8_T port;	// high half byte -- baut , low half byte - port
	U8_T product_model;  
} SCAN_DB;

#define STACK_LEN  20
#define MAX_WRITE_RETRY 3//10

typedef struct
{
	U8_T ip;
	U8_T func;
	U8_T id;
	U16_T reg;
	U16_T value[2];
	U8_T len;
	U8_T flag;
	U8_T retry;
}STR_NP_NODE_OPERATE;

extern STR_NP_NODE_OPERATE NP_node_write[STACK_LEN];
// ��չIOӳ��
typedef struct
{
  U8_T sub_index;
	U8_T type;
	U8_T id;
	U8_T do_start;
	U8_T ao_start;
	U8_T ai_start;
	U8_T var_start;
	U8_T do_len;
	U8_T ao_len;
	U8_T ai_len;
	U8_T var_len;
	U8_T add_in_map;
}STR_MAP_table;


#define MAX_ID		5//255
#define SUB_NO		MAX_ID - 1

#define	NONE_ID						0
#define	UNIQUE_ID					1
#define	MULTIPLE_ID					2
#define	UNIQUE_ID_FROM_MULTIPLE		3
#define	ASSIGN_ID					4
#define SCAN_BUSY					0xff

#define MAX_BUF_LEN        500// 300

#define	SCAN_BINSEARCH			1
#define	SCAN_ASSIGN_ID_WITH_SN	2

#define READ_TSTAT_COUNT 30

#define SCAN_DB_TIME_TO_LIVE  600  // 1min


extern U32_T  com_rx[3];
extern U32_T  com_tx[3];
extern U16_T  collision[3];  // id collision
extern U16_T  packet_error[3];  // bautrate not match
extern U16_T  timeout[3];

extern EXT_RAM_ATTR U8_T  tstat_name[MAX_ID][16];
extern EXT_RAM_ATTR U8_T  flag_tstat_name[MAX_ID];
extern EXT_RAM_ATTR U16_T  count_read_tstat_name[MAX_ID];
extern EXT_RAM_ATTR SCAN_DB  current_db;
extern EXT_RAM_ATTR SCAN_DB  scan_db[MAX_ID];
extern EXT_RAM_ATTR S16_T  scan_db_time_to_live[MAX_ID];
extern U8_T db_ctr;
extern U8_T db_online[32], db_occupy[32]/*, get_para[32]*/;
extern U8_T current_online[32];
extern U8_T current_online_ctr;
extern U8_T reset_scan_db_flag;

extern U8_T flag_suspend_scan;
extern U8_T suspend_scan_count;

extern U8_T count_send_id_to_zigbee;

// time sync, schedule sync, send schedule_data ..
// all commands between T3_BB and T8
//extern 	U8_T f_time_sync; // 0 - writing , 1 - ok, 2 - fail

typedef struct
{
	U8_T f_schedule_sync; // 
	U8_T f_send_schdule_data;
	U8_T f_time_sync;
	U8_T schedule_id;
	U8_T schedule_index;
	U8_T schedule_output;
	U8_T en_device_schdule;// 0: noused 1: enabled 
	
	U8_T count_send_schedule;  // send command after 10 sec
}STR_Sch_To_T8;

//extern U8_T count_send_schedule[100];
extern STR_Sch_To_T8 Sch_To_T8[MAX_ID];
	
typedef struct
{
	U8_T flag; // 0 - writing , 1 - ok, 2 - fail
	U32_T sn; // 
	U8_T oldid;
	U8_T newid;
}STR_T3000;


typedef struct
{
	U8_T  id;
	U32_T sn_old;
	U32_T sn_new;
//	U8_T  num;
	U8_T  port;
	U8_T  product;
	U16_T count;
}Str_CONFILCT_ID;

#define 	MAX_ID_CONFILCT 	20
extern Str_CONFILCT_ID 	id_conflict[MAX_ID_CONFILCT];
extern U8_T index_id_conflict;

//#define MAX_CONFLICT_ID 10
extern STR_T3000 T3000_Private;
//extern U8_T far conflict_id;//[MAX_CONFLICT_ID];
//extern U32_T far conflict_sn_old;//[MAX_CONFLICT_ID];
//extern U32_T far conflict_sn_new;//[MAX_CONFLICT_ID];
//extern U8_T far conflict_num;
//extern U8_T far conflict_port;
//extern U8_T far conflict_product;
//extern u8 tcp_server_sendbuf[300];
//extern u16 tcp_server_sendlen;
extern u8 modbus_send_buf[500];
extern u16 modbus_send_len;

void Response_TCPIP_To_SUB(U8_T *buf, U16_T len,U8_T port,U8_T *header);
void Record_conflict_ID(U8_T id, U32_T oldsn,U32_T newsn,U8_T port,U8_T product);
void clear_conflict_id(void);
void Check_whether_clear_conflict_id(void);

//void check_write_to_nodes(void);
//void get_parameters_from_nodes(void);
void write_parameters_to_nodes(U8_T func,U8_T id, U16_T reg, U16_T* value, U8_T len);
void write_NP_Modbus_to_nodes(U8_T ip,U8_T func,U8_T sub_id, U16_T reg, U16_T * value, U8_T len);

void init_scan_db(void);
//U8_T check_master_id_in_database(U8_T set_id, U8_T increase) reentrant;
void vStartScanTask(unsigned char uxPriority);
void modify_master_id_in_database(U8_T old_id, U8_T set_id);
void remove_id_from_db(U8_T index_of_scan_db);
void Response_MAIN_To_SUB(U8_T *buf, U16_T len,U8_T port);
void Response_TCPIP_To_SUB(U8_T *buf, U16_T len,U8_T port,U8_T *header);

void Get_Tst_DB_From_Flash(void);
void list_tstat_pos(void);
void clear_scan_db(void);
void check_read_tstat_name(void);
void auto_check_uart_comfig(U8_T port);

void Count_com_config(void);
void Check_On_Line(void);



void send_ID_to_ZIGBEE(void);
void Check_Zigbee(void);
void Reset_ZIGBEE(void);
U8_T read_zigbee_map_number(U16_T addr,U8_T len,U8_T * zigbee_map);

void Count_bip_client_reading(void);

U8_T check_id_in_database(U8_T id, U32_T sn,U8_T port,U8_T baut,U8_T product_id);
U8_T assignment_id_with_sn(U8_T old_id, U8_T new_id, U32_T current_sn,U8_T port);

U8_T Get_product_by_id(U8_T id);
U8_T get_port_by_id(U8_T id);
U8_T get_index_by_id(U8_T id, U8_T* index);
U8_T get_IO_index_by_reg(U16_T reg, U8_T* index);

U8_T get_baut_by_port(U8_T port);
U8_T get_baut_by_id(U8_T port,U8_T id);
void set_baut_by_port(U8_T port,U8_T baut);   // need set real baut

void Check_scan_db_time_to_live(void);

#endif

