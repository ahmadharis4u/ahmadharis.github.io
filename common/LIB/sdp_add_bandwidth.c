/**********************************************************************
*
* FILE NAME     : sdp_add_bandwidth.c
*
* DESCRIPTION   : To add the bandwidth field to session structure or
*                 media structure.
*
* Date          Name        Reason              Reference                
*
* 19-Jan-10     Navjodh     Code Review         LLD
* 19-Jan-10     Navjodh     Error Handling      LLD
*
* Copyright(C) 2010 Aricent Inc. All Rights Reserved.
*
**********************************************************************/

/**************HEADER FILE INCLUSION********************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_add_bandwidth
*
* DESCRIPTION   : To add the bandwidth field to session structure or 
*                  media structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
***********************************************************************/
/**************FUNCTION DEFINITION**********************/

return_type sdp_add_bandwidth(
                                sdp_session_t *sess, /*session vairable*/
			        char *file_name /*file name*/
			      )
{
    FILE   *fp = NULL;    /* file pointer variable*/
    char   temp_char = INIT; /* stores characters read from file */
    char   bw_field[SIZE];   /* stores value bandwidth field*/
    int    index = ZERO;     /* index variable*/
    int    count = ZERO;     /* stores attribute count*/

    /*Pointers to bandwidth attribute values*/
    sdp_bandwidth_t    *ptr = NULL; 
    sdp_bandwidth_t    *ptr_temp = NULL;

    TRACE("\nTRACE: Entering function sdp_add_bandwidth\n");
    
    /*Initialize the char array*/
    memset(bw_field, 0, SIZE);
    
    /************OPENING THE INPUT FILE*****************/
    fp = fopen(file_name,"r");

    if(NULL == fp)
    {
	error_handling(ERR_MAJOR, SDP_BANDWIDTH_FILE_ERROR,
			       "sdp_add_bandwidth: File error ");
        TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Failure\n");
 	
        return FAILURE;
    }
    
    while(TRUE == TRUE)
    {
	temp_char = (char)fgetc(fp);

	if(temp_char == 'm')
	{
            if(EOF == fclose(fp))
            {
                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                               "sdp_add_bandwidth: fclose error");
            
                TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Failure\n");
                return FAILURE;
            }
            
            TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Success\n");
	    return SUCCESS;
	}

	if(temp_char == 'b')
	{
	    (char)fgetc(fp);
	    ptr = (sdp_bandwidth_t *) calloc(ONE,sizeof(sdp_bandwidth_t));
	    
            if(NULL == ptr)
            {
		error_handling(ERR_MAJOR, SDP_BANDWIDTH_MALLOC_ERROR,
		    		       "sdp_add_bandwidth: Malloc Error ");
                TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Failure\n");
                if(EOF == fclose(fp))
                {
                    error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                    "sdp_add_bandwidth: fclose error");
            
                    TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Failure\n");
                    return FAILURE;
                }
		return FAILURE;
            }
             
	    /***This for loop will read the first field of bandwidth**/ 

	    for(index = 0, temp_char = (char)fgetc(fp); 
		temp_char != ':'; temp_char = (char)fgetc(fp), 
                index++)
		bw_field[index] = temp_char;

    	    bw_field[index] = '\0'; /* Terminate with a null char */
	    ptr->b_type = (char *) calloc(strlen(bw_field) + ONE, sizeof(char));
            
	    if(NULL == ptr->b_type)
            {
           	error_handling(ERR_MAJOR, SDP_BANDWIDTH_MALLOC_ERROR, 
 				    "sdp_add_bandwidth: Malloc Error ");
            	TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Failure\n");
		free(ptr);
                /* Close file */
                if(EOF == fclose(fp))
                {
                    error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                    "sdp_add_bandwidth: fclose error");
            
                    TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Failure\n");
                    return FAILURE;
                }
		return FAILURE; 
       	    }
        
	    if(strcpy(ptr->b_type , bw_field) == NULL)
	    {
        	error_handling(ERR_MAJOR, SDP_STRCPY_ERROR, 
 				    "sdp_add_bandwidth: strcpy Error ");

         	TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Failure\n");
		free(ptr);
                /* Close file */
                if(EOF == fclose(fp))
                {
                    error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                    "sdp_add_bandwidth: fclose error");
            
                    TRACE("\nTRACE: Exiting function sdp_add_bandwidth with Failure\n");
                    return FAILURE;
                }
	        return FAILURE;
	    }
	    
	    
	    /***This for loop read the second field***/

	    for(index = 0, temp_char= (char)fgetc(fp); 
		temp_char != '\\'; temp_char = (char)fgetc(fp), index++)
	    bw_field[index] = temp_char;

       	    bw_field[index] = '\0';
            ptr->b_value = (unsigned int) atoi(bw_field);
	    ptr->b_next = NULL;
	    
	    if(count == 0)
	    {
	    	sess->s_bw = ptr;
		count++;
	    }
	    else
	    {
	        for(ptr_temp = sess->s_bw; ptr_temp->b_next != NULL; 
		    ptr_temp = ptr_temp->b_next);
		    ptr_temp->b_next = ptr;
	    }

	}//end of if

        else
	{
            ;
	}
        
	 /*** This for loop will move the pointer to the starting of the
	        next line of the input file***/
		for(temp_char = (char)fgetc(fp); temp_char != '\n'; 
			temp_char = (char)fgetc(fp));
    }//end of while
}// end of function
