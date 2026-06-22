#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#include "modules/modules.h"
#include "modules/config.h"
//#include "modules/file.h"

// Constants
char module_path[] = "/etc/unifand/module";
char config_path[] = "./unifand.conf";

// Functions
int get_fan_speed(config_file config, int tempature, bool first_run) {

    config_section rules = get_section(config, "unifand/rules");
    int speed = -1;

    for (int i=0;i<rules.argument_cnt;i++) {

        config_argument rule = rules.arguments[i];
        int rule_speed = atoi(rule.value);
        range temp_range = parse_range(rule.key, rule.key_size);
        
        if (temp_range.min <= tempature && tempature <= temp_range.max) {
            speed = rule_speed;
            break;
        }

        if (temp_range.max < tempature && first_run) {speed = rule_speed;}
        if (temp_range.min > tempature) {break;}

    }

    return speed;

}


int main(void) {
    config_file const config = get_config(config_path);

    int temps = get_temps(module_path);
    int fan_speed = get_fan_speed(config, temps, true);
    
    while (true) {

        temps = get_temps(module_path);
        int temp_fan_speed = get_fan_speed(config, temps, false);

        if (temp_fan_speed != -1 && temp_fan_speed != fan_speed) {
            fan_speed = temp_fan_speed;
            set_fan_speed(module_path, fan_speed);
        }

        printf("fan speed: %d\ntemp: %d\n", fan_speed, temps);
        sleep(3);
       // break;
    }

    return 0;
}
