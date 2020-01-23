#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "shiki-string-tools.h"

int main(){
    char buff_test[33];
    char buff_hex[67];
    time_t time_now;

    long balance = 123456789;
    time(&time_now);

    printf("%li\n", time_now);

    sstr_convert_long_to_string((unsigned char *) buff_test, time_now);
    printf("test long to string: %s\n", buff_test);

    sstr_convert_string_to_hex_string((unsigned char *) buff_hex, (unsigned char *) buff_test, strlen(buff_test));
    printf("test string to hex string: %s\n", buff_hex);

    sstr_convert_hex_string_to_hex_data((unsigned char *) buff_test, (unsigned char *) buff_hex, strlen(buff_hex));
    printf("test hex string to hex data: %s\n", buff_test);

    sstr_convert_long_to_hex_string_little_endian(time_now, (unsigned char*) buff_test, 8);
    printf("test long to hex string litle endian: %s\n", buff_test);

    time_now = 0;
    sstr_convert_hex_string_to_long(&time_now, (unsigned char*) buff_test, 8);
    printf("test hex string to long: %li\n", time_now);

    time_now = 0;
    sstr_convert_hex_string_little_endian_to_long(&time_now, (unsigned char *) buff_test, strlen(buff_test));
    printf("test hex string litle endian to long: %li\n", time_now);

    time_now = 0;
    sstr_convert_hex_string_to_hex_data((unsigned char *) buff_hex, (unsigned char *) buff_test, strlen(buff_test));
    sstr_print_hex_data_from_array("tes hex string to hex data", (unsigned char *) buff_hex, 4);
    sstr_convert_hex_data_little_endian_to_long(&time_now, (unsigned char *) buff_hex, 4);
    printf("test hex data litle endian to long: %li\n", time_now);

    sstr_convert_time_to_hex_string(time_now, (unsigned char *) buff_test);
    printf("tes time to hex string: %s\n", buff_test);

    sstr_convert_long_to_money_format(balance, (unsigned char *) buff_test);
    printf("tes long to money format: %s\n", buff_test);

    return 0;
}