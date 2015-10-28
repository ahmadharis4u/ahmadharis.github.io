/**********************************************************************
*
* FILE NAME     : sdp_add_repeat.c
*
* DESCRIPTION   : To add the repeat field to session structure.
*
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris			Modfied				LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
**********************************************************************/

/************HEADER FILES INCLUSION*************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_add_repeat
*
* DESCRIPTION   : To add the repeat field to session structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
**********************************************************************/
return_type sdp_add_repeat(sdp_session_t *sess, /*session vairable*/
                           char *file_name      /*file name*/
						   )
{
    FILE   *fp = NULL;            /*file pointer*/
    char   temp_char = INIT;      /*for fgetc*/
    char   repeat_field[SIZE];    /*repeat_field buffer*/
    int    index = ZERO;          /*buffer index*/
    int    count = ZERO;          /*node count*/
	
	/*sdp_list_t pointers to store repeat info*/
    sdp_list_t *ptr = NULL;
    sdp_list_t *ptr_temp = NULL;

    TRACE("\nTRACE: Entering function sdp_add_repeat\n");
    memset(repeat_field, 0, SIZE);
	
	sess->s_time->t_repeat = (sdp_repeat_t *) calloc(ONE,sizeof(sdp_repeat_t));
    if(NULL == sess->s_time->t_repeat)
    {
         error_handling(ERR_MAJOR,SDP_REPEAT_MALLOC_ERROR,
                        "sdp_add_repeat: Malloc error");
         TRACE("\nTRACE: Exiting function sdp_add_repeat with failure\n");
         return FAILURE;
    }
    /***********OPENING THE INPUT FILE***********/
    fp = fopen(file_name,"r");
    /*error handling*/
    if(NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_REPEAT_FILE_ERROR,
                                "sdp_add_repeat: File error");
        TRACE("\nTRACE: Exiting function sdp_add_repeat with failure\n");
        return FAILURE;
    }
	
    /*reading the file*/
    while(TRUE == TRUE)
    {
        temp_char = (char)fgetc(fp);
        
		if(temp_char == 'm')
        {
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   	"sdp_add_repeat: fclose error ");
          	  	TRACE("\nTRACE: Exiting function sdp_add_repeat with Failure\n");
				return FAILURE;
			}
        	TRACE("\nTRACE: Exiting function sdp_add_repeat with Success\n");
            return SUCCESS;
        }
        /*checking for repeat field*/
		if(temp_char == 'r')
        {
            (void)fgetc(fp);

            /***Assign fields of repeat attribute to session structure***/
            for(index = 0, temp_char = (char)fgetc(fp); 
	        	temp_char != ' '; 
				temp_char = (char)fgetc(fp), index++)
	    	{
                repeat_field[index] = temp_char;
	    	}

            repeat_field[index] = '\0';
            sess->s_time->t_repeat->r_interval = (unsigned long)atol(repeat_field);
			/*reading a word*/
            for(index = 0, temp_char = (char)fgetc(fp); 
                temp_char != ' '; 
	            temp_char = (char)fgetc(fp), index++)
	    	{
                repeat_field[index] = temp_char;
	    	}

            repeat_field[index] = '\0';
            
	    	sess->s_time->t_repeat->r_duration = (unsigned long)atol(repeat_field);
            
			while(TRUE == TRUE)
            {
                ptr = (sdp_list_t *) calloc(ONE, sizeof(sdp_list_t));
                /*error handling*/
				if(NULL == ptr)
                {
                    error_handling(ERR_MAJOR,SDP_REPEAT_MALLOC_ERROR,
                               "sdp_add_repeat: Calloc error");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   				"sdp_add_repeat: fclose error ");
					}
                    TRACE("\nTRACE: Exiting function sdp_add_repeat with failure\n");
                    return FAILURE;
                }

                for(index = 0, temp_char = (char)fgetc(fp); 
		    		temp_char != ' ' && temp_char != '\\'; 
		    		temp_char = (char)fgetc(fp), index++)
	        		{
                    	repeat_field[index] = temp_char;
					}
                
				repeat_field[index] = '\0';
                ptr->value = (char *) calloc(strlen(repeat_field) + ONE, sizeof(char));
				/*error handling*/
                if(NULL == ptr->value)
                {
                    error_handling(ERR_MAJOR,SDP_REPEAT_MALLOC_ERROR,
                                "sdp_add_repeat: Calloc error");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   				"sdp_add_repeat: fclose error ");
					}
                    TRACE("\nTRACE: Exiting function sdp_add_repeat with failure\n");
                    free(ptr);
                    return FAILURE;
                }

                if(NULL == (strncpy(ptr->value, repeat_field,
					strlen(repeat_field) + 1)))
				{
                    error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
                                "sdp_add_repeat: Strncpy error");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   				"sdp_add_repeat: fclose error ");
					}
                    TRACE("\nTRACE: Exiting function sdp_add_repeat with failure\n");
                    free(ptr);
                    return FAILURE;
				}
                ptr->next = NULL;
				
				/*going to the last node*/
                if(count == ZERO)
                {
                    sess->s_time->t_repeat->r_offset = ptr;
                    count++;
                }
                else
                {
                    for(ptr_temp = sess->s_time->t_repeat->r_offset; 
		        		ptr_temp->next != NULL; 
						ptr_temp = ptr_temp->next); /* end of for */
                   
		   			ptr_temp->next = ptr;
                }
                if(temp_char == '\\')
                {
                    break;
                }
            }

            sess->s_time->t_repeat->r_next = NULL;
        }
        for(temp_char = (char)fgetc(fp); 
	    	temp_char != '\n'; 
	    	temp_char = (char)fgetc(fp)); /* end of for */
    } /* end of while */
			
	if(EOF == fclose(fp))
	{
		error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
  		   				"sdp_add_repeat: fclose error ");
		TRACE("\nTRACE: Exiting function sdp_add_repeat with Failure\n");
		return FAILURE;
	}
	TRACE("\nTRACE: Exiting function sdp_add_repeat with success\n");
	return SUCCESS;
}
