/* test.c --- 
 * 
 * 
 * Author: Yenkai Huang
 * Created: Mon Jan 16 00:11:11 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    char str[] = "4294967295";
    char *endptr;
    unsigned long int value = strtoul(str, &endptr, 10);
    if (value <= UINT32_MAX && *endptr == '\0') {
        uint32_t result = (uint32_t)value;
        printf("The string '%s' converted to uint32_t is: %u\n", str, result);
    } else {
        printf("The string '%s' is out of range for a uint32_t value or contains non-numeric characters\n", str);
    }
    return 0;
}
