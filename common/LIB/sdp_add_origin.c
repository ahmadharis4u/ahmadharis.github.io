/**********************************************************************
*
* FILE NAME     : sdp_add_origin.c
*
* DESCRIPTION   : To add the origin field to session structure.
*
* Date          Name        Reason              Reference                
*
* 19-Jan-10     Navjodh     Code Review         LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
**********************************************************************/

/*************HEADER FILES INCLUSION**************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_add_origin
*
* DESCRIPTION   : To add the origin field to session structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
**********************************************************************/
return_type sdp_add_origin(
                                sdp_session_t *sess, /*session variable*/
				char *file_name 	/*file name*/
			  )
{
    FILE    *fp = NULL;   /* File pointer variable */
    char    temp_char = INIT;    /* Stores characters read from file*/
    char    origin_field[SIZE];  /* Stores the value of origin
                                    attribute*/
    int     index = ZERO;       /* loop index variable*/
    int     var_count = ONE;    /* Attribute instance count*/

    TRACE("\nTRACE: Entering function sdp_add_origin\n");

    memset(origin_field, 0, SIZE);
    
    sess->s_origin = (sdp_origin_t *) calloc(ONE, sizeof(sdp_origin_t));
    
    if(NULL == sess->s_origin)
    {
    	error_handling(ERR_MAJOR,SDP_ORIGIN_MALLOC_ERROR,
		       "sdp_add_origin:Malloc error");
        TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
		return FAILURE;
    }	

    /*****OPENING THE INPUT FILE******/
    fp = fopen(file_name, "r");
    
    if(NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_ORIGIN_FILE_ERROR,
			"sdp_add_origin: File error");
        TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
		return FAILURE;
    }
    
    while(TRUE == TRUE) 
    {
        if((int)(temp_char = (char)fgetc(fp)) == -1)
        break;
	
	if(temp_char == 'o')
	{
	    (char)fgetc(fp);
	    
	    while(TRUE == TRUE)
	    {
	        
	    /***This for loop will read origin fields***/	
		
		for(index = 0, temp_char = (char)fgetc(fp); 
	    	    temp_char != ' ' && temp_char != '\\'; 
	    	    temp_char = (char)fgetc(fp), index++)
		{
	    	    origin_field[index] = temp_char;
		} /* end of for */
		
		origin_field[index] = '\0';
		
		if(temp_char == '\\')
		{
	    	    sess->s_origin->o_address = (char *) calloc(ONE, strlen(origin_field) + ONE);
	            if(NULL == sess->s_origin->o_address)
	    	    {
  		        error_handling(ERR_MAJOR,SDP_ORIGIN_MALLOC_ERROR,
        				"sdp_add_origin: Malloc error");
                        TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                        if(EOF == fclose(fp))
                        {
                            error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                            "sdp_add_origin: fclose error");
            
                            TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                            return FAILURE;
                        }   
			return FAILURE;
	    	    }
	    	    if (NULL == (strncpy(sess->s_origin->o_address,
                                         origin_field, 1 + strlen(origin_field))))
	    	    {
		    
   		        error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
	        				"sdp_add_origin: Strncpy error");
                        TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                        if(EOF == fclose(fp))
                        {
                            error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                            "sdp_add_origin: fclose error");
            
                            TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                            return FAILURE;
                        }   
                 	return FAILURE;
	    	    }
	   
                    if(EOF == fclose(fp))
                    {
                        error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                            "sdp_add_origin: fclose error");
            
                        TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                        return FAILURE;
                    }   
                    TRACE("\nTRACE: Exiting function sdp_add_origin with Success\n");
	    	    return SUCCESS;
		}

	       /***This switch case will assign origin fields to corresponding
		origin structure fields***/
		switch(var_count)
		{
	    	    case 1:
	        	sess->s_origin->o_username = (char *) calloc(ONE, 
							    strlen(origin_field) + ONE);
			
			if(NULL == sess->s_origin->o_username)
			{
   			    error_handling(ERR_MAJOR, SDP_ORIGIN_MALLOC_ERROR,
		    		  		"sdp_add_origin: Malloc error");
                            TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                            "sdp_add_origin: fclose error");
            
                                TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                                return FAILURE;
                            }   
	    	     	    return FAILURE;
			}	
	        	if(NULL ==
                           (strncpy(sess->s_origin->o_username,
                                    origin_field, 1 + strlen(origin_field))))
			{
			    error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
	          		    	   "sdp_add_origin: Strncpy error");
                       	    TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                "sdp_add_origin: fclose error");
            
                                TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                                return FAILURE;
                            }   
                 	    return FAILURE;
			}
			
	        	var_count++; 
                        break;
 
	    	    case 2:
		        sess->s_origin->o_id = (unsigned long)atol(origin_field);
                        var_count++; 
                        break;

		    case 3:
			sess->s_origin->o_version = (unsigned long)atol(origin_field);
		       	var_count++; 
                        break;

		    case 4:
			sess->s_origin->o_nettype = (char *) calloc(strlen(origin_field) + ONE, sizeof(char));
			if(NULL == sess->s_origin->o_nettype)
			{
    		   	    error_handling(ERR_MAJOR,SDP_ORIGIN_MALLOC_ERROR,
		         	  		"sdp_add_origin: Malloc error");
                            TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                "sdp_add_origin: fclose error");
            
                                TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                                return FAILURE;
                            }   
		   	    return FAILURE;
			}	
		        
			if (NULL == (strncpy(sess->s_origin->o_nettype,
				   			origin_field,
                                                        1 + strlen(origin_field))))
			{
			
    	        	    error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
		          			"sdp_add_origin: Strncpy error");
                       	    TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                 "sdp_add_origin: fclose error");
            
                                TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                                return FAILURE;
                            }   
	               	    return FAILURE;
			}
		        
			var_count++;
                        break;

		    case 5:
			sess->s_origin->o_addrtype = (char *) calloc(strlen(origin_field) + ONE, sizeof(char));
			if(NULL == sess->s_origin->o_addrtype)
			{
    		   	    error_handling(ERR_MAJOR,SDP_ORIGIN_MALLOC_ERROR,
			          		"sdp_add_origin: Malloc error");
                       	    TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                            "sdp_add_origin: fclose error");
            
                                TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                                return FAILURE;
                            }   
	    	   	    return FAILURE;
			}
			
	        	if (NULL == (strncpy(sess->s_origin->o_addrtype, 
				    	origin_field, 1 + strlen(origin_field))))
	        	{
		
    		            error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
	    	      		       "sdp_add_origin: Strncpy error");
                       	    TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                    "sdp_add_origin: fclose error");
            
                                TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
                                return FAILURE;
                            }   
	           	    return FAILURE;
			}
	        
			var_count++;
                        break;
		    } /* End of switch*/
	   	} /* end of while */
	    } /* End of if */
	
       	for(temp_char = (char)fgetc(fp); 
	   		temp_char != '\n'; 
	   		temp_char = (char)fgetc(fp));
    } /*End of outer while */
    
    if(EOF == fclose(fp))
    {
        error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                            "sdp_add_origin: fclose error");
            
        TRACE("\nTRACE: Exiting function sdp_add_origin with Failure\n");
        return FAILURE;
    }   

    TRACE("\nTRACE: Exiting function sdp_add_origin with Success\n");
    return SUCCESS;
			
}
