#include <stdio.h>
#include <string.h>
#include <math.h>

int binary[32];
FILE *oFile;
void toBytes(char* hex,int size, int length){// from hexadecimal to 1 & 0's
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

int isAllOnes(int start, int length) {
	int i;
	for(i = start; i < start + length; i++) {
		if(binary[i] == 0) {
			return 0;
		}
	}
	return 1;
}
int isAllZeros(int start, int length) {
	int i;
	for(i = start; i < start + length; i++) {
		if(binary[i] == 1) {
			return 0;
		}
	}
	return 1;
}
void incrementBinary(int start) {
	int carry = 1;
	int i;
	for (i = start+12; i >= start; i--) {
		if (carry == 0) break;
		else if (binary[i] == 0 && carry) {
			binary[i] = 1;
			carry = 0;
		}
		else if (binary[i] == 1 && carry)
			binary[i] = 0;
	}
}
void round_(int start, int length){
	if (isAllZeros(start + 14,length - 14)) {
		//half-way
		if (binary[start+ 12] == 0) {/*round down*/}
		else {
			//round up
			incrementBinary(start);
		}
	}
	else {
		//round up
		incrementBinary(start);
	}
}
double afterBinaryPoint(int start, int length) {
	double sum = 0;
	int power = 1;
	int i;
	for(i = start; i < start + length; i++) {
		if(binary[i] == 1)
			sum += 1.0/(binary[i]<<power);
		power++;
	}
	return sum;
}

void reverse(char* hex,int size){
	char temp[8];
	int i,j;
	char tempDgt;

	for(i = size-1, j = 0; i > 0; i-=2, j++){
		temp[j] = hex[i-1];
		temp[++j] = hex[i];
	}
	toBytes(temp, size, size*4);
}

long signed_int(int numberLength){
	int power = numberLength - 1;
	long result = 0;
	int i;
	for(i = 0; i < numberLength; i++) {
		if (i == 0)
			result -= binary[i] << power;
		else
			result += binary[i] << power;
		power--;
	}
	fprintf(oFile,"%ld ",result);
	return result;
}

unsigned int unsigned_int_for_floating(int start,int numberLength){
	int power = numberLength - 1;
	unsigned int result = 0;
	int i;
	for(i = start; i < start + numberLength; i++) {
		result += binary[i] << power;
		power--;
	}
	return result;
}

unsigned int unsigned_int(int start,int numberLength){
	int power = numberLength - 1;
	unsigned int result = 0;
	int i;
	for(i = start; i < start + numberLength; i++) {
		result += binary[i] << power;
		power--;
	}
	fprintf(oFile,"%u ",result);
	return result;
}

double floating_point(int byte){
	int expLen = byte*2+2;
	int fracLen = byte*8 - expLen - 1;
	int bias = (1<<(expLen-1)) - 1;
	int E = 0;
	double M = 0;
	double result = 0;

	if(fracLen > 13) {
		if(binary[expLen + 14] == 1)
			round_(expLen + 1, fracLen);
		fracLen = 13;
	}

	if (isAllOnes(1, expLen)) {
		if(isAllZeros(expLen+1, fracLen)) {
			if (binary[0] == 1) {/*return minus_infinity*/	fprintf(oFile,"-inf "); return 0;}
			else {/*return infinity;*/ 	fprintf(oFile,"inf "); return 0;}
		}
		else {fprintf(oFile,"NaN "); return 0;}
	}

	if(isAllZeros(1,expLen))
		E = 1-bias;
	else {
		E = (int)unsigned_int_for_floating(1,expLen) - bias;
		M = 1;
	}
	
	M += afterBinaryPoint(expLen+1,fracLen);

	if(binary[0] == 1) {
		result = -1*M*pow(2,E);
		if(result==0.0){
			fprintf(oFile,"-0 ");
			return 0;
		}	
		if(E == 1-bias)
		fprintf(oFile,"%.5e ",(-1*M*pow(2,E)));
		else
		fprintf(oFile,"%.5f ",(-1*M*pow(2,E)));
	}
	else {
		result = M*pow(2,E);
		if(result==0.0){
			fprintf(oFile,"0 ");
			return 0;
		}	
		if(E == 1-bias)
		fprintf(oFile,"%.5e ",(M*pow(2,E)));
		else
		fprintf(oFile,"%.5f ",(M*pow(2,E)));
	}
	return result;
}
int main(int argc, char *argv[]) {
	
	if((oFile = fopen("output.txt", "w")) == NULL){
		puts("File could not be found");
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
			if(hexDigit == ' ') continue;
			else if(hexDigit == '\n'){
				fprintf(oFile,"\n");
			}
			else{
				hexNum[i] = hexDigit;
				if(i + 1 == digitCount){

					if(*argv[2] == 'l'){
						reverse(hexNum,digitCount);
					}
					else
						toBytes(hexNum,digitCount, digitCount*4);

					if (*argv[3] == 'i') 
    						signed_int(digitCount * 4); 
					else if (*argv[3] == 'u') 
    						unsigned_int(0, digitCount * 4);
					else if (strcmp(argv[3], "fp") == 0) 
    						floating_point(digitCount / 2);

					i = 0;
				}
				else i++;
			}
		}
	}
	fclose(oFile);
}
