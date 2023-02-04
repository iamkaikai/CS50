/* crawler.c --- 
1;95;0c * 
 * 
 * Author: YENKAI HUANG, HONGKE (LUIS) LU, ERIN
 * Created: Fri Feb  3 22:15:27 2023 (-0500)
 * Version: 1.0
 * 
 * Description: tiny search engine in module 4, grounp 2 in W23 CS50 
 * 
 */

# include <stdio.h>
# include <stdlib.h>   
# include <webpage.h>

int main(void){

	printf("Hello\n");
	char seed[] = "https://thayer.github.io/engs50/";
	int depth = 0;
	webpage_t *page;
				
	page = webpage_new(seed, depth, NULL);	

	if(page == NULL){
		printf("webpage_new error!\n");
	}else{
		printf("webpage_new succeed!\n");

		if(webpage_fetch(page)){
			printf("webpage_fetch succeed!\n");

			int pos = 0;
			char *result;
			while((pos = webpage_getNextURL(page, pos, &result)) > 0){
				if(IsInternalURL(result)){
					printf("Found URL (internal): %s\n",result);
				}else{
					printf("Found URL (external): %s\n",result);
				}
				free(result);
			}
			
		}else{
			exit(EXIT_FAILURE);
		}
	}


	webpage_delete(page);
	exit(EXIT_SUCCESS);
}
