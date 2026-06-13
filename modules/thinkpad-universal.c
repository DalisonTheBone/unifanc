#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

// Variables
bool hwmon_sensors_available = false;
bool hwmon_fc_available = false;
bool proc_fc_available = false;

char fc_hwmon[7] = {0};
int hwmon_cnt = 0;

// Functions
int find_fc_hwmon(void) {

    DIR *dir = opendir("/sys/class/hwmon/");

    if (dir == NULL) {return 1;}

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        
        if (entry->d_name[0] == '.') {continue;}
        hwmon_cnt++;
        char location[36];
        strcpy(location, "/sys/class/hwmon/");
        strcat(location, entry->d_name);
        strcat(location, "/pwm1_enable");
        
        struct stat st;
        if (stat(location, &st) == 0 && fc_hwmon[0] == 0) {
            strcpy(fc_hwmon, entry->d_name);
        }

    }

    return 1;
}

int check_availablity(void) {
    struct stat st;

    // hwmon sensors
    if (stat("/sys/class/hwmon", &st) == 0) {hwmon_sensors_available = true;}

    // hwmon fan control
    find_fc_hwmon();
    if (fc_hwmon[0] != 0) {hwmon_fc_available = true;}
    
    // proc fanc control
    if (stat("/proc/acpi/ibm/fan", &st) == 0) {proc_fc_available = true;}

    return 0;
}

int enable_fc(void) {

    system("modprobe -r thinkpad_acpi");
    system("modprobe thinkpad_acpi fan_control=1");

    char hwmon_location[40];
    strcpy(hwmon_location, "/sys/class/hwmon/");
    strcat(hwmon_location, fc_hwmon);
    strcat(hwmon_location, "/pwm1_enable");

    FILE *fptr;
    fptr = fopen(hwmon_location, "w");
    fprintf(fptr, "1");
    fclose(fptr); 

    return 0;

}

// Module Functions
int init(void) {

    check_availablity();
    enable_fc();

    return 0;
}

int get_temp(void) {
    check_availablity();

    if (!hwmon_sensors_available) {printf("0"); return 1;}

    DIR *dir = opendir("/sys/class/hwmon/");

    if (dir == NULL) {return 1;}

    int temp = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        
        if (entry->d_name[0] == '.') {continue;}

        char location[36];
        strcpy(location, "/sys/class/hwmon/");
        strcat(location, entry->d_name);
        strcat(location, "/temp1_input");
        
        struct stat st;
        if (stat(location, &st) == 0) {
            
            char result[7];
            FILE *fp = fopen(location, "r");
            if (!fp) return 1;

            if (fgets(result, sizeof(result), fp) != NULL) {}

            fclose(fp);

            if (atoi(result) / 1000 > temp) {temp = atoi(result) / 1000;}

        }

    }

    printf("%d", temp);

    return 0;
}

int set_speed(int speed) {
    
    check_availablity();
    

    if (hwmon_fc_available) {

        char hwmon_location[36];
        strcpy(hwmon_location, "/sys/class/hwmon/");
        strcat(hwmon_location, fc_hwmon);
        strcat(hwmon_location, "/pwm1");

        FILE *fptr;
        fptr = fopen(hwmon_location, "w");
        fprintf(fptr, "%d", speed*255/100);
        fclose(fptr); 

    }

    if (!hwmon_fc_available && proc_fc_available) {
        printf("test");
        char proc_location[] = "/proc/acpi/ibm/fan";

        char speed_string[11] = "";

        sprintf(speed_string, "%d", speed*8/100);

        FILE *fptr;
        fptr = fopen(proc_location, "w");
        fprintf(fptr, "level %s", speed_string);
        if (speed*8/100 == 0) {fprintf(fptr, "level auto");}
        if (speed*8/100 == 8) {fprintf(fptr, "level disengaged");}
        if (speed*8/100 != 0 && speed*8/100 != 8) {fprintf(fptr, "level %d", speed*8/100);}

        fclose(fptr); 

    }

    return 0;
}

// init
int main(int argc, char *argv[]) {
    //check_availablity();
    //printf("%s", fc_hwmon);
    if (hwmon_fc_available) {printf("yay");}
    if (argc == 1) {return 1;}

    if (*argv[1] == 'i') {init();}
    if (*argv[1] == 'g') {get_temp();}

    if (*argv[1] == 's' && argc >= 3) {set_speed(atoi(argv[2]));}
    
    return 0;
}