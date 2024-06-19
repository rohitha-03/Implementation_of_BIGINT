#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BIGINT_SIZE 310

typedef struct BIG_INT_TAG
{
	int *num;
	int sign;	//1 if negative & 0 if positive
	int length;
} BigInt;

//To print the bigint number
void print_bigint(BigInt *n)
{
    int i = n->length-1; 
    int flag=0;   
    // Remove leading zeros
    while( i > 0  && flag==0)
    {
        if(n->num[i]==0)
        {
          i--;
        }
        else
        {
            flag=1;
        }
    }
    if (n->sign == 1)
    {
        printf("-");
    }
    n->length=i+1;
    for(int i=n->length-1;i>=0;i--)
    {
        printf("%d",n->num[i]);
    }   
}

//For converting into bigint datatype
BigInt string_to_bigint(char *s)
{
    BigInt big;
    int l = strlen(s);
    if (s[0] == '-')
    {
        big.sign = 1;
    } 
    else 
    {
        big.sign = 0;
    }
    big.length = l - big.sign;
    big.num = (int*)malloc(sizeof(int)*(big.length));
    int *i;
	i = big.num;
	l--;
	while(l>=big.sign)
	{
		*(i) = s[l]-'0';
		i++;
		l--;
	}

    return big;
}

//comparing two bigint variables
int compare(BigInt *n1, BigInt *n2) 
{
    int i, ret_val;
    if (n1->length == n2->length) 
    {
        i = n1->length - 1;
        while (i >= 0 && n1->num[i] == n2->num[i]) 
        {
            i--;
        }
        if (i < 0) 
        {
            ret_val = 0;
        } 
        else 
        {
            ret_val = (n1->num[i]) - (n2->num[i]);
        }
    } 
    else 
    {
        ret_val = (n1->length) - (n2->length);
    }
    return ret_val;
}

BigInt* add_bigints(BigInt *n1, BigInt *n2);
BigInt* subtract_bigints(BigInt *n1, BigInt *n2);

// ADDITION OF BIGINTS 
BigInt* add_bigints(BigInt *n1, BigInt *n2)
{
    int l1 = n1->length;
    int l2 = n2->length;
    BigInt *sum = (BigInt*)malloc(sizeof(BigInt));
    
	if(l2>l1)
		sum->num=(int*)malloc(sizeof(int)*(l2+1));
	else
		sum->num=(int*)malloc(sizeof(int)*(l1+1));
	
	//if signs are same
    if(n1->sign == n2->sign)
    {
        sum->sign=n1->sign;
        int digit,i=0;
        int carry=0;
        while (i < l1 && i < l2) 
        {
            
            digit = ((n1->num[i]) + (n2->num[i]) + carry);
            carry = (digit / 10);
            digit = (digit % 10);
            sum->num[i] = digit;
            i++;
        }
        while (i < l1) 
        {
            digit = (n1->num[i]) + carry;
            carry = digit / 10;
            digit = digit % 10;
            sum->num[i] = digit;
            i++;
        }
        while (i < l2) 
        {
            digit = (n2->num[i]) + carry;
            carry = digit / 10;
            digit = digit % 10;
            sum->num[i] = digit;
            i++;
        }
        if (carry != 0) 
        {
            sum->num[i] = (carry % 10);
            carry /= 10;
            i++;
        }
        sum->length = i;
    }
    //if signs are different
    else
    {
       if (n1->sign == 1) //(-n1+n2)
       {
            n2->sign = 1;
            sum = subtract_bigints(n1, n2);
            n2->sign = 0;
        } 
        else //(n1-n2)
        {
            n2->sign = 0;
            sum = subtract_bigints(n1, n2);
            n2->sign = 1;
        }
    }
    return sum;	
}

//SUBTRACTION OF BIGINTS
BigInt* subtraction(BigInt *n1, BigInt *n2) //Here we are considering (n1 > n2) 
{
    BigInt *diff = (BigInt*)malloc(sizeof(BigInt));
    diff->num=(int*)malloc(sizeof(int)*(n1->length));
    int  digit;
    int  l1=n1->length;
    int  l2=n2->length;
    int k;
    for(k=0;k<l2;k++)
    {

        if((n1->num[k])>=(n2->num[k]))
        {
            digit=(n1->num[k])-(n2->num[k]);
        }
        else
        {
            digit=(n1->num[k])-(n2->num[k])+10;
            (n1->num[k+1]) = (n1->num[k+1])-1;
        }
        diff->num[k] = digit;
    }
    while(k<l1)
    {
        if(n1->num[k]<0)
        {
            n1->num[k]+=10;
            n1->num[k+1]--;
        }
        diff->num[k]=n1->num[k];
        k++;
    }
    
    diff->length = k;
    return diff;
    
}

BigInt* subtract_bigints(BigInt *n1, BigInt *n2)
{
    int l1 = n1->length;
    int l2 = n2->length;
    BigInt *diff = (BigInt*)malloc(sizeof(BigInt));
    
	if(l2>l1)
		diff->num=(int*)malloc(sizeof(int)*l2);
	else
		diff->num=(int*)malloc(sizeof(int)*l1);
		
	// if signs are different
    if (n1->sign != n2->sign) 
    {
        if (n2->sign == 1) 
        {
            n2->sign = 0;
            diff = add_bigints(n1, n2);
            n2->sign = 1;
            diff->sign = 0;
        } 
        else 
        {
            n2->sign = 1;
            diff = add_bigints(n1, n2);
            n2->sign = 0;
            diff->sign = 1;
        }
    }
    else//if signs are same
    {
       if (compare(n1, n2) > 0)  // (n1>n2)
        {
            diff = subtraction(n1, n2);  // subtraction of magnitudes
            diff->sign = n1->sign;  // n1-n2   gets sign of n1
        } 
        else 
        {
            diff = subtraction(n2, n1);
            if (n1->sign == 0)  // if sign is positive then n1-n2 and n1<n2  diff sign is negative (n2>n1)
            {    
                if(compare(n1,n2)!=0)
                   diff->sign = 1;
                else //if they are equal (n1==n2)
                   diff->sign = 0;   
            }    
            else   // as signs are same and they are negative -n1-(-n2) and n2-n1 diff sign is positive (n2>n1)
                diff->sign = 0;
        }
    }   
    return diff; 	
}

//MULTIPLICATION OF BIGINTS
BigInt* mult_bigints(BigInt *n1, BigInt *n2)
{
    BigInt *result = (BigInt*)malloc(sizeof(BigInt));
    result->length = n1->length + n2->length;
    //result->num=(int*)malloc(sizeof(int)*(result->length));
    result->num=(int*)calloc((result->length),sizeof(int));
	
	//for(int i=0; i<result->length; i++)//initialising 
		//result->num[i]=0;
	
	for (int i = 0; i < n1->length; i++) 
    {
        for (int j = 0; j < n2->length; j++) 
        {
            result->num[i + j] += (n1->num[i]) * (n2->num[j]);
            result->num[i + j + 1] += result->num[i + j] / 10;
            result->num[i + j] %= 10;
        }
    }
    
    if (n1->sign == n2->sign)
        result->sign = 0;
    else
        result->sign = 1;
    
   // Remove leading zeros
    while (result->length >=1  && result->num[result->length - 1] == 0) 
    {
        result->length--;
    }
    
    if (result->length == 0)  // if input is 0, product is 0
    {
        result->sign = 0;  // if mult by negative number, - sign comes
        result->length = 1;
    }

    return result;
}

int main()
{
	BigInt num1, num2;
	char *b1, *b2;
	b1 = (char*)malloc(sizeof(char)*BIGINT_SIZE);//DYNAMIC ALLOCATION OF THE STRINGS
	b2 = (char*)malloc(sizeof(char)*BIGINT_SIZE);
	
	printf("Enter number 1 : ");
	scanf("%s", b1);
	printf("Enter number 2 : ");
	scanf("%s", b2);
	
	num1 = string_to_bigint(b1);  
    num2 = string_to_bigint(b2);
	
	BigInt *ans = add_bigints(&num1, &num2);
	printf("SUM OF 2 NUMBERS : ");
	print_bigint(ans);
	
	num1 = string_to_bigint(b1);  
    num2 = string_to_bigint(b2);
	
	ans = subtract_bigints(&num1, &num2);
	printf("\nDIFFERENCE OF 2 NUMBERS : ");
	print_bigint(ans);
	
	num1 = string_to_bigint(b1);  
    num2 = string_to_bigint(b2);
	
	ans = mult_bigints(&num1, &num2);
	printf("\nPRODUCT OF 2 NUMBERS : ");
	print_bigint(ans);
	
	free(ans);
	free(b1);
	free(b2);
	
	return 0;
}


