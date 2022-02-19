
#include <stdio.h>	// IO functions
#include <stdlib.h> 	// std lib function
#include <unistd.h>	// close function
#include <pthread.h>	// creating, running, joining threads
#include <string.h>	// string processing 
#include "web.h"

#define DEBUG 1 


// Global variable shared by all threads
// This is not thread safe.
web_t* web_array;


// Function prototypes to read and write url files
int read_url_file( char* file_path, web_t* web_array );
int write_url_file( char* file_path, web_t* web_array, int num_urls );


// Function prototypes that is ran by a thread
void th_run( int* i );

int main( int argc, char *argv[] ) {

	int i = 0;
	int write_error = 0;

	// malloc web_t array with 100 elements
	web_array = malloc( sizeof( web_t )*100 );

	// Verify the correct number of arguments are provided
	// when the application is executed.

	if ( argc == 2 ) { 

		int num_urls = read_url_file( argv[1], web_array );

		if ( DEBUG ) printf("Number of urls in text file = %d\n", num_urls);

		

		pthread_t thread_array[num_urls];
		int index[num_urls];
		int rc;
		for ( i=0; i<num_urls; i++ ) {	
			index[i] = i;
			printf("main: creating thread:\n");
			//rc = pthread_create(&thread_array[i], NULL, (void*)th_run, &index[i]);
			rc = pthread_create(&thread_array[i], NULL, (void*)&th_run, (void *)&index[i]);
			printf("rc = %d\n", rc);
			printf("In main \n thread id = %ld\n", thread_array[i]);
			if ( DEBUG ) {
				printf("URL=%s\n", web_array[i].url ); 
				printf("CNT=%d\n", web_array[i].link_cnt );
				//printf("WEBPAGE=%s\n", web_array[i].webpage, "END\n" );
			}
			if (rc){
				printf("ERROR: CAN'T CREATE THREAD, %d\n", rc);
				exit(-1);
			}
			printf("INDEX: %d\n", index[i]);
			
			
		}
		
	
		// Join threads
		
		sleep(10);
		printf("Joining\n");
		for (i =0; i<num_urls; i++){
			pthread_join(thread_array[i], NULL);
		}
		

		// Write results to output file 
		
		
		write_url_file("output.txt", web_array, num_urls);

		//for (i = 0; i<num_urls; i++){
			//printf("\n url IN [i]= %s\n", web_array[i].url);

			//printf("\nx= links in [i]= %d\n", web_array[i].link_cnt);
			//printf("all the links for [i]\n");
			//for(int x =0; x<web_array[i].link_cnt;x++){
			//	printf("%s\n", web_array[i].links[x]);
			//}
				//if(web_array[i].link_cnt>0){
					//printf("\n END PRINT %s ", web_array[i].url);
					//for(int x=0; x<web_array[i].link_cnt; x++){
					//	printf(" %s \n", web_array[i].links[x]);
					//}
				//}

		

		

	}
	else {

		printf( "URL file is not specified!\n" );
		printf( "Example usage: %s %s\n", argv[0], "url.txt" );

	}

	return OK;
     
} // end main function

// ------------------------------------
// Function ran by an individual thread
// 
//
// Arguments:	i = an index value (to access individual web_t structs in the web_array)
//
// Return:     	nothing (void)
//
void th_run( int* i ) {

	// -------------------------------------
	// Please do not modify this code
	// -------------------------------------
	printf("INSIDE TH_RUN\n");
	if ( web_array[ (*i) ].link_cnt == OK ) {

		parse( &web_array[ (*i) ] );

	} else printf( "[%s] failed to retrieve webpage\n", web_array[ (*i) ].url );

	pthread_exit( NULL );

} // end th_run function

// ------------------------------------
// Function that writes the identified links 
// stored in the web_t links field to formated
// text file.
// 
// 
//
// Arguments:	file_path = fully qualified path to output text file
//		web_array = pointer to array of web_t structs
//		num_urls = the total number of websites in the url.txt file
//
// Return:     	OK on success, FAIL on error
//
int write_url_file( char* file_path, web_t* web_array, int num_urls ) {
	
	int z;
	FILE *fp;
	fp = fopen(file_path, "w");
	for (int i = 0; i<num_urls; i++){
			//fprintf(fp, "\n %s\n", web_array[i].url);

			//fprintf(fp, "\nx= links in [i]= %d\n", web_array[i].link_cnt);
			//fprintf(fp, "all the links for [i]\n");
			for(int x =0; x<web_array[i].link_cnt;x++){
				fprintf(fp, "\n%s", web_array[i].url);
				fprintf(fp, ", ");
				fprintf(fp, "%s\n", web_array[i].links[x]);
				
			}
	}
	fclose(fp);
	// -----------------------------------------
	// TODO:
	// -----------------------------------------
	// The format of this file is provided below. 
	// 
	// <url>, <link>\n
	//
	//
	// For example,
	//
	//	usatoday.com, /story/news/politics/2018/10/23/donald-trump-looks-ahead-life-after-midterms/1732675002/
	//	wsj.com, /articles/trump-not-satisfied-with-saudi-arabias-explanation-for-khashoggi-death-1540237930?mod=hp_major_pos8
	//      .
	//	.
	//      .
	//

	

	return OK;

} // end write_url_file function



// ------------------------------------
// Function that reads the url in the input
// url text file.
// 
// 
// Arguments:	file_path = fully qualified path to output text file
//		web_array = pointer to array of web_t structs
//
// Return:     	Total number of urls defined in the url text file
//
int read_url_file( char* file_path, web_t* web_array ) {

	// -------------------------------------
	// Please do not modify this code
	// -------------------------------------

	int url_cnt = 0;

	FILE* fhnd;

    	fhnd = fopen( file_path, "r" );

    	char line[50];

    	if ( fhnd != NULL ) {

        	while ( fgets( line, sizeof( line ), fhnd ) != NULL ) {

			line[strlen(line)-1] = '\0';
			web_array[url_cnt].url = malloc(sizeof(char)*100);
			sprintf( web_array[url_cnt].url, "www.%s", line );

			web_array[url_cnt].link_cnt = OK;

			if ( FAIL == get_webpage( &web_array[url_cnt++] ) ) {

				printf("Failed to load webpage for URL( %s )\n", web_array[url_cnt--].url );
				web_array[url_cnt].link_cnt = FAIL;				

			}

        	}

    	} else url_cnt = FAIL;

    	fclose( fhnd );

	return url_cnt;

} // end read_url_file function

