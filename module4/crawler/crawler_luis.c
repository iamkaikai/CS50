/* crawler.c --- 
 * 
 * 
 * Author: Hongke Lu
 * Created: Tue Feb  7 21:33:30 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
# include <stdio.h>                                                     
# include <stdlib.h>                                                    
# include <string.h>                                                    
# include <stdbool.h>                                                   
# include <webpage.h>
# include <queue.h>
# include <hash.h>

void print_element(void *p){
	printf("URL = %s\n", (char*)p);
}

void BFS(char *seed, hashtable_t *url_hash,int depth){
	queue_t *url_queue = qopen();
	int pos = 0;
	char *result;
	qput(url_queue,seed);
	//hput(url_hash,seed,seed,strlen(seed));
	char *curr_url = (char*)qget(url_queue);
	hput(url_hash,curr_url,curr_url,strlen(curr_url));
	webpage_t *curr_page = webpage_new(curr_url, depth, NULL);  
  	if (!webpage_fetch(curr_page)){
		exit(EXIT_FAILURE);
	}
	while((pos = webpage_getNextURL(curr_page, pos, &result)) > 0){
		if(IsInternalURL(result)){                                            
          	printf("Found URL (internal): %s\n", result);

			qput(url_queue,result);
		}
		else{
			printf("Found URL (external): %s\n", result); 
		}
	}
	//free(curr_url);
	webpage_delete(curr_page);
	//qapply(url_queue,print_element);
	//qclose(url_queue);    
}


int main(void){                                                         
                                                                        
  	//setup up basic variables                                            
  	char seed[] = "https://thayer.github.io/engs50/";                     
  	int depth = 0;                                                        
	uint32_t hsize = 10;                                                 
	hashtable_t *url_hash  = hopen(hsize);                                
	//BFS(seed, url_hash,depth);                                                               
  	//initiate the seed page 
	//happly(url_hash,print_element); 
	char* a = "A";
	char* b = "B";
	hput(url_hash,a,b,1);                                                                           
  	hclose(url_hash);                                                   
  	exit(EXIT_SUCCESS);                                                   
}
