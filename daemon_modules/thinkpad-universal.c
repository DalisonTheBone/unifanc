#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Consts
char hwmon_path[] = "";

// Types
typedef struct {
    bool enabled;
    bool fan_control;
    int min;
    int max;
    char *name;
    size_t name_size;
} sensor;

typedef struct {
    size_t length;
    sensor sensors[];
} sensor_list;

sensor_list get_sensors() {



}

int main (int argc, char *argv[]) {

    if (argc == 1) {return 0;}
    printf("%d\n", strlen(argv[1]));
    printf("%d d", ("d" == "d"));

}
/*
## Mandatory Commands ##
g - returns highest tempature percent from sensors
s %d - sets fan speed to percent of maximum
i - sets computer so it can read sensors and set speeds (has to be able to run once at boot)

## Optional Commands ##
h - help command, returns info on module commands
d - returns default configuration
*/
