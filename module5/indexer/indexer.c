/* indexer.c --- 
 * 
 * 
 * Author: Yenkai Huang, Luis, Erin
 * Created: Sun Feb 12 15:55:17 2023 (-0500)
 * Version: 1.0
 * 
 * Description: indexer for tse 
 * 
 */

# include <stdio.h>
# include <stdlib.h>         
# include <string.h>
# include <stdbool.h>
# include <webpage.h>
# include <queue.h>
# include <hash.h>
# include <pageio.h>

int main(void){
	
	printf("hello!\n");
	webpage_t *page = pageload(1, "../pages/");
	
	int c = webpage_getNextWord(page, int pos, char **word);
}
