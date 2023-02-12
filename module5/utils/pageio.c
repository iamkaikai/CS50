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
/*
 * pagesave -- save the page in filename id in directory dirnm
 * returns: 0 for success; nonzero otherwise
 * The suggested format for the file is:
 *   <url>
 *   <depth>
 *   <html-length>
 *   <html>
 */
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
 * returns: non-NULL for success; NULL otherwise
 */

webpage_t *pageload(int id, char *dirnm){
    char file_location[64];
    sprintf(file_location, "%s%d", dirnm, id);
    FILE *fp1 = fopen(file_location, "r");
    char *url = malloc(sizeof(char)*256);
    char *depth = malloc(sizeof(char)*4);
    char *html_len = malloc(sizeof(char)*8);
    char *html;
    int depth_int;
    int html_len_int;
    int pos = 0;
    
    if (fp1 == NULL) {
        printf("Wrong directory or cannot find the file!!");
        return NULL;
    }else{
        
        //intercept the first three lines for url, depth and html_len
        fscanf(fp1, "%s\n%s\n%s\n", url, depth, html_len);
        depth_int = atoi(depth);
        html_len_int = atoi(html_len);
        html = (char*)malloc(sizeof(char) * (html_len_int + 10));

        //extract the content from the file
        int c;
        while((c = fgetc(fp1)) != EOF) {
            html[pos] = c;
            pos += 1;
        }
        html[pos] = '\0';
    }
    fclose(fp1);

    webpage_t *new_page = webpage_new(url, depth_int, html);
    free(url);
    free(depth);
    free(html_len);
    return new_page;
}