/******************************************************************************

**	NAME : sdp.h

**	DESCRIPTION : it contains the included files, prototypes of functions 
		      used and macros used throughout

**	copyright @ 2008 ARICENT Inc. All Rights Reserved

******************************************************************************/


#ifndef _SDP_H_
	#define _SDP_H_

/************************* INCLUDED FILES ********************/

#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>
#include 	<sys/types.h>
#include 	<sys/stat.h>
#include 	<sys/timeb.h>
#include 	<sys/socket.h>
#include 	<netinet/in.h>
#include 	<arpa/inet.h>
#include 	<sys/wait.h>
#include 	<time.h>
#include 	<stdarg.h>
#include 	<fcntl.h>
#include 	<unistd.h>
#include 	<errno.h>

/**************************** MACROS *********************/

#ifdef DEBUG
	#define TRACE printf
#else
	#define dummy(x)  
	#define TRACE dummy
#endif

#ifdef LOG
	#define LOGGER logger
#else
	#define dummy_log(x,y)  
	#define LOGGER dummy_log
#endif

/************************** Error Numbers **************************/

#define SDP_VERSION_ERROR 1010
#define SDP_VERSION_FILE_ERROR 1011
#define SDP_VERSION_MALLOC_ERROR 1012

#define SDP_ORIGIN_ERROR 1020
#define SDP_ORIGIN_FILE_ERROR 1021
#define SDP_ORIGIN_MALLOC_ERROR 1022

#define SDP_NAME_ERROR 1030
#define SDP_NAME_FILE_ERROR 1031
#define SDP_NAME_MALLOC_ERROR 1032

#define SDP_INFO_ERROR 1040
#define SDP_INFO_FILE_ERROR 1041
#define SDP_INFO_MALLOC_ERROR 1042

#define SDP_URI_ERROR 1050
#define SDP_URI_FILE_ERROR 1051
#define SDP_URI_MALLOC_ERROR 1052

#define SDP_EMAIL_ERROR 1060
#define SDP_EMAIL_FILE_ERROR 1061
#define SDP_EMAIL_MALLOC_ERROR 1062

#define SDP_PHONE_ERROR 1070
#define SDP_PHONE_FILE_ERROR 1071
#define SDP_PHONE_MALLOC_ERROR 1072

#define SDP_CONNECTION_ERROR 1080
#define SDP_CONNECTION_FILE_ERROR 1081
#define SDP_CONNECTION_MALLOC_ERROR 1082

#define SDP_BANDWIDTH_ERROR 1090
#define SDP_BANDWIDTH_FILE_ERROR 1091
#define SDP_BANDWIDTH_MALLOC_ERROR 1092

#define SDP_TIME_ERROR 1100
#define SDP_TIME_FILE_ERROR 1101
#define SDP_TIME_MALLOC_ERROR 1102

#define SDP_REPEAT_ERROR 1110
#define SDP_REPEAT_FILE_ERROR 1111
#define SDP_REPEAT_MALLOC_ERROR 1112

#define SDP_ZONE_ERROR 1120
#define SDP_ZONE_FILE_ERROR 1121
#define SDP_ZONE_MALLOC_ERROR 1122

#define SDP_KEY_ERROR 1130
#define SDP_KEY_FILE_ERROR 1131
#define SDP_KEY_MALLOC_ERROR 1132

#define SDP_ATTRIBUTE_ERROR 1140
#define SDP_ATTRIBUTE_FILE_ERROR 1141
#define SDP_ATTRIBUTE_MALLOC_ERROR 1142

#define SDP_MEDIA_ERROR 1150
#define SDP_MEDIA_FILE_ERROR 1151
#define SDP_MEDIA_MALLOC_ERROR 1152

#define SDP_FIELDS_FILE_ERROR 1161
#define SDP_FIELDS_MALLOC_ERROR 1162

#define SDP_CHECKER_ERROR 1170
#define SDP_CHECKER_FILE_ERROR 1171
#define SDP_CHECKER_MANDATORY_ATTR_ERROR 1172
#define SDP_CHECKER_NON_REPEATIVE_ATTR_ERROR 1173
#define SDP_CHECKER_S_ORDER_ERROR 1174
#define SDP_CHECKER_M_ORDER_ERROR 1175
#define SDP_CHECKER_FIELDS_NUM_ERROR 1176

#define SDP_SENDER_ERROR 1180
#define SDP_SENDER_SOCKET_ERROR 1181
#define SDP_SENDER_BIND_ERROR 1182
#define SDP_SENDER_LISTEN_ERROR 1183
#define SDP_SENDER_ACCEPT_ERROR 1184
#define SDP_SENDER_SELECT_ERROR 1185
#define SDP_ADD_CLIENT_ERROR 1186
#define SDP_SENDER_MALLOC_ERROR 1187

#define SDP_MEDIA_CHECK_ERROR 1190

#define SDP_POPULATE_ERROR 1300

#define SDP_SESSION_TO_STR_ERROR 1310

#define SDP_FILE_WRITE_ERROR 1200

#define SDP_STRING_MALLOC_ERROR 1210

#define SDP_FILE_HASH_ERROR 1220

#define SDP_STRCPY_ERROR 1230

#define SDP_READ_ERROR 1240

#define SDP_WRITE_ERROR 1250

#define SDP_STRNCPY_ERROR 1260

#define SDP_ARGC_ERROR 1270

#define SDP_CLOSE_ERROR 1280

#define SDP_GETHOSTNAME_ERROR 1290

#define SDP_TIMER_ERROR 1300

#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define FAILURE -1

#define FILE_SIZE 32
#define SIZE 100

#define ARR_SIZE 7
#define COUNT_SIZE 5
#define SESS_ARR_SIZE 15
#define MEDIA_ARR_SIZE 6
#define FIELDCOUNT_SIZE 14
#define ERROR_STR_SIZE 30
#define LOOP 5
#define BUFF_SIZE 512
#define MAXBUFFER 1024
#define MYPORT 21930
#define BACKLOG 5
#define ATTR_SIZE 100
#define FIELD_SIZE 20
#define BLOCK 100
#define MARGIN 10
#define ONE 1
#define ZERO 0
#define INIT '\0'
#define ARGC 2

/************************** Logger Levels **************************/
 
#define LOG_CRITICAL 0
#define LOG_MAJOR 1
#define LOG_MINOR 2
#define LOG_TRIVIAL 3
#define LOG_LEVEL 4
#define LOG_MAX 10000L


/*************************error levels*****************************/

#define ERR_ALL 4
#define ERR_CRITICAL 3
#define ERR_MAJOR 2
#define ERR_MINOR 1

/*********************typedefs of basic datatypes********************/

typedef int return_type;

/************STRUCTURE DECLARATIONS*********************/

/************LIST STRUCTURE***************/
typedef struct sdp_list
{
	void			*value;
	struct sdp_list		*next;
}sdp_list_t;	

/*******ORIGIN STRUCTURE*****************/
typedef struct sdp_origin
{
	char			*o_username;
	unsigned long   	o_id;
	unsigned long   	o_version;
	char			*o_nettype;
	char			*o_addrtype;
	char			*o_address;
}sdp_origin_t;

/********CONNECTION STRUCTURE*************/
typedef struct sdp_conn
{
	char			*c_nettype;
	char			*c_addrtype;
	char			*c_address;
	int			c_addrcount;
	struct sdp_conn		*c_next;
	unsigned int		c_ttl;
}sdp_conn_t;	

/*********TIME REPEAT FIELD***************/
typedef struct sdp_repeat
{
	unsigned long   	r_interval;
	unsigned long   	r_duration;
	sdp_list_t		*r_offset;
	struct sdp_repeat	*r_next;
}sdp_repeat_t;

/*********TIME STRUCTURE******************/
typedef struct sdp_time
{
	unsigned long   	t_start;
	unsigned long   	t_stop;
	sdp_repeat_t		*t_repeat;
	struct sdp_time		*t_next;
}sdp_time_t;

/*********ZONE STRUCTURE******************/
typedef struct sdp_zone
{
	unsigned long  	z_time;
	char			*z_offset;
	struct sdp_zone		*z_next;
}sdp_zone_t;	


/*********ATTRIBUTE STRUCTURE**************/
typedef struct sdp_attr
{
	char			*a_name;
	char			*a_value;
	struct sdp_attr		*a_next;
}sdp_attr_t;

/*********BANDWIDTH STRUCTURE***************/
typedef	struct sdp_bandwidth
{
	char 			*b_type;
	unsigned int		b_value;
	struct sdp_bandwidth	*b_next;
}sdp_bandwidth_t;

/**********ENCRYPTION KEY STRUCTURE**********/
typedef struct sdp_key
{
	char 			*k_method;
	char			*k_enckey;
}sdp_key_t;

/***********MEDIA STRUCTURE******************/
typedef struct sdp_media
{
	char			*m_name;
	int			m_port;
	int			m_portcount;
	char			*m_proto;
	sdp_list_t		*m_format;
	char			*m_info;
	sdp_conn_t		*m_conn;
	sdp_bandwidth_t		*m_bw;
	sdp_key_t		*m_key;
	sdp_attr_t		*m_attr;
	struct sdp_media	*m_next;
}sdp_media_t;

/************SESSION STRUCTURE****************/
typedef struct sdp_session
{
	int 			s_version;
	sdp_origin_t		*s_origin;
	char			*s_name;
	char			*s_info;
	char			*s_uri;
	sdp_list_t		*s_email;
	sdp_list_t		*s_phone;
	sdp_bandwidth_t		*s_bw;
	sdp_conn_t		*s_conn;
	sdp_time_t		*s_time;
	sdp_zone_t      *s_zone;
	sdp_key_t		*s_key;
	sdp_attr_t		*s_attr;
	sdp_media_t		*s_media;
}sdp_session_t ;

/********************** GLOBAL VARIABLE ************************/

char inputfile[FILE_SIZE];
int log_level;
long log_max;
char log_file[FILE_SIZE];
char program_name[FILE_SIZE];

/***********************FUNCTION DECLARATIONS*******************/

extern return_type sdp_checker(char *);
extern return_type sdp_populate_message(sdp_session_t * , char *);
extern return_type sdp_add_version(sdp_session_t * , char *);
extern return_type sdp_add_origin(sdp_session_t * , char *);
extern return_type sdp_add_name(sdp_session_t * , char *);
extern return_type sdp_add_information(sdp_session_t * , char *);
extern return_type sdp_add_uri(sdp_session_t * , char *);
extern return_type sdp_add_email(sdp_session_t * , char *);
extern return_type sdp_add_phone(sdp_session_t * , char *);
extern return_type sdp_add_connection(sdp_session_t * , char *);
extern return_type sdp_add_bandwidth(sdp_session_t * , char *);
extern return_type sdp_add_repeat(sdp_session_t * , char *);
extern return_type sdp_add_time(sdp_session_t * , char *);
extern return_type sdp_add_zone(sdp_session_t * , char *);
extern return_type sdp_add_key(sdp_session_t * , char *);
extern return_type sdp_add_attribute(sdp_session_t * , char *);
extern return_type sdp_add_media(sdp_session_t * , char *);
extern return_type sdp_server_sender(char *);
extern return_type sdp_write_string_to_file(char * , char *);
extern return_type sdp_hash_append(char *);
extern return_type sdp_answer_decoder(char *);
extern return_type sdp_media_check(sdp_session_t *);
extern return_type sdp_client_check(struct sockaddr_in *);
extern return_type sdp_add_client(struct sockaddr_in *);
extern char *sdp_session_to_str(sdp_session_t *, int *);
extern void error_handling(int , int , char * ); 
extern void myfree(sdp_session_t *);
extern void logger(int, char *);

#endif
