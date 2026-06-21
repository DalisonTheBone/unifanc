#include <stdio.h>
#include "modules/modules.c"

int main(void) {
    //file test = read_file("/etc/unifand/unifand.conf");
    cmd_output responce = run_command("echo Hello World!");

    printf("%s", responce.bytes);
    
}
