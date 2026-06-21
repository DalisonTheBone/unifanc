#include <stdio.h>
#include "modules/file.c"

int main(void) {
    file test = read_file("/etc/unifand/unifand.conf");

    
    printf("%s", test.bytes);
}
