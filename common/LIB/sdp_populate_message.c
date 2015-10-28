/**********************GLOBAL VARIABLES************************************
* FILE NAME	: sdp_populate_message.c
*
* DESCRIPTION   : Populates the sdp_session_t variable
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris			Modfied				LLD
*
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/**************HEADER FILE DECALARATIONS************/

#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_populate_message
*
* DESCRIPTION   : It encodes sdp message
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t *, char *
*
**********************************************************************/



return_type sdp_populate_message(sdp_session_t *my_session, /*sdp_session variable*/
                                 char *file_name            /*file name*/
								 )
{
    TRACE("\nTRACE: Entering function sdp_populate_message\n"); 
    
	LOGGER(LOG_CRITICAL, "Start of sdp_add_version"); 	
	if(sdp_add_version(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_VERSION_ERROR,  
    		   	"sdp_populate_message: sdp_add_version error ");
       	LOGGER(LOG_CRITICAL, "End of sdp_add_version"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}
        LOGGER(LOG_CRITICAL, "End of sdp_add_version"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_origin"); 	
	if(sdp_add_origin(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_ORIGIN_ERROR,  
    		   	"sdp_populate_message: sdp_add_origin error ");
       	LOGGER(LOG_CRITICAL, "End of sdp_add_origin"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}
        LOGGER(LOG_CRITICAL, "End of sdp_add_origin"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_name"); 	
	if(sdp_add_name(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_NAME_ERROR,  
    		   	"sdp_populate_message: sdp_add_name error ");
	    LOGGER(LOG_CRITICAL, "End of sdp_add_name"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}
        LOGGER(LOG_CRITICAL, "End of sdp_add_name"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_information"); 	
	if(sdp_add_information(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_INFO_ERROR,  
    		   	"sdp_populate_message: sdp_add_information error ");
	    LOGGER(LOG_CRITICAL, "End of sdp_add_information"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_information"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_uri"); 	
	if(sdp_add_uri(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_URI_ERROR,  
    		   	"sdp_populate_message: sdp_add_information error ");
	    LOGGER(LOG_CRITICAL, "End of sdp_add_uri"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_uri"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_email"); 	
	if(sdp_add_email(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_EMAIL_ERROR,  
    		   	"sdp_populate_message: sdp_add_email error ");
	    LOGGER(LOG_CRITICAL, "End of sdp_add_email"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_email"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_phone"); 	
	if(sdp_add_phone(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_PHONE_ERROR,  
    		   	"sdp_populate_message: sdp_add_phone error ");
	    LOGGER(LOG_CRITICAL, "End of sdp_add_phone"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_phone"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_connection"); 	
	if(sdp_add_connection(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_CONNECTION_ERROR,  
    		   	"sdp_populate_message: sdp_add_connection error ");
        LOGGER(LOG_CRITICAL, "End of sdp_add_connection"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_connection"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_bandwidth"); 	
	if(sdp_add_bandwidth(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_BANDWIDTH_ERROR,  
    		   	"sdp_populate_message: sdp_add_bandwidth error ");
        LOGGER(LOG_CRITICAL, "End of sdp_add_bandwidth"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_bandwidth"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_time"); 	
	if(sdp_add_time(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_TIME_ERROR,  
    		   	"sdp_populate_message: sdp_add_time error ");
        LOGGER(LOG_CRITICAL, "End of sdp_add_time"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_time"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_repeat"); 	
	if(sdp_add_repeat(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_REPEAT_ERROR,  
    		   	"sdp_populate_message: sdp_add_repeat error ");
       	LOGGER(LOG_CRITICAL, "End of sdp_add_repeat"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_repeat"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_zone"); 	
	if(sdp_add_zone(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_ZONE_ERROR,  
    		   	"sdp_populate_message: sdp_add_zone error ");
       	LOGGER(LOG_CRITICAL, "End of sdp_add_zone"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_zone"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_key"); 	
	if(sdp_add_key(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_KEY_ERROR,  
    		   	"sdp_populate_message: sdp_add_key error ");
       	LOGGER(LOG_CRITICAL, "End of sdp_add_key"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_key"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_attribute"); 	
	if(sdp_add_attribute(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_ATTRIBUTE_ERROR,  
    		   	"sdp_populate_message: sdp_add_attribute error ");
        LOGGER(LOG_CRITICAL, "End of sdp_add_attribute"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}	
        LOGGER(LOG_CRITICAL, "End of sdp_add_attribute"); 	
	
        LOGGER(LOG_CRITICAL, "Start of sdp_add_media"); 	
	if(sdp_add_media(my_session, file_name) != 0)
	{
		error_handling(ERR_CRITICAL, SDP_MEDIA_ERROR,  
    		   	"sdp_populate_message: sdp_add_media error ");
	    LOGGER(LOG_CRITICAL, "End of sdp_add_media"); 	
    	TRACE("\nTRACE: Exiting function sdp_populate_message with Failure\n"); 
		return FAILURE;
	}
        LOGGER(LOG_CRITICAL, "End of sdp_add_media"); 	
   	
	TRACE("\nTRACE: Exiting function sdp_populate_message with Success\n"); 
	return SUCCESS;
}	
