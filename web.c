

#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "web.h"
#include <stdbool.h>
#include <regex.h>
// global variable used by the write_callback function
size_t length = 0;


// ------------------------------------
// Function prototype that is used by the curl library to
// retrieve the webpage (in the response message) and 
// store in the web_t webpage field.
// 
//
// Arguments:	web_t pointer
//
// Return:     	OK on success, FAIL on error
//

size_t write_callback(void *ptr, size_t size, size_t nmemb, char** buffer ) {


	*buffer = (char*)realloc( *buffer, ( length + size*nmemb )*sizeof( char* ) ); 

	strcat( *buffer, (char*) ptr );

	length += (size*nmemb);

	return size * nmemb;

} // end write_callback function

// ------------------------------------
// Function prototype that connects to the web address 
// specified in the web_t url field and retrieves
// the html webpage (using the curl c-library)
// 
//
// Arguments:	web_t pointer
//
// Return:     	OK on success, FAIL on error
//

int get_webpage( web_t* web_struct ) {



	CURL *curl;
  	CURLcode error_code = FAIL;

	length = 0;

  	curl = curl_easy_init();

  	if ( curl ) {

	    	web_struct->webpage = malloc( sizeof( char ) * length );

	    	curl_easy_setopt(curl, CURLOPT_URL, web_struct->url );
	    	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &web_struct->webpage );
	 
	    	error_code = curl_easy_perform( curl );

		curl_easy_cleanup( curl );

	}
    
	return ( error_code == OK ) ? OK : FAIL;

} // end get_response function



// ------------------------------------
// Function prototype that parses webpage/URL 
// pointed at by client socket
// 
//
// Arguments:	web_t pointer
//
// Return:     	OK on success, FAIL on error
//

 
void parse( web_t* web_struct )
{
	//char** allFoundLinks;
           printf("PARSING\n");
           printf("BEFORE THREaD URL=%s\n", web_struct->url );
           printf("BEFORE THREAD CNT=%d\n", web_struct->link_cnt );
           web_struct->links =(char**) malloc(sizeof (char*));
		   char **allFoundLinks = malloc(sizeof (char*));
           int counter=0;
           int run=0;
           do
           {
                      int result = false;
                      char *ref0 = "<a href=";  // first key 
                      int reflen = strlen(ref0);//length of key
                      char *ptr = strstr(web_struct->webpage, ref0);  //find first key
                      char *ptr_end=ptr; //init
                      int numpos =0;     //init
                      if (NULL != ptr)
                      {
                                 web_struct->webpage = ptr + reflen+ 1;  //copy the new ptr loc past quotes
                                 char *ptr_start = ptr + reflen+ 1;                   //mark the position of the pointer
                                
                                 int end_link=0;                                                                        //precharge the do while end case
                                 do
                                 {
                                                                  
                                            ptr = strchr(web_struct->webpage, '"');  //find next quotes
                                            ptr++;
                                            if(strncmp (ptr, ">", 1)==0 ||  strncmp (ptr, " >", 1)==0)                     //see if the > is directly after the " or with a space after.
                                            {
                                                       end_link=1;                                                                  //found ending
                                                       ptr_end= ptr-1;
                                            }
                                            web_struct->webpage = ptr;                                     //copy position to webpage variable 

                                 }
                                 while(end_link==0);                                                                         ////found ending to link
                                 numpos =ptr_end - ptr_start;                                   //allocate and copy the link to the struct links varaible
                                 counter++;
                                 
								 allFoundLinks = (char**)realloc(allFoundLinks,sizeof(char*)*counter);
                                 //web_struct->links[counter-1] = (char*)malloc(sizeof(char)*numpos);
								 allFoundLinks[counter-1] = (char*)malloc(sizeof(char)*numpos);
                                 //strncpy(web_struct->links[counter-1],ptr_start,numpos/sizeof(char));
								 strncpy(allFoundLinks[counter-1],ptr_start,numpos/sizeof(char));

                      }
                      if(NULL==ptr)                                                                                                //end of page
                      {
                                 run=1;
                      }
           }
           while(!run);
 for (int i = 0; i<counter; i++){
		printf("RESULTS:%s\n", allFoundLinks[i]);

	
 }    
 int x = 0;
 for (int i = 0; i<counter; i++){
	 //int x = 0;
	 char* ptr = strstr(allFoundLinks[i], "russia");  // Keyword
	 if(ptr != NULL){
		 int stringLength = strlen(allFoundLinks[i]);
		 web_struct->links =(char**) realloc(web_struct->links,sizeof(char*)*(x+1));
		 web_struct->links[x]=(char*) malloc(sizeof(char)*(stringLength+2));
	 	 strcpy(web_struct->links[x], allFoundLinks[i]);
		x++;
	 }
 }          
 for (int i = 0; i<x; i++){
		printf("RESULTS struct%s\n", web_struct->links[i]);
 }
 printf("The keyword has appeared: %d", x); 
 printf(" times.\n");
 web_struct->link_cnt= x;              
                                                           //end of function
}
