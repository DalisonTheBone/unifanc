#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

// Constants
const char configPath[] = "/etc/unifand/unifand.conf";
const char modulePath[] = "/etc/unifand/module";

// Variables
int fan_speed;
int temp = 0;
bool change_fan_speed = true;
bool first_temp = true;

// Structs
struct temp_range {
    int8_t min;
    int8_t max;
    int8_t speed;
};

// Misc Functions
int main_paths_exist(void) {
    struct stat st;
    return (
        (stat("/etc/unifand/", &st) + 
        stat(configPath, &st) + 
        stat(modulePath, &st)
    ) == 0
    ) ? 0 : 1;
}

char concat(char s1[], char s2[]) {
    char return_string[strlen(s1) + strlen(s2) + 1];
    strcpy(return_string, s1);
    strcat(return_string, s2);

    return *return_string;
}

// Config Functions
int get_fan_speed(void) {

    change_fan_speed = true;

    FILE *fp = fopen(configPath, "r");
    if (fp) {
        char config[256];
        int index = 0;
        int cnt;

        struct temp_range *settings;

        while (fgets(config, sizeof(config), fp)) {

            if (index == 0) {

                char *key = strtok(config, "=");
                char *value = strtok(NULL, "=");
                
                if (value == NULL) {fan_speed = 0; return 1;}

                cnt = atoi(value);
                settings = malloc(cnt*sizeof(struct temp_range));

            }

            if (index > 0) {

                char *range = strtok(config, "=");
                char *speed = strtok(NULL, "=");
                char *min = strtok(range, "-");
                char *max = strtok(NULL, "-");

                struct temp_range tempRange;

                tempRange.min = atoi(min);
                tempRange.max = atoi(max);
                tempRange.speed = atoi(speed);
                
                settings[index - 1] = tempRange;

            }

            index++;
        }

        fclose(fp);

        int last_fan_speed = fan_speed;

        for (int i = 0; i < cnt; i++) {
            
            if (temp >= settings[i].min && temp <= settings[i].max) {
                fan_speed = settings[i].speed;
            }

            if (temp > settings[i].max && first_temp) {fan_speed = settings[i].speed;}
            first_temp = false;
        }
        
        free(settings);

        if (fan_speed == last_fan_speed) {change_fan_speed = false;}

        return 0;
    }
    return 1;
}

// Module Functions
char init_module(void) {
    char cmd[35];
    strcpy(cmd, modulePath);
    strcat(cmd, " i");
    system(cmd);

    return 0;
}

int get_temp(void) {
    char cmd[35];
    strcpy(cmd, modulePath);
    strcat(cmd, " g");
    system(cmd);

    char result[3];
    FILE *fp = popen(cmd, "r");
    if (!fp) return 1;

    if (fgets(result, sizeof(result), fp) != NULL) {}

    pclose(fp);

    temp = atoi(result);

    return 0;
};

int set_fan_speed(void) {

    if (!change_fan_speed) {return 1;}

    char cmd[35];
    char Speed[3];

    sprintf(Speed, "%d", fan_speed);

    strcpy(cmd, modulePath);
    strcat(cmd, " s ");
    strcat(cmd, Speed);

    system(cmd);

    return 0;
};

// Init
int main(void) {
    if (main_paths_exist() > 0) {
        printf("Improper File Structure");
        return 1;
    }

    init_module();

    while (0==0) {
        get_temp();
        get_fan_speed();
        set_fan_speed();
        sleep(3);
    }
    
    return 0;
}
