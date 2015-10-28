/**********************************************************************
*
* FILE NAME     : sdp_media_comp.c
*
* DESCRIPTION   : To compare client and server media.
*
* Date        Name               Reference        Purpose
* 14 jan     sharad kumar yadav   LLD             Initial creation 
*            Himanshu saini
* 18 jan     sharad kumar yadav   LLD             Error handling
             Himanshu saini
Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
**********************************************************************/

/************HEADER FILES INCLUSION************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_media_comp
*
* DESCRIPTION   : To compare client and server media.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer
*
***********************************************************************/


/************FUNCTION DEFINITION***************/
return_type sdp_media_comp(char *file_name_server,  /*client file*/
                           char *file_name_client   /*server file*/
						  )
			   
{
	FILE 	*fp_server = NULL; /*File pointers for server*/ 
    	FILE    *fp_client = NULL; /*File pointers for client*/
	int 	flag = ZERO;       /* Flag is set when any of the codec matches*/
	int 	codec_position = ZERO; /* Track the codec position*/
	int		lcounter = ONE;    /* Increments untill codec position matches */  
	int 	index = ONE;        /*buffer index*/
	char 	temp_char_2 = INIT;  /*for storing temp_char*/
	char 	temp_char = INIT;    /*for fgetc*/
	char 	temp_char_c = INIT;  /*for fgetc*/
  	char 	client_media_type[SIZE];    /*Client buffer for media type */
  	char 	client_media_port[SIZE];    /*Client buffer for media type */
  	char 	client_media_proto[SIZE];   /*Client buffer for prototype*/
  	char 	client_media_codec[SIZE];   /*Client buffer for codec*/
  	char 	server_media_buffer[SIZE];  /*Server buffer for temporary storage*/
  	char 	server_media_type[SIZE];    /*Server buffer for media type*/
  	char 	client_media_attr[ATTR_SIZE];/*Client buffer for media attributes*/
  	char 	server_media_attr[ATTR_SIZE];/*Server buffer for media attributes*/
        
        TRACE("\nTRACE: Entering function sdp_media_comp\n");

	/*initialising buffer*/
    memset(client_media_type, 0, SIZE);
    memset(client_media_port, 0, SIZE);
    memset(client_media_proto, 0, SIZE);
    memset(client_media_codec, 0, SIZE);
    memset(server_media_buffer, 0, SIZE);
    memset(server_media_type, 0, SIZE);
    memset(client_media_attr, 0, SIZE);
    memset(server_media_attr, 0, SIZE);
	
	/*opening client file*/
    fp_client = fopen (file_name_client, "r");
  
  	if ( NULL == fp_client )
        { 
		error_handling(ERR_MAJOR, SDP_MEDIA_FILE_ERROR,
		               "sdp_media_comp: File openning error ");
		TRACE("\nTRACE: Exiting function sdp_media_comp with unclosed client file\n");
        return FAILURE;
        }
        
        /*opening server file*/
        fp_server = fopen ( file_name_server, "r");
        if ( NULL ==  fp_server )
         {
		error_handling(ERR_MAJOR, SDP_MEDIA_FILE_ERROR,
		               "sdp_media_comp: File openning error ");
		TRACE("\nTRACE: Exiting function sdp_media_comp with unclosed server file\n");
           return FAILURE;
        }
  	
        while (feof (fp_client) == ZERO ) /*Beginning of while for client file */
        {

      		temp_char = (char) fgetc (fp_client); /*Read a character*/
      		temp_char_c = temp_char;
      		
                /*Searching for m-field*/
                if (temp_char == 'm' || temp_char_c == 'm') 
		{
	  		rewind (fp_server);/*Setting the file pointer to begining*/
	  		(char) fgetc (fp_client);
	                
                        /*Storing media type of the client */
	  		index = 0;
	  		memset (client_media_type,0, SIZE);
	  		
                        for (temp_char = (char) fgetc (fp_client);
	       			temp_char != ' '; 
				temp_char = (char) fgetc (fp_client))
	    		{
	      			client_media_type[index] = temp_char;
	      			index++;
	    		}
	  		client_media_type[index] = '\0';/*Null termination*/

	                /*Storing media port of the client in buffer */
	  		index = 0;
	  		memset (client_media_port, 0, SIZE);
	  		
                        for (temp_char = (char) fgetc (fp_client);
	    	   		temp_char != ' '; 
			   	temp_char = (char) fgetc (fp_client))
	    		{
	      			client_media_port[index] = temp_char;
	      			index++;
	    		}
	  		client_media_port[index] = '\0';/*Null termination*/

	                /*Storing media protocol of the client in buffer*/
	  		index = 0;
	  		memset (client_media_proto, 0, SIZE);
	  		
                        for (temp_char = (char) fgetc (fp_client);
	       		temp_char != ' '; 
				temp_char = (char) fgetc (fp_client))
	    	        {
	      			client_media_proto[index] = temp_char;
	      			index++;
	    		}
	  		client_media_proto[index] = '\0';/*Null termination */

	               /*Storing codec field of the client */
	  		index = 0;
	  		memset (client_media_codec, 0, SIZE);
                         	  		
			for (temp_char = (char) fgetc (fp_client);
	       			temp_char != '\n'; 
				temp_char = (char) fgetc (fp_client))
	    		{
	      			client_media_codec[index] = temp_char;
	      			index++;
	    		}
	  		client_media_codec[index] = '\0';/*Null termination*/

	               /*Storing attrributes of media of client */
	  		index = 0;
	  		memset (client_media_attr, 0, ATTR_SIZE);
	  		
			for (temp_char = (char) fgetc (fp_client);;
	       		temp_char = (char) fgetc (fp_client))
	    		{       /*reading character untill \n followed by m or EOF*/
	      			if (((temp_char_2 == '\n') && (temp_char == 'm')) || (temp_char == (char)EOF))
					break;
	      
		  		client_media_attr[index] = temp_char;
	      			index++;
	      			temp_char_2 = temp_char;
	    		}
	  		temp_char_c = temp_char;
	  		client_media_attr[index] = '\0';/*Null termination*/
	  		if(fseek (fp_client, -1, SEEK_CUR) != SUCCESS )
                        { 
                         	error_handling(ERR_MAJOR, SDP_READ_ERROR,
                				"sdp_media_comp: fseek error ");
                                
                        }
	  		
			while (feof (fp_server) == ZERO)/*while loop for server file*/
	    		{
	      			temp_char = (char) fgetc (fp_server);
	      			if (temp_char == 'm')
				{
		  			while (TRUE == TRUE)
		    			{
		      				/*takimg media type of server file */
		      				index = 0;
		      				memset (server_media_type, 0, SIZE);
			      			(char) fgetc (fp_server);
	
			      			for (temp_char = (char) fgetc (fp_server);
							temp_char != ' ';
				   			temp_char = (char) fgetc (fp_server))
						{
				  			server_media_type[index] = temp_char;
				  			index++;
						}
	
			      			server_media_type[index] = '\0';/*Null termination */
			      			/*comparing the media type of server and client */
		    	  			if (strcmp (server_media_type, client_media_type) != ZERO )
							break;
	
			      			/*taking media port of server file */
			      			index = 0;
		    	  			memset (server_media_buffer, 0, SIZE);
	
			     	 		for (temp_char = (char) fgetc (fp_server);
                                                        temp_char != ' ';
				   			temp_char = (char) fgetc (fp_server))
						{
			  				server_media_buffer[index] = temp_char;
			  				index++;
						}
		      					server_media_buffer[index] = '\0';/*Null termination */
					
		      				/*comparing the media port of server and client */
			      			if (strcmp (server_media_buffer, client_media_port) != ZERO )
							break;
	
			      			/*taking media proto of server */
			      			index = 0;
		    	  			memset (server_media_buffer, 0, SIZE);
	
			      			for (temp_char = (char) fgetc (fp_server);
				   			temp_char != ' ';
				   			temp_char = (char) fgetc (fp_server))
						{
				  			server_media_buffer[index] = temp_char;
				  			index++;
						}
			      			server_media_buffer[index] = '\0';/*Null termination*/
					
			      			/*comparing the media  protocol of server and client */
			      			if (strcmp (server_media_buffer, client_media_proto) != ZERO)
							break;

						/*for tracking the position of the codecno which 
		 				*will be matched with the client codecno */
			      			while (temp_char != '\n')/*codec comparison of servers to client codec untill newline*/
						{
				  			index = 0;
			  				memset (server_media_buffer, 0, SIZE);
	
				  			for (temp_char = (char) fgetc (fp_server);
				       			(temp_char != ' ') && (temp_char != '\n');
				       			temp_char = (char) fgetc (fp_server))
			    				{
			      					server_media_buffer[index] = temp_char;
			      					index++;
			    				}
			 	 			index++;
			  				server_media_buffer[index] = '\0';/*Null termination*/
			  				codec_position++;
				  			if ( strncmp(server_media_buffer,client_media_codec, 2) == ZERO)
			    				{
				      				flag = 1;
				      				while (temp_char != '\n')
								{
					  				temp_char = (char) fgetc (fp_server);
								}
			      					break;
			    				}

						}
		      				if (flag == 0)
							break;

		      				/*attribute comparision of server and client */
		      				index = 0;
			      			memset (server_media_attr, 0, ATTR_SIZE);
                                                /* If media type is audio */
			      			if ( strcmp(server_media_type, "audio") == ZERO )
						{
				  			for (temp_char = (char)fgetc (fp_server);;
			    	   			temp_char = (char)fgetc (fp_server))
			    				{
			      					if ((temp_char_2 == '\n') && (temp_char == 'm'))
									break;

				      				server_media_attr[index] = temp_char;
				      				temp_char_2 = temp_char;
				      				index++;
				    			}

						}
		      				else /*For media-type is video */
						{
			  				for (temp_char = (char) fgetc (fp_server);;
			       				temp_char = (char) fgetc (fp_server))
			    				{
			      					if ((temp_char_2 == '\n') && (temp_char == 'a'))
									break;

			      					server_media_attr[index] = temp_char;

			      					temp_char_2 = temp_char;
			      					index++;
			    				}
                                                        
                                                       /*Finding the matched codec*/
			  				while (lcounter < codec_position)
			    				{       /*skip lines*/
			      					for (; (char) fgetc (fp_server) != '\n';);
			      						lcounter++;/*increment lcounter*/
			    				}

			  				if (codec_position == ONE)
			    				{       /*store newline character*/
			      					server_media_attr[index] = temp_char;
			      					index++;
			    				}
			  				else
			    				{      /*read next character*/
			      					server_media_attr[index] = (char) fgetc(fp_server);
			      					index++;
			    				}
			  				for (temp_char = (char) fgetc (fp_server);
			       				temp_char != '\n';
			       				temp_char = (char) fgetc (fp_server))
			    				{
			      					server_media_attr[index] = temp_char;
			      					index++;
			    				}
			  				server_media_attr[index] = '\0';/*Null termination*/
			  				server_media_attr[index] = '\n';
						}

		      				if (strcmp (server_media_attr, client_media_attr) != ZERO)
							break;
		      				else
						{
							TRACE("\nTRACE: Exiting function sdp_media_comp with Success\n");
       							if(EOF == fclose(fp_client) )
       							{
        							error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
    "sdp_media_comp: File closing error ");
		                                                TRACE("\nTRACE: Exiting function sdp_media_comp with unclosed client file\n");

       							}
       
       							if(EOF == fclose(fp_server))
       							{
        							error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
    "sdp_media_comp: File closing error ");
		                                                TRACE("\nTRACE: Exiting function sdp_media_comp with unclosed server file\n");
       							}
			  				return SUCCESS;
						}
		    			}/*End of while loop*/
				}
	                	
				/*If not m character move to end of line */
		    		if (temp_char != 'm')
				{
			 		 for (temp_char = (char)fgetc (fp_server);
		    			  temp_char != '\n';
		       				temp_char = (char) fgetc (fp_server))
		    			{
		      				if (temp_char == (char)EOF)
							break;
		    			}
				}
			}/*End of while loop of the server file*/
		}
      		
                else 
		{       /*Read until newline or EOF encounters*/
	  		for (temp_char_c = (char)fgetc (fp_client); temp_char_c != '\n';
	       		temp_char_c = (char) fgetc (fp_client))
	    		{
	      			if (temp_char_c == (char)EOF)
					break;
	    		}
		}
        }/*End of while for client file*/
	
        TRACE("\nTRACE: Exiting function sdp_media_comp with Failure\n");
     
       if(EOF == fclose(fp_client) )
       {
        	error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
                               "sdp_media_comp: File closing error ");
	        TRACE("\nTRACE: Exiting function sdp_media_comp with unclosed client file\n");

       }
       
       if(EOF == fclose(fp_server))
       {
        	error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
                               "sdp_media_comp: File closing error ");
		TRACE("\nTRACE: Exiting function sdp_media_comp with unclosed server file\n");
       }

	return FAILURE;
}/*End of sdp_media_comp*/
