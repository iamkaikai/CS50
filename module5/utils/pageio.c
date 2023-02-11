/* pageio.c --- 
 * 
 * 
 * Author: Yenkai Huang Erin Luis
 * Created: Sat Feb 11 17:41:49 2023 (-0500)
 * Version: 1.0
 * 
 * Description: 
 * 
 */
# include <stdio.h>
# include <stdlib.h>     
# include <stdint.h> 
# include "webpage.h"  
# include <string.h>

int32_t pagesave(webpage_t *pagep, int id, char *dirname) {
    FILE *fp;
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

/* 
 * pageload -- loads the numbered filename <id> in direcory <dirnm>
 * into a new webpage
 *
 * returns: non-NULL for success; NULL otherwise
 */

webpage_t *pageload(int id, char *dirnm){
    char filename[64];
    sprintf(filename, "%s%d", dirnm, id);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Wrong directory or cannot find the file!!");
        return NULL;
    }else{
        
    }
};