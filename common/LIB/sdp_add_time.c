/***********************************************************************
*
* FILE NAME : sdp_add_time
*
* DESCRIPTION   : To add the time field to session structure.
*
* Date          Name        Reason              Reference                
*
* 19-Jan-10     Navjodh     Code Review         LLD
*
* Copyright(C) 2010 Aricent Inc. All Rights Reserved.
*
***********************************************************************/

/********************HEADER FILES INCLUSION******************/
#include"./../HEADER/sdp.h"


/***********************************************************************
*
* FUNCTION NAME : sdp_add_time
*
* DESCRIPTION   : To add the time field to session structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
***********************************************************************/
return_type sdp_add_time(sdp_session_t *sess, 	/*session variable*/
			    char *file_name 	/*file name*/
			)
{
    FILE   *fp = NULL; /*file pointer variable*/
    char   temp_char = INIT;  /*stores characters read from file*/
    char   time_field[SIZE];  /*stores the value of time field*/
    int    index = ZERO;      /*loop index variable*/

    TRACE("\nEntering function sdp_add_time\n");
    /*Initialize  time field variable*/
    memset(time_field, 0, SIZE);
    
    /*****OPENING THE INPUT FILE*****/
    fp = fopen(file_name, "r");
    
    if(NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_TIME_FILE_ERROR,
				 " sdp_add_time: File error");
 		return FAILURE;
    }
    
    while(TRUE == TRUE)
    {
        if((int)(temp_char = (char)fgetc(fp)) == -1)
            break;
	
	if(temp_char == 't')
	{
	    (void)fgetc(fp);
	    sess->s_time = (sdp_time_t *) calloc(ONE, sizeof(sdp_time_t));
            
	    if(NULL == fp)
            {
                error_handling(ERR_MAJOR,SDP_TIME_MALLOC_ERROR,
					 "sdp_add_time: Malloc Error" );
    	        TRACE("\nExiting function sdp_add_time with failure\n");
                if(EOF == fclose(fp))
                {
                    error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                               "sdp_add_time: fclose error");
            
                    TRACE("\nTRACE: Exiting function sdp_add_time with Failure\n");
                    return FAILURE;
                }
  	        return FAILURE;
            }

     /***Assigns first field of time attribute to session structure***/ 
	    for(index = 0, temp_char = (char)fgetc(fp); 
	        temp_char != ' '; 
		temp_char = (char)fgetc(fp), index++)
	    {
		time_field[index] = temp_char;
	    } /* end of for */
            
	    time_field[index] = '\0';
	    
	    if(sess->s_time != NULL)
	    	sess->s_time->t_start = (unsigned long)atol(time_field);
      
     /***Assigns first field of time attribute to session structure***/
   	    for(index = 0, temp_char = (char)fgetc(fp); 
	        temp_char != '\\'; 
		temp_char = (char)fgetc(fp), index++)
            {
		time_field[index] = temp_char;
	    }
	    
            time_field[index] = '\0';
	    
	    if(sess->s_time != NULL)
	    {
	    	sess->s_time->t_stop = (unsigned long)atol(time_field);
	   		sess->s_time->t_next = NULL;
	    }
	}
	for(temp_char = (char)fgetc(fp); 
	    temp_char != '\n'; 
	    temp_char = (char)fgetc(fp));
    } /* end of for */
	
    if(EOF == fclose(fp))
    {
        error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                               "sdp_add_time: fclose error");
            
        TRACE("\nTRACE: Exiting function sdp_add_time with Failure\n");
        return FAILURE;
    }

    TRACE("\nExiting function sdp_add_time with success\n");
    return SUCCESS;
}
