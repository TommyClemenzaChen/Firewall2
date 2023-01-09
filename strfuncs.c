#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
uint64_t slen(char * input){
	if(input == NULL){
		return 0;
	}
        uint64_t cou = 0;
        //End when it reaches the null value
        while(input[cou] != '\0'){
                cou++;

        }
        return cou;
}
char * sdup(char * input){
        uint64_t length = slen(input);
        char * dup = (char *)malloc(length+1);
	uint64_t i = 0;
        while(input[i] != '\0'){
		dup[i] = input[i];
		i++;
	}
	dup[i] = '\0';
        return dup;
}

bool scmp(char * str1, char *str2){
	
        uint64_t index = 0;
	uint64_t len1 = slen(str1);
	uint64_t len2 = slen(str2);
	if(len2 == 0 || len1 ==0 || len2 != len1){
		return false;

	}
        while(str1[index] != '\0' && str2[index] != '\0'){
                if(str1[index] != str2[index]){

                        return false;
                }
                index++;

        }


        return true;
}

void scpy(char * res, char * object){
	uint64_t c = 0;
	while(object[c] != '\0'){
		res[c] = object[c];
		c++;
	}
	res[c] = '\0';
	

}

