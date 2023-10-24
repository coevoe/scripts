#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libnotify/notify.h>
#include <libnotify/notification.h>

int batt_cap(FILE *batt_cap);
int batt_status(FILE *batt_status);

int batt_status(FILE *batt_status) {
    char str[20];

    fgets(str, 20, batt_status);
    if (!strncmp(str, "Charging\n", 20)) {
        return 0;
    } else if (!strncmp(str, "Discharging\n", 20)) {
        return 1;
    } else if (!strncmp(str, "Not charging\n", 20)) {
        return 2;
    } else if (!strncmp(str, "Full\n", 20)) {
        return 3;
    }
    return -1;
}
int batt_cap(FILE *batt_cap) {
    int cap;

    if (!fscanf(batt_cap, "%3d", &cap)) {
        return -1;
    } else {
        return cap;
    }
}
int main() {
    FILE *fp1, *fp2;
    NotifyNotification *battacpi;

    char summary[20];
    char body[70];
    char *appname= "Battery alerter";

    fp1 = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    fp2 = fopen("/sys/class/power_supply/BAT0/status", "r");

    int cap = batt_cap(fp1);
    int status = batt_status(fp2);

    if (cap == -1) {
        fprintf(stderr, "Could not obtain battery capacity!\n");
        fclose(fp1);
        fclose(fp2);
        exit(EXIT_FAILURE);
    } else {
        snprintf(summary, 20, "%d%% baterky!", cap);
        snprintf(body, 70, "Tvoja kapacita baterky je %d%%! Zapoj nabíjačku do počítača!", cap);
    }

    notify_init(appname);
    battacpi = notify_notification_new(summary, body, NULL);
    notify_notification_set_urgency(battacpi, NOTIFY_URGENCY_CRITICAL);
    notify_notification_set_timeout(battacpi, 7500);

    switch (status) {
        case 0:
            printf("BAT +%d%%\n", cap);
        break;
        case 1:
            if (cap <= 15) {
                notify_notification_show(battacpi, NULL);
                printf("BAT -%d%%\n", cap);
            }
            printf("BAT -%d%%\n", cap);
        break;
        case 2:
            printf("BAT =%d%%\n", cap);
        break;
        case 3:
            printf("BAT *%d%%\n", cap);
        break;
        case -1:
            fprintf(stderr, "Could not obtain battery status!\n");
        break;
        default:
            fprintf(stderr, "Unknown value!\n");
        break;
    }

    fclose(fp1);
    fclose(fp2);
    return EXIT_SUCCESS;
}
