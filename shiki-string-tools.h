#ifndef __SHIKI_STRING_TOOLS_H__
#define __SHIKI_STRING_TOOLS_H__

#include <stdint.h>

void sstr_print_hex_data_from_array(char* information, unsigned char *array, int array_size);
void sstr_convert_long_to_string(unsigned char *_buff_dest, long _number);
void sstr_convert_string_to_hex_string(unsigned char *_buff_dest, unsigned char *_buff_source, uint8_t _buff_source_size);
void sstr_convert_hex_string_to_hex_data(unsigned char *_buff_dest, unsigned char *_buff_source, uint8_t _buff_source_size);
void sstr_convert_hex_data_to_hex_string(unsigned char* _buff_dest, unsigned char *_buff_source, uint8_t _buff_source_size);
void sstr_convert_hex_string_to_long(long *_dest, unsigned char *_hex_source, uint8_t _hex_size);
void sstr_convert_hex_data_little_endian_to_long(long *_dest, unsigned char *_hex_source, uint8_t _hex_size);
void sstr_convert_hex_string_little_endian_to_long(long *_dest, unsigned char *_hex_source, uint8_t _hex_size);
void sstr_convert_long_to_hex_string_little_endian(long _source, unsigned char *_hex_buff_dest, uint8_t _hex_size);
void sstr_convert_long_to_hex_data(long _long, unsigned char *_hex_buff, uint8_t _size_hex);
void sstr_convert_time_to_hex_string(long _time, unsigned char *_hex_buff);
void sstr_convert_long_to_money_format(long _nominal, unsigned char *_target_buff);

void sstr_get_some_str(unsigned char *_buff_dest, unsigned char *_buff_source, uint16_t _from, uint16_t _until);
int8_t sstr_get_string_after_find_specific_word(unsigned char *_source_text, unsigned char *_specific_word, unsigned char *_target_text, uint16_t _get_size);
int8_t sstr_remove_word_from_string(char *_text, char *_target_remove);
void sstr_to_lower(char *_buff);
void sstr_to_upper(char *_buff);
int8_t sstr_find_word_normal(char *_source_text, char *_word_target);
int8_t sstr_find_word_deep(char *_source_text, char *_word_target);
#endif
