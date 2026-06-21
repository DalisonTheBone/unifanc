#include <stdio.h>
#include "modules/config.h"

int main(void) {
    config_file test = get_config("unifand.conf");
    

    printf("done!");
    
}
