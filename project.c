#include <stdio.h>
#include <string.h>
int binary[32];
void toBytes(char* hex,int size){// from hexadecimal to 1 & 0's
	int correspondingInt = 0;
	short binaryCounter = 0;
	int i,j;
	for(i = 0; i < size; i++){
		if(hex[i] >= '0' && hex[i] <= '9'){
			correspondingInt = hex[i] - '0';
		}
		else if(hex[i] >= 'A' && hex[i] <= 'F'){
			correspondingInt = hex[i] - 'A' + 10;
		}
		else if(hex[i] >= 'a' && hex[i] <= 'f'){
			correspondingInt = hex[i] - 'a' + 10;
		}
		else printf("Wrong character in file!!!");

		for(j = 3; j >= 0; j--){
			binary[binaryCounter] = correspondingInt>>j & 1;
			binaryCounter++;
		}
	}					 
} 

void reverse(char* hex,int size){
	char temp[8];
	int i,j;
	char tempDgt;
	for(i = size-1; i >= 0; i-=2){
		int a = 1;
		for(j = i - 1; j <= i ;j++){
			temp[size - i - a] = hex[j];
			a--;
		}
	}
	strcpy(hex, temp);
}

signed_int(){
	
}
unsigned_int(){
	
}
floating_point(){
	printf("\nfloat");
}
round(){
	
}

int main(int argc, char *argv[]) {
	int i;
    for (i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

	FILE *fptrin;
	if((fptrin = fopen(argv[1], "r")) == NULL){
		puts("File could not be found");
	}
	else {
		int digitCount = (*argv[4] - '0') * 2; 		//how many digit are used for our numbers. (byte number * 2)
														//also, the number of exponent bits in floating points is according to this.			
		char hexDigit;
		char hexNum[8];
		int i = 0;
		while((hexDigit = fgetc(fptrin)) && hexDigit != EOF){
			if(hexDigit == ' ' || hexDigit == '\n') continue;
			else{
				hexNum[i] = hexDigit;
				if(i + 1 == digitCount){
					
					if(*argv[2] == 'l'){
						reverse(hexNum,digitCount);
					}
					
					int u;
					printf("\n");
					for(u=0; u < digitCount; u++){
						printf("%c", hexNum[u]);
					}
					
					toBytes(hexNum,digitCount);
		
					int binaryDigitCount = digitCount * 4;
					switch(*argv[3]){
						case 'i': signed_int();
							break;
						case 'u': unsigned_int();
							break;
						case 'f': floating_point();
							break;
					}
					i = 0;
				}
				else i++;
			}				
		}
	}
}
