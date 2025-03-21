#include <stdio.h>

toBytes(){
						 // from hexadecimal to 1 & 0's
} 
reverse(){
	printf("\nreverse");
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
						reverse();
					}
		
					toBytes();
		
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
