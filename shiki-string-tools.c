#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shiki-string-tools.h"

void sstr_print_hex_data_from_array(char* information, unsigned char *array, int array_size){
    printf("%s : \n", information);
    int i = 0;
    while(i < array_size)
    {
        printf("%02X ",array[i]);
        i++;
		if (i%10==0) printf("\n");
    }
    printf("\n");
}

void sstr_convert_long_to_string(unsigned char *_buff_dest, long _number){
	unsigned char buff[21];
	memset(buff, 0x00, 21*sizeof(char));
	sprintf((char *) buff, "%li", _number);
	memcpy(_buff_dest, buff, strlen((char *) buff));
}

void sstr_convert_string_to_hex_string(unsigned char *_buff_dest, unsigned char *_buff_source, uint8_t _buff_source_size){
	memset(_buff_dest, 0x00, 1+2*_buff_source_size*sizeof(char));
	uint8_t idx_buff=0;
	uint8_t i=0;
	for (i=0; i<_buff_source_size; i++){
		sprintf((char*)(_buff_dest+idx_buff),"%02X", _buff_source[i]);
		idx_buff+=2;
	}
}

void sstr_convert_hex_string_to_hex_data(unsigned char* _buff_dest, unsigned char *_buff_source, uint8_t _buff_source_size){
	unsigned char hex_str[3];
	uint16_t dec = 0;
	memset(_buff_dest, 0x00, (_buff_source_size/2)*sizeof(char));
	uint8_t i = 0;
	for(i=0; i<_buff_source_size; i+=2){
		sstr_get_some_str(hex_str, _buff_source, i, i+2);
		dec = 0;
		if(hex_str[0]<='9' && hex_str[0] >= '0') dec = (hex_str[0]-'0')*16;
		else if (hex_str[0]<='F' && hex_str[0] >= 'A') dec = (hex_str[0]-'A' + 10)*16;
		else if (hex_str[0]<='f' && hex_str[0] >= 'a') dec = (hex_str[0]-'a' + 10)*16;
		if(hex_str[1]<='9' && hex_str[1] >= '0') dec = dec + (hex_str[1]-'0');
		else if (hex_str[1]<='F' && hex_str[1] >= 'A') dec = dec + (hex_str[1]-'A' + 10);
		else if (hex_str[1]<='f' && hex_str[1] >= 'a') dec = dec + (hex_str[1]-'a' + 10);
		_buff_dest[i/2] = dec;
	}
	_buff_dest[_buff_source_size/2] = 0x00;
}

void sstr_convert_hex_data_to_hex_string(unsigned char* _buff_dest, unsigned char *_buff_source, uint8_t _buff_source_size){
	unsigned char buff_tmp[_buff_source_size * 2 + 1];
	unsigned char hex_tmp[3];
	memset(buff_tmp, 0x00, (_buff_source_size * 2 + 1)*sizeof(char));
	uint8_t i = 0;
	for (i=0; i<_buff_source_size; i++){
		memset(hex_tmp, 0x00, 3*sizeof(char));
		sprintf((char *)hex_tmp, "%02X" , _buff_source[i]);
		strcat((char *)buff_tmp, (char *)hex_tmp);
	}
	memcpy(_buff_dest, buff_tmp, _buff_source_size * 2 + 1);
}

void sstr_convert_hex_string_to_long(long *_dest, unsigned char *_hex_source, uint8_t _hex_size){
	unsigned long dec = 0;
	uint8_t i = 0;
	for (i=0; i<_hex_size; i++){
		uint32_t pkt = 1;
		uint8_t j = 0;
		for (j=(_hex_size - 1) - i; j>0; j--) pkt=pkt*16;
		if(_hex_source[i]<='9' && _hex_source[i] >= '0') dec = dec + (_hex_source[i]-'0')*pkt;
		else if (_hex_source[i]<='F' && _hex_source[i] >= 'A') dec = dec + (_hex_source[i]-'A' + 10)*pkt;
		else if (_hex_source[i]<='f' && _hex_source[i] >= 'a') dec = dec + (_hex_source[i]-'a' + 10)*pkt;
	}
	*_dest = (long) dec;
}

void sstr_convert_hex_data_little_endian_to_long(long *_dest, unsigned char *_hex_source, uint8_t _hex_size){
	long dec=0;
	unsigned char buff_tmp[_hex_size * 2 + 1];
	memset(buff_tmp, 0x00, (_hex_size * 2 + 1)*sizeof(char));
	sstr_convert_hex_data_to_hex_string(buff_tmp, _hex_source, _hex_size);
	sstr_convert_hex_string_little_endian_to_long(&dec, buff_tmp, _hex_size*2);
	*_dest = (long) dec;
}


void sstr_convert_hex_string_little_endian_to_long(long *_dest, unsigned char *_hex_source, uint8_t _hex_size){
	unsigned char hex_tmp[_hex_size+1];
	memset(hex_tmp, 0x00, (_hex_size+1)*sizeof(char));
	uint8_t i = 0;
	for(i=0; i<_hex_size; i+=2){
		hex_tmp[i]=_hex_source[_hex_size-2-i];
		hex_tmp[i+1]=_hex_source[_hex_size-1-i];
	}
	unsigned long dec=0;
	for (i=0; i<_hex_size; i++){
		uint32_t pkt=1;
		uint8_t j = 0;
		for (j=(_hex_size - 1) - i; j>0; j--) pkt=pkt*16;
		if(hex_tmp[i]<='9' && hex_tmp[i] >= '0') dec = dec + (hex_tmp[i]-'0')*pkt;
		else if (hex_tmp[i]<='F' && hex_tmp[i] >= 'A') dec = dec + (hex_tmp[i]-'A' + 10)*pkt;
		else if (hex_tmp[i]<='f' && hex_tmp[i] >= 'a') dec = dec + (hex_tmp[i]-'a' + 10)*pkt;
	}
	*_dest = (long) dec;
}

void sstr_convert_long_to_hex_string_little_endian(long _source, unsigned char *_hex_buff_dest, uint8_t _hex_size){
	unsigned char hex_tmp[_hex_size+1], hex_tmp2[_hex_size+1];
	memset(hex_tmp, 0x00, (_hex_size+1)*sizeof(char));
	sstr_convert_long_to_hex_data(_source, hex_tmp, _hex_size);

	memset(hex_tmp2, 0x00, (_hex_size+1)*sizeof(char));
	uint8_t i = 0;
	for(i=0; i<_hex_size; i+=2){
		hex_tmp2[i]=hex_tmp[_hex_size-2-i];
		hex_tmp2[i+1]=hex_tmp[_hex_size-1-i];
	}
	memcpy(_hex_buff_dest, hex_tmp2, _hex_size + 1);	
}

void sstr_convert_long_to_hex_data(long _long, unsigned char *_hex_buff, uint8_t _size_hex){
	memset(_hex_buff, 0x00, (_size_hex+1)*sizeof(char));
	if (_size_hex == 2){
		sprintf((char *) _hex_buff, "%02lX", _long);
	}
	else if (_size_hex == 3){
		sprintf((char *) _hex_buff, "%03lX", _long);
	}
	else if (_size_hex == 4){
		sprintf((char *) _hex_buff, "%04lX", _long);
	}
	else if (_size_hex == 5){
		sprintf((char *) _hex_buff, "%05lX", _long);
	}
	else if (_size_hex == 6){
		sprintf((char *) _hex_buff, "%06lX", _long);
	}
	else if (_size_hex == 7){
		sprintf((char *) _hex_buff, "%07lX", _long);
	}
	else if (_size_hex == 8){
		sprintf((char *) _hex_buff, "%08lX", _long);
	}
}

void sstr_convert_time_to_hex_string(long _time, unsigned char *_hex_buff){
	memset(_hex_buff, 0x00, 9*sizeof(char));
	sprintf((char *)_hex_buff, "%08lX", _time);
}

void sstr_convert_long_to_money_format(long _nominal, unsigned char *_target_buff){
	unsigned char buff_tmp[16];
	memset(buff_tmp, 0x00, 16*sizeof(char));
	sprintf((char *) buff_tmp, "%li", _nominal);

	uint8_t len_buff_tmp = strlen((char *) buff_tmp);

	uint8_t idx_buff = 0;
	uint8_t i = 0;
	for (i=0; i<len_buff_tmp; i++){
		if (i>0 && (i-(len_buff_tmp%3))%3==0){
			_target_buff[idx_buff] = '.';
			idx_buff++;
		}
		_target_buff[idx_buff] = buff_tmp[i];
		idx_buff++;
	}
	_target_buff[idx_buff] = 0x00;
}


void sstr_get_some_str(unsigned char *_buff_dest, unsigned char *_buff_source, uint16_t _from, uint16_t _until){
	uint16_t idx_buff = 0;
	uint16_t i = 0;
	for (i=_from; i<_until; i++){
		_buff_dest[idx_buff] = _buff_source[i];
		idx_buff++;
	}
	_buff_dest[idx_buff] = 0x00;
}

int8_t sstr_get_string_after_find_specific_word(unsigned char *_source_text, unsigned char *_specific_word, unsigned char *_target_text, uint16_t _get_size){
    int8_t ret_val = -1;
    uint16_t len_buff = strlen((char *) _source_text);
    unsigned char buff_tmp[50];
    uint16_t idx_char=0;
	uint16_t idx_end=0;
    while (idx_char<=len_buff){
        memset(buff_tmp, 0x00, 50*sizeof(char));
        uint16_t idx_add = 0;
        while(_source_text[idx_char] != ' ' && _source_text[idx_char] != '\n' && _source_text[idx_char] != 0x00){
            buff_tmp[idx_add]=_source_text[idx_char];
            idx_char++;
            idx_add++;
        }
        idx_char++;
		idx_end = idx_char+_get_size;
        if(memcmp(buff_tmp, _specific_word, strlen((char *) _specific_word)) == 0 && len_buff-idx_char > 2){
			uint16_t i = 0;
            for (i=idx_char; i<idx_end && i<len_buff; i++){
                if(_source_text[i] != ' ') _target_text[i-idx_char] = _source_text[i];
                else _target_text[i-idx_char] = '+';
            }
            _target_text[len_buff-idx_char] = 0x00;
            ret_val = 0;
            break;
        }
    }
    return ret_val;
}

int8_t sstr_remove_word_from_string(char *_text, char *_target_remove){
	int8_t ret_val = 0;
    uint16_t len_buff = strlen(_text);
    char buff_tmp[33];
	char target_tmp[len_buff + 1];
    uint16_t idx_char=0;

	memset(target_tmp, 0x00, (len_buff + 1)*sizeof(char));
    while (idx_char<len_buff){
        memset(buff_tmp, 0x00, 33*sizeof(char));
        uint16_t idx_buff = 0;
        while(_text[idx_char] != ' ' && _text[idx_char] != '\n' && _text[idx_char] != 0x00){
            buff_tmp[idx_buff]=_text[idx_char];
            idx_char++;
            idx_buff++;
        }
        idx_char++;
        if (strncmp(buff_tmp, _target_remove, strlen(_target_remove)) != 0){
            strcat(target_tmp, buff_tmp);
        }
		else ret_val++;
    }
	strcpy(_text, target_tmp);
	return ret_val;
}

void sstr_to_lower(char *_buff){
    char buff_temp[strlen(_buff) + 1];
    memset(buff_temp, 0x00, strlen(_buff) + 1*sizeof(char));
	uint16_t i = 0;
    for(i=0; i<strlen(_buff) + 1 && _buff[i]!=0; i++){
        if (_buff[i]>='A' && _buff[i]<='Z') buff_temp[i] = _buff[i] - 'A' + 'a';
        else buff_temp[i] = _buff[i];
    }
    strcpy(_buff, buff_temp);
}

void sstr_to_upper(char *_buff){
    char buff_temp[strlen(_buff) + 1];
    memset(buff_temp, 0x00, strlen(_buff) + 1*sizeof(char));
	uint16_t i = 0;
    for(i=0; i<strlen(_buff) + 1 && _buff[i]!=0; i++){
        if (_buff[i]>='a' && _buff[i]<='z') buff_temp[i] = _buff[i] - 'a' + 'A';
        else buff_temp[i] = _buff[i];
    }
    strcpy(_buff, buff_temp);
}

int8_t sstr_find_word_normal(char *_source_text, char *_word_target){
    int8_t ret_val = -1;
    uint16_t len_buff = strlen(_source_text);
    char buff_tmp[20];
    uint16_t idx_char=0;
    while (idx_char<=len_buff){
        memset(buff_tmp, 0x00, 20*sizeof(char));
        uint16_t idx_buff = 0;
        while(_source_text[idx_char] != ' ' && _source_text[idx_char] != '\n' && _source_text[idx_char] != 0x00){
            buff_tmp[idx_buff]=_source_text[idx_char];
            idx_char++;
            idx_buff++;
        }
        idx_char++;
        if (strncmp(buff_tmp, _word_target, strlen(_word_target)) == 0){
            ret_val = 0;
            break;
        }
    }
    return ret_val;
}

int8_t sstr_find_word_deep(char *_source_text, char *_word_target){
    int8_t ret_val = -1;
    uint16_t len_buff = strlen(_source_text);
	uint8_t len_buff_target = strlen(_word_target);
    char buff_tmp[20];
    uint16_t idx_char=0;
	memset(buff_tmp, 0x00, 20*sizeof(char));
	strncpy(buff_tmp, _source_text, len_buff_target);
	idx_char = len_buff_target;
    while (idx_char<=len_buff){
        if (strncmp(buff_tmp, _word_target, strlen(_word_target)) == 0){
            ret_val = 0;
            break;
        }
		uint8_t i = 0;
		for (i=0; i<len_buff_target-1; i++){
			buff_tmp[i]=buff_tmp[i+1];
		}
		buff_tmp[len_buff_target-1] = _source_text[idx_char];
		idx_char++;
    }
	if (strncmp(buff_tmp, _word_target, strlen(_word_target)) == 0){
        ret_val = 0;
    }
    return ret_val;
}