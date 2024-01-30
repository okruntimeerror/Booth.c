#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void readMultiplicationOperation(FILE* pInfile, char** pN1, char** pN2);

char* decimalToBinary(int input);

char* take2sComplement(char* in);
char* boothMultiply(char* n1, char* n2);
char *twos_complement(char *bits);
void rightShift(char* product_and_q_arr, int twice_mult_length);

int binaryToDecimal(char* binary) {
    int decimal = 0;
    int power = 1;
    int len = strlen(binary);
    int is_negative = binary[0] == '1';

    for (int i = len - 1; i >= 0; i--) {
        if (i == 0 && is_negative) {
            decimal -= power;

        } 
        else if (binary[i] == '1') {
            decimal += power;
        }
        power *= 2;
    }

    return decimal;
}


char *add_binary_new(char *product_and_q_arr, const char *multiplicand, int mult_length) {
    int len1 = mult_length;
    int len2 = mult_length;
    int len = len1;
    char *result = (char *) malloc((len + 2) * sizeof(char)); // Add extra space for carry and null terminator

    int carry = 0;
    int sum = 0;
    int i = len1 - 1;
    int j = len2 - 1;
    int k = len + 1;


    while (i >= 0 || j >= 0 || carry > 0) {

        int bit1 = i >= 0 ? product_and_q_arr[i] - '0': 0;
        int bit2 = j >= 0 ? multiplicand[j] - '0' : 0;

        sum = bit1 + bit2 + carry;
        result[k] = sum % 2 + '0';
        carry = sum / 2;
        i--;
        j--;
        k--;
    }

    for (int l = 0; l < mult_length; l++)
    {
        product_and_q_arr[l] = result[l+2];
    }

}

// multiply the binary numbers together
// if 1 is on it is negative and goes to twos complement
// and shifts
char* boothMultiply(char* n1, char* n2)
{
    //  5 numba (multiplicand) x 4 (multiplier) = 20 (product)
    printf("Product\t\tQ-1\tM\t-M\tLog\n");
    printf("----------------------------------------------------\n");

    int mult_length = strlen(n1);
    char* multiplicand = n1;
    char* twos_multiplicand = twos_complement(n1);
    char* multiplier = n2;
    char* product_and_q = (char *)malloc((2*mult_length+1) * sizeof(char));
    char* product_holder = (char *)malloc((2*mult_length + 1) * sizeof(char));
    char q_holder = '0';
    // allocating new product and q array

    for (int i = 0; i < mult_length; i++) {
        product_and_q[i] = '0';
    }
    // Fill with 0
    for (int j = mult_length; j < (2*mult_length); j++) {
        product_and_q[j] = multiplier[j-mult_length];
    }
    for (int i = 0; i < 2*mult_length; i++) {
        product_holder[i] = product_and_q[i];
    }
    product_holder[2*mult_length] ='\0';
    //Fill with le multiplier

    product_and_q[2*(mult_length)] = '0';
    printf("%s\t%c\t%s\t%s\tPopulate Data\n", product_holder,q_holder, n1, n2);
    int step = 0;
    while(step < mult_length) {
        if (product_and_q[2*mult_length - 1] == '0' && product_and_q[2*mult_length] == '1') {
            add_binary_new(product_and_q, multiplicand, mult_length);
            for (int i = 0; i < 2*mult_length; i++) {
                product_holder[i] = product_and_q[i];
            }
            q_holder = product_and_q[2*mult_length];
            printf("%s\t%c\t%s\t%s\tA = A + M\n", product_holder,q_holder, n1, n2);
        } else if (product_and_q[2*mult_length - 1] == '1' && product_and_q[2*mult_length] == '0') {
            add_binary_new(product_and_q, twos_multiplicand, mult_length);
            for (int i = 0; i < 2*mult_length; i++) {
                product_holder[i] = product_and_q[i];
            }
            q_holder = product_and_q[2*mult_length];
            printf("%s\t%c\t%s\t%s\tA = A - M\n", product_holder,q_holder, n1, n2);
        }

        int temp = '0';
        if (product_and_q[0] == '1')
            temp = '1';
        rightShift(product_and_q, (2*(mult_length)));
        product_and_q[0] = temp;
        for (int i = 0; i < 2*mult_length; i++) {
                product_holder[i] = product_and_q[i];
        }
        q_holder = product_and_q[2*mult_length];
        printf("%s\t%c\t%s\t%s\tShift\n", product_holder,q_holder, n1, n2);
        step++;
    }

    printf("\n%s * %s = %s", n1, n2, product_holder);

    printf("\n%d * %d = %d", binaryToDecimal(n1),binaryToDecimal(n2), binaryToDecimal(product_holder));


}

void rightShift(char* product_and_q_arr, int twice_mult_length)
{

    // Right shift the whole thang, remember to replace the first element with '0' after loop
    for (int i = twice_mult_length; i > 0; i--) {
        product_and_q_arr[i] = product_and_q_arr[i-1];
    }
    product_and_q_arr[0] = '0';

}
char *twos_complement(char *bits) {
    int carry = 1;
    int len = strlen(bits);
    char *result = (char *) malloc((len + 1) * sizeof(char));

    // Flip all the bits
    for (int i = 0; i < len; i++) {
        if (bits[i] == '0') {
            result[i] = '1';
        } else {
            result[i] = '0';
        }
    }
    result[len] = '\0';

    // Add 1 to the flipped bits
    for (int i = len - 1; i >= 0; i--) {
        if (carry == 0) {
            break;
        }
        if (result[i] == '0') {
            result[i] = '1';
            carry = 0;
        } else {
            result[i] = '0';
        }
    }

    return result;
}



void readMultiplicationOperation(FILE* pInfile, char** pN1, char** pN2)
{
    int numBits;

    fscanf(pInfile, "%d", &numBits);


    printf("\n");

    printf("number of bits is %d\n", numBits);
    *pN1 = (char *)malloc(numBits + 1);

    fscanf(pInfile, "%s", *pN1);

    *pN2 = (char*)malloc(numBits + 1);

    fscanf(pInfile, "%s", *pN2);

    boothMultiply(*pN1, *pN2);


}

int main()
{

    FILE* pInfile;
    int n = 0;
    char *n1 = NULL;
    char *n2 = NULL;


    pInfile = fopen("input.txt", "r");

    if (pInfile == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }


    if(pInfile!= NULL)
    {
        fscanf(pInfile,"%d", &n);


        printf("%d\n", n);
        for(int i = 0; i < n; i++)
        {
            readMultiplicationOperation(pInfile, &n1, &n2);
           // printf("%s %s\n", n1, n2);

        }

    }

    free(n1);
    free(n2);
    fclose(pInfile);

    return 0;
}