#include <stdio.h>
#include <string.h>
int binary[32];
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
	printf("  ");
	for (int i = 0; i < length; i++) {
		printf("%d ", binary[i]);
	}
}

int isAllOnes(int start, int length) {
	for(int i = start; i < start + length; i++) {
		if(binary[i] == 0) {
			return 0;
		}
	}
	return 1;
}
int isAllZeros(int start, int length) {
	for(int i = start; i < start + length; i++) {
		if(binary[i] == 1) {
			return 0;
		}
	}
	return 1;
}
void incrementBinary(int start) {
	binary[start + 12] = 0;
	int carry = 1;
	for (int i = start+11; i >= start; i--) {
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
	}else {
		//round up
		incrementBinary(start);
	}
}
double afterBinaryPoint(int start, int length) {
	double sum = 0;
	int power = 1;
	for(int i = start; i < start + length; i++) {
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
	for(i = size-1; i >= 0; i-=2){
		int a = 1;
		for(j = i - 1; j <= i ;j++){
			temp[size - i - a] = hex[j];
			a--;
		}
	}
	strcpy(hex, temp);
}

long signed_int(int numberLength){
	printf(" Length:%d", numberLength);
	int power = numberLength - 1;
	long result = 0;
	for(int i = 0; i < numberLength; i++) {
		if (i == 0)
			result -= binary[i] << power;
		else
			result += binary[i] << power;
		power--;
	}
	return result;
}
unsigned int unsigned_int(int start,int numberLength){
	printf(" Length:%d", numberLength);
	int power = numberLength - 1;
	unsigned int result = 0;
	for(int i = start; i < start + numberLength; i++) {
		result += binary[i] << power;
		power--;
	}
	return result;
}

double floating_point(int byte){
	printf(" Length:%d", byte*8);
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
			if (binary[0] == 1) {/*return minus_infinity*/}
			else {/*return infinity;*/}
		}
		else {/*return NaN*/}
	}

	if(isAllZeros(1,expLen))
		E = 1-bias;
	else {
		E = (int)unsigned_int(1,expLen) - bias;
		M = 1;
	}
	printf(" E:%d", E);
	M += afterBinaryPoint(expLen+1,fracLen);
	printf(" M:%.10lf", M);
	if(binary[0] == 1) {
		if (E > 0)
			result = -1*M*(1<<E);
		else {
			E = -E;
			result = -1*M/(1<<E);
		}
	}
	else {
		if (E > 0)
			result = M*(1<<E);
		else {
			E = -E;
			result = M/(1<<E);
		}
	}
	return result;
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

					toBytes(hexNum,digitCount, digitCount*4);

					switch(*argv[3]){
						case 'i': printf("  Result:%ld", signed_int(digitCount*4));
							break;
						case 'u': printf("  Result:%u", unsigned_int(0, digitCount*4));
							break;
						case 'f': printf("  Result:%.10lf", floating_point(digitCount/2));
							break;
					}
					i = 0;
				}
				else i++;
			}
		}
	}
}
