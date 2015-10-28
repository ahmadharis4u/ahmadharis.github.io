/**********************************************************************
*
* FILE NAME     : sdp_add_connection.c
*
* DESCRIPTION   : To add the connection field to session structure or
*                 media structure.
*
* Date          Name        Reason              Reference                
*
* 19-Jan-10     Neeraj      Code Review         LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/**************HEADER FILES INCLUSION*****************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_add_connection
*
* DESCRIPTION   : To add the connection field to session structure or 
*		  media structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
***********************************************************************/

/**************FUNCTION DEFINTION*********************/
return_type sdp_add_connection(
                                    sdp_session_t *sess, /*session variable*/
				    char *file_name /*file name*/
			      )
{
    FILE   *fp = NULL;  /* File pointer variable */
    char   temp_char = INIT; /* Stores characters read from file */
    char   conn_field[SIZE]; /* Stores value of connection filed
                                attribute*/
    int    index = ZERO;     /* loop index variable */
    int    count = ZERO;
    int    var_count = ZERO;  /* counts var instances*/
    int break_while = ZERO;   /* while loop break indication */

    /*Pointer variables for sdp connection attribute values*/
    sdp_conn_t   *ptr = NULL;
    sdp_conn_t   *ptr_temp = NULL;

    TRACE("\nTRACE: Entering function sdp_add_connection\n");
    
    /***********OPENING INPUT FILE*************/
    fp = fopen(file_name,"r");
    if(NULL == fp)
    {
		error_handling(ERR_MAJOR, SDP_CONNECTION_FILE_ERROR, 
		       "sdp_add_connection: File Error ");
        TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
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
                                "sdp_add_connection: fclose error");
            
                TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                return FAILURE;
            }
            TRACE("\nTRACE: Exiting function sdp_add_connection with Success\n");
	    return SUCCESS;
	}
	
	if(temp_char == 'c')
	{
	    var_count = 1;
	    (void)fgetc(fp);
	    ptr = (sdp_conn_t *) calloc(ONE, sizeof(sdp_conn_t));
            
            if(NULL == ptr)
            {
   		error_handling(ERR_MAJOR, SDP_CONNECTION_MALLOC_ERROR,
				    "sdp_add_connection: Malloc Error ");
                TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");

                if(EOF == fclose(fp))
                {
                    error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                     "sdp_add_connection: fclose error");
            
                    TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                    return FAILURE;
                }
                
 	        return FAILURE;         
            }

            while(TRUE == TRUE)
            {
                
	     /*This for loop is reading all the fields of connection attribute*/	
				for(index=0, temp_char = (char)fgetc(fp); 
		    		 temp_char != ' ' && temp_char != '/' 
		    		 && temp_char != '\\';temp_char = (char)fgetc(fp), index++)
                   conn_field[index] = temp_char;

                conn_field[index] = '\0';

                if(temp_char == '\\')
                {
		    break_while = ONE;
                }
	
	        /*This switch will assign fields of connection 
                to the corresponding connection structure fields*/
                switch(var_count)
                {
                    case 1:
                        ptr->c_nettype = (char *) calloc(strlen(conn_field) + ONE, sizeof(char));
                        if(NULL == ptr->c_nettype)
                        { 
			    error_handling(ERR_MAJOR, SDP_CONNECTION_MALLOC_ERROR, 
                                           "sdp_add_connection: Malloc Error ");
			    free(ptr);

                            TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                "sdp_add_connection: fclose error");
                                TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                                return FAILURE;
                            }

		            return FAILURE;
                        }

                        if (strncpy(ptr->c_nettype, conn_field, 1 + strlen(conn_field)) == NULL)
	                {
                            error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
					        "sdp_add_connection : strncpy error");
			    free(ptr);
                            TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                "sdp_add_connection: fclose error");
                                TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                                return FAILURE;
                            }
			return FAILURE;
                        }

                        var_count++; 
                        break;
                      
                    case 2:
                        ptr->c_addrtype = (char *) calloc(strlen(conn_field) +
										ONE,sizeof(char));
                        
                        if (NULL == ptr->c_addrtype)
                        {
		            error_handling(ERR_MAJOR, SDP_CONNECTION_MALLOC_ERROR,
                                                "sdp_add_connection: Malloc Error ");
			    free(ptr);
                            TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                "sdp_add_connection: fclose error");
                                TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                                return FAILURE;
                            }
			    return FAILURE;
                        }

                        if (NULL== strncpy (ptr->c_addrtype, conn_field, 
                                            1 +strlen(conn_field)))
			{
			
			    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			    "sdp_add_connection: Strncpy Error ");
			    free(ptr);
                            TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                "sdp_add_connection: fclose error");
                                TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                                return FAILURE;
                            }
			    return FAILURE;
                        }

			var_count++;
                        break;
		    
		    case 3:
                        ptr->c_address = (char *) calloc(strlen(conn_field) +
										ONE, sizeof(char));
                    
                        if(NULL == ptr->c_address)
                        {
           		    error_handling(ERR_MAJOR, SDP_CONNECTION_MALLOC_ERROR,
                                                "sdp_add_connection: Malloc Error");
		            free(ptr);
                            TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                "sdp_add_connection: fclose error");
                                TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                                return FAILURE;
                            }
			    return FAILURE;    
                        }

                        if (strncpy(ptr->c_address, conn_field, 1 + strlen(conn_field)) == NULL)
		        {	
			    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
                                            "sdp_add_connection: Strncpy Error ");
			    free(ptr);

                            TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                            if(EOF == fclose(fp))
                            {
                                error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                                                "sdp_add_connection: fclose error");
                                TRACE("\nTRACE: Exiting function sdp_add_connection with Failure\n");
                                return FAILURE;
                            }
			    return FAILURE;
                        }

                        var_count++; 
		      	break;

 		    case 4:
 		        ptr->c_ttl = atoi(conn_field);
 		       	var_count++;
 		        break;

 		    case 5:
 		        ptr->c_addrcount = atoi(conn_field);
 		        break;

                }// end of switch

	    		if(break_while == ONE)
					break;

            }//end of while

	    ptr->c_next = NULL;
	    break_while = ZERO;

	    if (count == ZERO)
	    {
	       	sess->s_conn = ptr;
		count++;
	    }
	    else
	    {
		for (ptr_temp = sess->s_conn; ptr_temp->c_next != NULL; 
					ptr_temp = ptr_temp->c_next);
                
				ptr_temp->c_next = ptr;
	    }

	   
	}//end of if
        
	for (temp_char = (char)fgetc(fp); temp_char != '\n'; 
			temp_char = (char)fgetc(fp));
    }//end of while
}//end of function
