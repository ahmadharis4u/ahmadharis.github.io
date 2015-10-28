/**********************************************************************
*
* FILE NAME     : sdp_add_media.c
*
* DESCRIPTION   : To add the media field to session structure.
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris,		Modfied				LLD
*				Deepshikha
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/*************HEADER FILES INCLUSION*************/
#include"./../HEADER/sdp.h"




/***********************************************************************
*
* FUNCTION NAME : sdp_add_media
*
* DESCRIPTION   : To add the media field to session structure.
*
* RETURN TYPE   : return_type
*
***********************************************************************/
/*************FUNCTION DEFINTION*****************/
return_type sdp_add_media(sdp_session_t *sess,	 	/*session variable*/
							char *file_name 		/*file name*/
							)
{
    FILE   *fp = NULL;		              /*file pointer*/
    char   temp_char = INIT;              /*for fgetc*/
    char   media_field[SIZE];             /*storage buffer*/
    int    index = ZERO;                  /*loop index*/
    int    count = ZERO;                  /*loop count*/
    int    format_count = ZERO;           /*for proper format check*/
    int    attr_count = ZERO;             /*for attribute count*/
    int    var_count = ZERO;              /*for variable counting*/
    int    break_while = ZERO;            /*flag*/

	/*declaring variables used for media addition*/
    sdp_media_t   *ptr = NULL;
    sdp_media_t   *ptr_temp = NULL;
    sdp_list_t    *ptr_format =NULL;
    sdp_list_t    *ptr_format_temp = NULL;
    sdp_attr_t    *ptr_attr = NULL;
    sdp_attr_t    *ptr_attr_temp = NULL;

    TRACE("\nTRACE: Entering function sdp_add_media\n");    
    
	memset(media_field, 0, SIZE);
	
    /********OPENING THE INPUT FILE*********/
    fp = fopen(file_name,"r");

    if(NULL == fp)
    {
		error_handling(ERR_MAJOR, SDP_MEDIA_FILE_ERROR,
		       "sdp_add_media: File error ");
        TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");    
        return FAILURE;
    }
	
	/**Reading file**/
    while(TRUE == TRUE)
    {
        temp_char = (char)fgetc(fp);
	
		if(temp_char == 'm')
		{
	   		(void)fgetc(fp);
	    	ptr = (sdp_media_t *) calloc(ONE, sizeof(sdp_media_t));
            
			/*error handling*/
            if(NULL == ptr)
            {
				error_handling(ERR_MAJOR, SDP_MEDIA_MALLOC_ERROR,
			    		   "sdp_add_media: Malloc error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
			    		   	"sdp_add_media: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");    
        		return FAILURE;
            }
			
	    	/*going to last media node*/
	    	if(count == ZERO)
	    	{
	       		sess->s_media = ptr;
				count++;
	    	}
	       	else
	    	{
				for(ptr_temp = sess->s_media; 
		    		ptr_temp->m_next != NULL; 
		    		ptr_temp = ptr_temp->m_next);
				{
					ptr_temp->m_next = ptr;
				}/* end of for */
	    	}
	    

 	    /***This for loop will read m_name field***/    
	    	for(index = 0, temp_char = (char)fgetc(fp); 
	        	temp_char != ' '; 
				temp_char = (char)fgetc(fp), index++)
	    	{
	        	media_field[index] = temp_char;
	    	}	
            media_field[index] = '\0';
	    	ptr->m_name = (char *) calloc(strlen(media_field) + ONE, 
	    				  sizeof(char));
            		
            if(NULL == ptr->m_name)
            {
				error_handling(ERR_MAJOR, SDP_MEDIA_MALLOC_ERROR,
			    		   "sdp_add_media: Malloc error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
			    		   	"sdp_add_media: fclose error ");
				}
	        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
        		return FAILURE;
            }
	    	if ((strcpy(ptr->m_name, media_field)) == NULL)
	    	{
				error_handling(ERR_MAJOR, SDP_STRCPY_ERROR,
			    	   "sdp_add_media: Strcpy error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
			    		   	"sdp_add_media: fclose error ");
				}
	        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
        		return FAILURE;
	    	}
	    
	    /***This for loop will read m_port field***/     
            
	    	for(index = 0, temp_char = (char)fgetc(fp); 
	        	temp_char != ' '; 
				temp_char = (char)fgetc(fp),index++)
	    	{
	        	media_field[index] = temp_char;
	    	} /* end of for */
            
	    	media_field[index] = '\0';
	    	ptr->m_port = (int)atoi(media_field);
	    
 
            /***This for loop will read  m_proto field***/   
	    
	    	for(index = 0, temp_char = (char)fgetc(fp); 
	        	temp_char != ' '; 
				temp_char = (char)fgetc(fp), index++)
	    	{
	        	media_field[index] = temp_char;
	    	} /* end of for */
	    
            media_field[index] = '\0';
	    	ptr->m_proto = (char *) calloc(strlen(media_field) + ONE, 
	    				   sizeof(char));
            
            if(NULL == ptr->m_proto)
            {
				error_handling(ERR_MAJOR, SDP_MEDIA_MALLOC_ERROR, 
			 		      "sdp_add_media: Malloc error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
			    		   	"sdp_add_media: fclose error ");
				}
				TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
                return FAILURE;
            }
	    
	    	if((strcpy(ptr->m_proto, media_field)) == NULL)
	    	{
				error_handling(ERR_MAJOR, SDP_STRCPY_ERROR, 
			    	   "sdp_add_media: Strcpy error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
			    		   	"sdp_add_media: fclose error ");
				}
				TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
                return FAILURE;
	    	}
	    
	    	format_count = ZERO;

	    /***This while loop will read m_format fields***/    
	    
	    	while (TRUE == TRUE)
	    	{
	        	ptr_format = (sdp_list_t *) calloc(ONE, sizeof( sdp_list_t));
                
                if(NULL == ptr_format)
                {
		    		error_handling(ERR_MAJOR, SDP_MEDIA_MALLOC_ERROR,
		    				   "sdp_add_media: Malloc error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
				    		   	"sdp_add_media: fclose error ");
					}
		    		TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	            return FAILURE;
                }
		
                for (index = 0, temp_char = (char)fgetc(fp); 
		    		temp_char != ' ' && temp_char != '\\'; 
		    		temp_char = (char)fgetc(fp), index++)
				{
	            	media_field[index] = temp_char;
				} /* end of for */
		
				media_field[index] = '\0';
				ptr_format->value = (char *) calloc(strlen(media_field) + ONE, 
								    sizeof(char));
                
                if (NULL == ptr_format->value)
                {
		    		error_handling(ERR_MAJOR, SDP_MEDIA_MALLOC_ERROR,
		    				   "sdp_add_media: Malloc error ");
		   			free(ptr_format);
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
				    		   	"sdp_add_media: fclose error ");
					}
		   			TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	            return FAILURE;
                }
				/*copying media_field*/
				if (NULL == (strncpy(ptr_format->value, media_field, 
							strlen(media_field) + 1)))
				{
					error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			       		        "sdp_add_media: strncpy error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
				    		   	"sdp_add_media: fclose error ");
					}
					TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
               		return FAILURE;
				}
		
				ptr_format->next = NULL;
				/*checking m_format list*/
				if(format_count == ZERO)
				{
		    		ptr->m_format = ptr_format;
		    		ptr_format_temp = ptr_format;
		    		format_count++;
				}
				else
				{ 
		    		if(NULL != ptr_format_temp)
		    		{
		        		ptr_format_temp->next = ptr_format; 
		        		ptr_format_temp = ptr_format;
		    		}
                    else
		    			ptr_format_temp = ptr_format;

				}
				if(temp_char == '\\')
		    		break;
	    	} /* end of while */
	    
            ptr->m_portcount = ONE;
	    	ptr->m_next = NULL;
	    	attr_count = ZERO;
	    
	    	while(TRUE == TRUE)
	    	{
	        	for(temp_char = (char)fgetc(fp); 
		    		temp_char != '\n' && temp_char != (char) -1; 
		    		temp_char = (char)fgetc(fp));
		    
                if(temp_char != (char) EOF)
		    		temp_char = (char)fgetc(fp);

	        /***assigning information field to media structure***/	    
	        
				if(temp_char == 'i')
				{ 
		    		(void)fgetc(fp);
                    for(index = 0, temp_char = (char)fgetc(fp); 
		        		temp_char != '\\'; 
						temp_char = (char)fgetc(fp), index++)
		    		{	
	                	media_field[index] = temp_char;
		    		} /* end of for */
		    		media_field[index] = '\0';
		    		ptr->m_info = (char *) calloc(strlen(media_field) + ONE, 
		    					  sizeof(char));
                    /*error handling*/ 
                    if(NULL == ptr->m_info)
                    {
		        		error_handling(ERR_MAJOR, 
				       				SDP_MEDIA_MALLOC_ERROR,
				       				"sdp_add_media: Malloc error ");
						if(EOF == fclose(fp))
						{
							error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
						    		   	"sdp_add_media: fclose error ");
						}
		        		TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	    	        return FAILURE;
                    }
					/*copying media field*/
		    		if(NULL == (strncpy(ptr->m_info , media_field, 
								strlen(media_field) + 1)))
		    		{
		    			error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			            		   "sdp_add_media: strncpy error ");
						if(EOF == fclose(fp))
						{
							error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
						    		   	"sdp_add_media: fclose error ");
						}
						TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
                		return FAILURE;
		    		}
		    
				}

	        /***assigning fields of connection attribute to media 
		structure***/	
				if(temp_char == 'c')
				{   
                 	ptr->m_conn = (sdp_conn_t *) calloc(ONE, 
		    					sizeof(sdp_conn_t));
                    
		    		if(NULL == ptr->m_conn)
                	{
		        		error_handling(ERR_MAJOR, 
				    		   SDP_MEDIA_MALLOC_ERROR,
				       			"sdp_add_media: Malloc error ");
		        		TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
						if(EOF == fclose(fp))
						{
							error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
						    		   	"sdp_add_media: fclose error ");
						}
       	    	   		return FAILURE;
                	}
                    
		    		var_count = ONE;
                	(void)fgetc(fp);
                    
		    		while(TRUE == TRUE)
                	{
                    	for(index = 0, temp_char = (char)fgetc(fp); 
			    			temp_char != ' ' && 
			    			temp_char != '/' && 
			    			temp_char != '\\'; 
			    			temp_char = (char)fgetc(fp), index++)
                    	{
                        	media_field[index] = temp_char;
                    	}
			
                    	media_field[index] = '\0';
			
						if(temp_char == '\\')
                    	{
							break_while = ONE;
                    	}
                        
						switch(var_count)
                    	{
                    		case 1:
                            	ptr->m_conn->c_nettype = (char *) calloc(strlen(media_field) + ONE,	sizeof(char));
                            
                            	if(NULL == ptr->m_conn->c_nettype)
                            	{
		                			error_handling(ERR_MAJOR,
					       				SDP_MEDIA_MALLOC_ERROR,
					       				"sdp_add_media: Malloc error ");
									if(EOF == fclose(fp))
									{
										error_handling(ERR_MINOR,
														SDP_CLOSE_ERROR,
							 	   		   	"sdp_add_media: fclose error ");
									}
		                			TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        					return FAILURE;
                            	}
                            	if (NULL == (strncpy(ptr->m_conn->c_nettype, 
			    						media_field, strlen(media_field) + 1)))
								{
									error_handling(ERR_MAJOR,
											SDP_STRNCPY_ERROR,
											"sdp_add_media: strncpy error ");
									if(EOF == fclose(fp))
									{
										error_handling(ERR_MINOR,
														SDP_CLOSE_ERROR,
							 	   		   	"sdp_add_media: fclose error ");
									}
		                			TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        					return FAILURE;
								}
                 	           	var_count++;
                    	        break;

                        	case 2:
                            	ptr->m_conn->c_addrtype = (char *) calloc(strlen(media_field) + ONE, sizeof(char));
                            
                            	if(NULL == ptr->m_conn->c_addrtype)
                           	 	{   
		                			error_handling(ERR_MAJOR,
				               				SDP_MEDIA_MALLOC_ERROR,
					       				"sdp_add_media: Malloc error ");
									if(EOF == fclose(fp))
									{
										error_handling(ERR_MINOR,
														SDP_CLOSE_ERROR,
							 	   		   	"sdp_add_media: fclose error ");
									}
		                			TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	    	    				return FAILURE;
                	            }
                    	        if (NULL == (strncpy(ptr->m_conn->c_addrtype, 
			    					media_field, strlen(media_field) + 1)))
								{
									error_handling(ERR_MAJOR,
											SDP_STRNCPY_ERROR,
											"sdp_add_media: strncpy error ");
									if(EOF == fclose(fp))
									{
										error_handling(ERR_MINOR,
														SDP_CLOSE_ERROR,
							 	   		   	"sdp_add_media: fclose error ");
									}
		                			TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        					return FAILURE;
								}
                            	var_count++;
			    				break;
			
			    			case 3:
                            	ptr->m_conn->c_address = (char *) calloc(strlen(media_field) + ONE, sizeof(char));
                            
                            	if(NULL == ptr->m_conn->c_address)
                            	{
		                			error_handling(ERR_MAJOR,
					       					SDP_MEDIA_MALLOC_ERROR,
					       				"sdp_add_media: Malloc error ");
									if(EOF == fclose(fp))
									{
										error_handling(ERR_MINOR,
														SDP_CLOSE_ERROR,
							 	   		   	"sdp_add_media: fclose error ");
									}
		                			TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        					return FAILURE;
                       		    }
                            	if (NULL == (strncpy(ptr->m_conn->c_address, 
			    				media_field, strlen(media_field) + 1)))
								{
									error_handling(ERR_MAJOR,
											SDP_STRNCPY_ERROR,
											"sdp_add_media: strncpy error ");
									if(EOF == fclose(fp))
									{
										error_handling(ERR_MINOR,
														SDP_CLOSE_ERROR,
							 	   		   	"sdp_add_media: fclose error ");
									}
		                			TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        					return FAILURE;
								}
                      	      	var_count++;
                            	break;

			    			case 4:
                    	    	ptr->m_conn->c_ttl = atoi(media_field);
			    				var_count++;
			    				break;
			    
			    			case 5:
                    		    ptr->m_conn->c_addrcount = atoi(media_field);
			    				break;
                        }/* end of switch */

					if(break_while == ONE)	
						break;
               	}   
              	ptr->m_conn->c_next = NULL;

			} 
		
       /***assigning fields of bandwidth attribute to media structure***/
			if(temp_char == 'b')
			{
                (void)fgetc(fp);
                 ptr->m_bw = (sdp_bandwidth_t *) calloc(ONE, 
		    				       sizeof(sdp_bandwidth_t));
                /*error handling*/
                if(NULL == ptr->m_bw)
                {
		    		error_handling(ERR_MAJOR, 
				    	   SDP_MEDIA_MALLOC_ERROR, 
				       		"sdp_add_media: Malloc error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
		        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	       		return FAILURE;
                }
		   
		   		/*going to next needed character*/ 
                for(index = 0, temp_char = (char)fgetc(fp); 
		    		temp_char != ':'; 
					temp_char = (char)fgetc(fp), index++)
		    	{
                   	media_field[index] = temp_char;
		    	}
                media_field[index] = '\0';
                ptr->m_bw->b_type = (char *) calloc(strlen(media_field) + ONE, sizeof(char));
                /*error handling*/   
                if(NULL == ptr->m_bw->b_type)
                {
		        	error_handling(ERR_MAJOR,
			        	       SDP_MEDIA_MALLOC_ERROR, 
				       		"sdp_add_media: Malloc error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
		        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        	return FAILURE;
                }
                if (NULL == (strncpy(ptr->m_bw->b_type, media_field, 
						strlen(media_field) + 1)))
		    	{
		    	
		    		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			        	       "sdp_add_media: Strncpy error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
					TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
                	return FAILURE;
		    	}
				/*traversing the whole line*/
                for(index = 0, temp_char = (char)fgetc(fp); 
		    		temp_char != '\\'; 
					temp_char = (char)fgetc(fp),index++)
		    	{
                  	media_field[index] = temp_char;
		    	} 
                media_field[index] = '\0';
				/*assigning values to corresponding fields*/
                ptr->m_bw->b_value = (unsigned int)atoi(media_field);
                ptr->m_bw->b_next = NULL;

			}

         /***assigning fields of key attribute to media structure***/                  
            if(temp_char == 'k')
			{
                (void)fgetc(fp);
                ptr->m_key = (sdp_key_t *) calloc(ONE, sizeof(sdp_key_t));
                    
                if(NULL == ptr->m_key)
                {
		        	error_handling(ERR_MAJOR, 
			               SDP_MEDIA_MALLOC_ERROR, 
				    	   "sdp_add_media: Malloc error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
		        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        	return FAILURE;
                }
				
                /*traversing the whole line*/
		    	for(index = 0, temp_char = (char)fgetc(fp); 
		        	temp_char != '\\'; 
					temp_char = (char)fgetc(fp), index++)
		    	{
                  	media_field[index] = temp_char;
		    	} /* end of for */
                media_field[index] = '\0';
                
				ptr->m_key->k_method = (char *) calloc(strlen(media_field) + 
										ONE, sizeof(char));
                /*error handling*/
                if(NULL == ptr->m_key->k_method)
                {
		        	error_handling(ERR_MAJOR,
			        	       SDP_MEDIA_MALLOC_ERROR, 
				       			"sdp_add_media: Malloc error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
		        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        	return FAILURE;
                }
		    
                if(NULL == (strncpy(ptr->m_key->k_method, media_field,
							strlen(media_field) + 1)))
		    	{
		    		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			               "sdp_add_media: Strncpy error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
					TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
                	return FAILURE;
		    
		    	}
                ptr->m_key->k_enckey = NULL;

			}
		
	/***assigning attribute	fields to media structure***/ 
			if(temp_char == 'a')
			{

                (char)fgetc(fp);
	     
            	ptr_attr = (sdp_attr_t *) calloc(ONE, sizeof(sdp_attr_t));
                      
                if(NULL == ptr_attr)
                {
		        	error_handling(ERR_MAJOR,
			               SDP_MEDIA_MALLOC_ERROR, 
				    	   "sdp_add_media: Malloc error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
		        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        	return FAILURE;
                }
				/*traversing attr list*/
		    	if(attr_count == ZERO)
		    	{
		        	ptr->m_attr = ptr_attr; 
					ptr_attr_temp = ptr_attr; 
					attr_count++;
		    	}
		    	else
		    	{  
		        	if(NULL != ptr_attr_temp)
					{
		            	ptr_attr_temp->a_next = ptr_attr;
			    		ptr_attr_temp = ptr_attr;
					}
					else
			    		ptr_attr_temp = ptr_attr;
		    	}
	     		/*getting next word*/
                for(index = 0, temp_char = (char)fgetc(fp); 
		        	temp_char != ' '; 
					temp_char = (char)fgetc(fp), index++)
		    	{
                  	media_field[index] = temp_char;
		    	}/* end of for */
		    
                media_field[index] = '\0';
                ptr_attr-> a_name = (char *) calloc(strlen(media_field) + 
									ONE, sizeof(char));
                    
                if(NULL == ptr_attr-> a_name)
                {
		        	error_handling(ERR_MAJOR,
			        	       SDP_MEDIA_MALLOC_ERROR,
				       			"sdp_add_media: Malloc error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
		        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        	return FAILURE;
                }
		    
                if (NULL == (strncpy(ptr_attr->a_name, media_field,
						strlen(media_field) + 1)))
		    	{
		    
		    		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			        		       "sdp_add_media: Strncpy error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
					TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
                	return FAILURE;
		    	}
				/*traversing the whole line*/
                for(index = 0, temp_char = (char)fgetc(fp); 
		        	temp_char != '\\'; 
					temp_char = (char)fgetc(fp), index++)
		    	{
                    media_field[index] = temp_char;
		    	} /* end of for */
                media_field[index] = '\0';
                ptr_attr->a_value = (char *) calloc(strlen(media_field) + ONE,
									sizeof(char));
                    
                if(NULL == ptr_attr->a_value)
                {
		        	error_handling(ERR_MAJOR, 
			               SDP_MEDIA_MALLOC_ERROR, 
				       "sdp_add_media: Malloc error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
		        	TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        	return FAILURE;
                }
                    
		    	if (NULL == (strncpy(ptr_attr->a_value, media_field, 
					strlen(media_field) + 1)))
		    	{
		    
		    		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			        	       "sdp_add_media: Strncpy error ");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
							 	      	"sdp_add_media: fclose error ");
					}
					TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
                	return FAILURE;
		    	}
                ptr_attr->a_next = NULL;
			}   
	    
	        if(temp_char == 'm')
	        {   
	            (void)fseek(fp, -1, ONE);
	            break;
	        }
		
	        if(temp_char == (char) -1)
	        {
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
						 	      	"sdp_add_media: fclose error ");
					TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	        	return FAILURE;
				}
		    	TRACE("\nTRACE: Exiting function sdp_add_media with success\n");
		    	return SUCCESS;
	        }
		
	    }   
	}   
	
	/***skip all lines other than media attribute***/
	if(temp_char != 'm')
   	    for(temp_char = (char)fgetc(fp); 
	        temp_char != '\n'; 
			temp_char =(char)fgetc(fp));  
		
    }/* end of while */
    
	if(EOF == fclose(fp))
	{
		error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
			 	      	"sdp_add_media: fclose error ");
		TRACE("\nTRACE: Exiting function sdp_add_media with failure\n");
       	return FAILURE;
	}
    TRACE("\nTRACE: Exiting function sdp_add_media with Success\n");
    return SUCCESS;
}   
