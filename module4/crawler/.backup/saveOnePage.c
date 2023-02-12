/* saveOnePage.c --- 
 * 
 * 
 * Author: Hongke Lu
 * Created: Wed Feb  8 15:36:50 2023 (-0500)
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

int32_t pagesave(webpage_t *pagep, int id, char *dirname) {
	FILE *fp;
	if (!(fp = malloc(sizeof(FILE)))){
      printf("Error: malloc failed allocating hash table!!\n");
      return 1;
  }
	char path[64];
	char name[64];
	char depth[64];
	char html_len[64];
	char *url = webpage_getURL(pagep);
	char *html = webpage_getHTML(pagep);
	strcpy(path, dirname);
	sprintf(name, "%d", id);
	sprintf(depth, "%d", webpage_getDepth(pagep));
	sprintf(html_len, "%d", webpage_getHTMLlen(pagep));
	fp = fopen(strcat(path,name), "w+");
	fprintf(fp, "%s\n%s\n%s\n%s\n", url,depth,html_len, html);
	fclose(fp);
	return 0;
}

int main(void){
 	//setup up basic variables
	//printf("1\n");
 	char seed[] = "https://thayer.github.io/engs50/";
	webpage_t *curr_page = webpage_new(seed, 0, NULL);  
	printf("1\n");
	bool fetch = webpage_fetch(curr_page);
	if (fetch){
		pagesave(curr_page,1,"/thayerfs/home/f006h5j/engs50/tse/pages/");   
	}
}
