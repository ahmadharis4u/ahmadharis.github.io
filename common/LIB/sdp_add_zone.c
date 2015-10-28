/************************************************************************
*
* FILE NAME : sdp_add_zone.c
*
* DESCRIPTION   : To add the zone field to session structure.
*
* Date          Name        Reason              Reference                
*
* 19-Jan-10     Neeraj      Code Review         LLD
*
* Copyright(C) 2010 Aricent Inc. All Rights Reserved.
*
***********************************************************************/

/******************HEADER FILE INCLUSION*************/
#include"./../HEADER/sdp.h"


/***********************************************************************
*
* FUNCTION NAME : sdp_add_zone
*
* DESCRIPTION   : To add the zone field to session structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
***********************************************************************/
return_type sdp_add_zone(sdp_session_t *sess, 	/*session variable*/
			char *file_name 		/*file name*/
			)
{
    FILE   *fp = NULL;       /* File pointer variable */
    char   temp_char = INIT; /* Stores characters read from file */
    char   zone_field[SIZE]; /* stores zone field attribute*/
    int    index = ZERO;     /* loop index variable */
    
    /* SDP zone attribute pointer variables*/
    sdp_zone_t *ptr = NULL;
    sdp_zone_t *ptr_temp = NULL; 

    int count = ZERO; /* Attribute instance counter */
    
    TRACE("\nTRACE: Entering function sdp_add_zone\n");   
    fp = fopen(file_name, "r");

    if (NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_ZONE_FILE_ERROR,
			"sdp_add_zone: File error");
        TRACE("\nTRACE: Exiting function sdp_add_zone with failure\n");   
		return FAILURE;
    }

    while (TRUE == TRUE)
    {
        if ((int)(temp_char = (char)fgetc(fp)) == -1)
            break;

        if (temp_char == 'z')
        {
            (void)fgetc(fp);

	    while(TRUE == TRUE)
	    {
	        ptr = (sdp_zone_t *) calloc(ONE, sizeof(sdp_zone_t));

                if(NULL == ptr)
                {
                    error_handling(ERR_MAJOR,SDP_ZONE_MALLOC_ERROR,
					"sdp_add_zone: Malloc error ");
        	    TRACE("\nTRACE: Exiting function sdp_add_zone with failure\n");
                    if(EOF == fclose(fp))
                    {
                        error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                       "sdp_add_zone: fclose error");
            
                        TRACE("\nTRACE: Exiting function sdp_add_zone with Failure\n");
                        return FAILURE;
                    }
		    return FAILURE;
                }
                
		for(index = 0, temp_char = (char)fgetc(fp); 
		    temp_char  != ' '; temp_char = (char)fgetc(fp), index ++)
                zone_field[index] = temp_char;
                zone_field[index] = '\0';

	        ptr->z_time = (unsigned long)atol(zone_field);
                
		for(index = 0, temp_char = (char)fgetc(fp); temp_char != ' ' 
                    && temp_char != '\\'; temp_char = (char)fgetc(fp), index++)
                zone_field[index] = temp_char;
                
		zone_field[index] = '\0';

                ptr->z_offset = (char *) calloc(strlen(zone_field) + ONE, sizeof(char));
                if(NULL == ptr->z_offset)
                {
                    error_handling(ERR_MAJOR,SDP_ZONE_MALLOC_ERROR,
					"sdp_add_zone:Malloc error ");
        	    TRACE("\nTRACE: Exiting function sdp_add_zone with failure\n");   
                    if(EOF == fclose(fp))
                    {
                        error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                       "sdp_add_zone: fclose error");
            
                        TRACE("\nTRACE: Exiting function sdp_add_zone with Failure\n");
                        return FAILURE;
                    }
		    free(ptr);
		    return FAILURE;
                }
                
		if (NULL == strncpy(ptr->z_offset, zone_field,
                                    1+strlen(zone_field)))
                {
                    error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
					"sdp_add_zone:strncpy error ");
        	    TRACE("\nTRACE: Exiting function sdp_add_zone with failure\n");   
		    free(ptr);
                    if(EOF == fclose(fp))
                    {
                        error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                       "sdp_add_zone: fclose error");
            
                        TRACE("\nTRACE: Exiting function sdp_add_zone with Failure\n");
                        return FAILURE;
                    }
		    return FAILURE;
                }
		    ptr->z_next = NULL;
	
		if (count == ZERO)
	    	{
	            sess->s_zone = ptr;
                    ptr_temp = ptr;
                    count++;
	        }
	        else 
	        {
		    if(ptr_temp != NULL) 
		    {	
	                ptr_temp->z_next = ptr;
                        ptr_temp = ptr;
		    }
		    
	        }

	        if(temp_char == '\\')
	        {
	            break;
	        }
	    }//end of while

        }// end of if

        for (temp_char = (char)fgetc(fp); temp_char != '\n'; 
	     temp_char = (char)fgetc(fp));

    }//end of while

    if(EOF == fclose(fp))
    {
        error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                        "sdp_add_zone: fclose error");
            
        TRACE("\nTRACE: Exiting function sdp_add_zone with Failure\n");
                return FAILURE;
    }
    TRACE("\nTRACE: Exiting function sdp_add_zone with success\n");   
    return SUCCESS;

}//end of function
