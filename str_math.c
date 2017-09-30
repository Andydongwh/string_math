#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int a;
    int b;
    char* c;
}str;


/*find point*/
str find_dot(char* string, int len) {

    int zeros = 0;
    int l;
    for (l = 0; l < len; l++) {
        if ((string[l] != '0') || (string[l] == '0' && string[l+1] == '.')) {
            break;
        }
        zeros++;
    }
    if (zeros > 0) {
        if (zeros == len) {
            string += (len - 1);
            len = 1;
        }
        else {
            string += zeros;
            len -= zeros;
        }
    }

    int pos;
    for (pos = 0; pos < len; pos++) {
        if (string[pos] == '.') {

            break;
        }
    }
    str change;
    change.a = pos;
    change.b = len;
    change.c = string;

    return change;
}

/*add the minus sign*/
void append(char *s) {  
    int  i,j;
    int len = strlen(s);
    s[len] = '-';
    int n = len;
    char tmp[100];
    strcpy(tmp,s+n);
    strcpy(tmp+1,s);
    *(tmp+n+1)='\0';
    strcpy(s,tmp); 
}

/*str1+str2*/
str integer_add(char* str1, int len1, char* str2, int len2, char* out, int carry) {
    int res; 
    int l1 = len1-1, l2 = len2-1;
    int olen = len1 > len2 ? len1+1 : len2+1;
    int l = olen-1;
    int len ;
    for (len = len1 < len2 ? len1 : len2; len > 0; l1--, l2--, len--, l--) {
        res = str1[l1] - '0' + str2[l2] - '0' + carry;
        carry = res>9;
        res = carry ? res-10: res;
        out[l] = res+'0';
    }   

    for (; l1 >= 0; l1--, l--) {
        res = str1[l1] - '0' + carry;
        carry = res>9;
        res = carry ? res-10: res;
        out[l] = res+'0';
    }
    for (; l2 >= 0; l2--, l--) {
        res = str2[l2] - '0' + carry;
        carry = res>9;
        res = carry ? res-10: res;
        out[l] = res+'0';
    }
    int shift = 0;
    if (carry == 1) {
        out[l--] = '1';
    }
    else {
        olen -= 1;
        for (l = 0; l < olen; l++) {
            out[l] = out[l+1];
        }
        shift = 1;
    }
    str integer; 
    integer.a = shift;
    integer.b = olen;
    return integer;
}


str fraction_add(int type, char* str1, int len1, const char* str2, int len2, char* out) {
    int carry = 0;
    int left = len1 < len2 ? len1 : len2; 
    int right = len1 > len2 ? len1 : len2; 
    const char* longer = len1  > len2 ? str1 : str2;
    int olen = right;
    int l = olen-1;
    for (--left; --right != left;) {
        out[l--] = longer[right];
    }

    size_t res;
    for (; left >= 0; --left) {
        res = str1[left] - '0' + str2[left] - '0' + carry;
        carry = res>9;
        res = carry ? res-10: res;
        out[l--] = res+'0';
    }
    if(type != 0) {
        while (olen != 0 && out[olen-1] == '0') {
            olen--;
        }
        if (olen == 0) {
            out[olen++] = '0';
        }
    }
    str fraction;
    fraction.a = carry;
    fraction.b = olen + 1;
    return fraction;
}

void positive_add(int type, char* str1, int len1, char* str2, int len2, char* out) {
    str dot1, dot2;
    dot1 = find_dot(str1, len1);
    dot2 = find_dot(str2, len2);
    int dot_pos1 = dot1.a;
    int dot_pos2 = dot2.a;
    len1 = dot1.b;
    len2 = dot2.b;
    str1 = dot1.c;
    str2 = dot2.c;
 
    int carry = 0;
    int olen_fraction = 0;
    if (dot_pos1 < len1 || dot_pos2 < len2) {
        char* fraction = out + (dot_pos1 > dot_pos2 ? dot_pos1+1 : dot_pos2+1);
        fraction[0] = '.';
        fraction++;
        int pos1 = dot_pos1 != len1 ? dot_pos1+1 : dot_pos1;
        int pos2 = dot_pos2 != len2 ? dot_pos2+1 : dot_pos2;
        str frac;
        frac = fraction_add(type, str1+pos1, len1-pos1,str2+pos2, len2-pos2, fraction);
        carry = frac.a;
        olen_fraction = frac.b;
    }

    str integer;
    integer = integer_add(str1, dot_pos1, str2, dot_pos2, out, carry);
    int shift = integer.a;
    int olen_integer = integer.b;

    if (shift > 0) {
        int olen = olen_integer + olen_fraction + 1;
        int l;
        for (l = olen_integer; l < olen; l++) {
            out[l] = out[l+shift];
        }
    }
    out[olen_fraction + olen_integer] = '\0';
}

/*str1-str2*/
str integer_minus(char* str1, int len1, char* str2, int len2,char* out, int borrow) {
    int res; 
    int l1 = len1-1, l2 = len2-1;
    int a,b;
    int olen = len1 > len2 ? len1 : len2;
    int l = olen-1;
    for (; l2 >= 0; l1--, l2--, l--) {
        a = str1[l1]-'0' - borrow;
        b = str2[l2] - '0';
        borrow = a < b ? 1 : 0;
        res = a + 10*borrow - b;
    
        out[l] = res+'0';
    }

    for (; l1 != >= 0; l1--, l--) {
        a = str1[l1]-'0' - borrow;
        borrow = a < 0 ? 1 : 0;
        res = a + 10*borrow;

        out[l] = res+'0';
    }

    int zeros = 0;
    for (l = 0; l < olen; l++) {
        if (out[l] != '0')
        {
            break;
        }
        zeros++;
    }
    if (zeros > 0) {
        if (zeros == olen)
        {
            olen = 1;
            out[0] = '0';
            zeros -= 1;
        }
        else {
            olen -= zeros;
            for (l = 0; l < olen; l++) {
                out[l] = out[l+zeros];
            }
        }
    }

    str integer;
    integer.a = zeros;
    integer.b = olen;
    return integer;
}


str fraction_minus(int type, char* str1, int len1, char* str2, int len2,char* out) {
    int borrow = 0;
    int left = len1 < len2 ? len1 : len2; 
    int right = len1 > len2 ? len1 : len2; 
    const char* longer = len1  > len2 ? str1 : str2;
    int olen = right;
    int l = olen-1;
    int a, b, res;

    if (longer == str2) {
        for (--left; --right != left;) {
            a = 0 - borrow;
            b = longer[right] - '0';
            borrow = a < b;
            res = a+10*borrow - b;
            out[l--] = res + '0';
        }
    }
    else {
        for (--left; --right != left;) {
            out[l--] = longer[right];
        }
    }

    for (; left >= 0; left--) {
        a = str1[left] - '0' - borrow;
        b = str2[left] - '0';
        borrow = a < b;
        res = a+10*borrow - b;
        out[l--] = res + '0';
    }

    if (type != 0) {
        l = olen-1;
        for (; l >= 0; l--) {
            if (out[l] != '0') {
                break;
            }
        }
        olen = l == -1 ? 1 : l+1;
    }
    str fraction;
    fraction.a = borrow;
    fraction.b = olen + 1;
    return fraction;
}

void positive_minus(int type, char* str1, int len1, char* str2, int len2, char* out) {
    str dot1, dot2;
    dot1 = find_dot(str1, len1);
    dot2 = find_dot(str2, len2);
    int dot_pos1 = dot1.a;
    int dot_pos2 = dot2.a;
    len1 = dot1.b;
    len2 = dot2.b;
    str1 = dot1.c;
    str2 = dot2.c;
    int borrow = 0;
    int olen_fraction = 0;
    if (dot_pos1 < len1 || dot_pos2 < len2) {
        char* fraction = out + dot_pos1;
        fraction[0] = '.';
        fraction++;
        int pos1 = dot_pos1 != len1 ? dot_pos1+1 : dot_pos1;
        int pos2 = dot_pos2 != len2 ? dot_pos2+1 : dot_pos2;
        str frac;
        frac = fraction_minus(type,str1+pos1, len1-pos1,str2+pos2, len2-pos2, fraction);
        borrow = frac.a;
        olen_fraction = frac.b;
    }

    str integer;
    integer = integer_minus(str1, dot_pos1, str2, dot_pos2, out, borrow);
    int shift = integer.a;
    int olen_integer = integer.b;

    if (shift > 0) {
        int olen = olen_integer + olen_fraction + 1;
        int l;
        for (l = olen_integer; l < olen; l++) {
            out[l] = out[l+shift];
        }
    }
    out[olen_fraction + olen_integer] = '\0';
}


int find_larger(char* str1, int len1, char* str2, int len2) {
    str dot1, dot2;
    dot1 = find_dot(str1, len1);
    dot2 = find_dot(str2, len2);
    int dot_pos1 = dot1.a;
    int dot_pos2 = dot2.a;
    len1 = dot1.b;
    len2 = dot2.b;
    str1 = dot1.c;
    str2 = dot2.c;

    if (dot_pos1 > dot_pos2) {
        return 1;
    }
    if (dot_pos1 < dot_pos2) {
        return -1;
    }

    size_t len;
    for (len = 0; len != dot_pos1; len++) {
        if (str1[len] != str2[len]) {
            if (str1[len] > str2[len]) {
                return 1;
            }
            return -1;
        }
    }

    dot_pos1++, dot_pos2++;
    for (; dot_pos1 < len1 && dot_pos2 < len2; dot_pos1++, dot_pos2++) {
        if (str1[dot_pos1] != str2[dot_pos2]) {
            if (str1[dot_pos1] > str2[dot_pos2]) {
                return 1;
            }
    
            return -1;
        }
    }
    
    if (len1 < len2) {
        return -1;
    }
    if (len1 > len2) {
        return 1;
    }

    return 0;
}

void str_minus(int type, char* str1, int len1, char* str2, int len2, char* out) {
    int cmp_res;
    if (*str1 == '-' && *str2 == '-') {
        cmp_res = find_larger(str1+1, len1-1, str2+1, len2-1);
        switch(cmp_res){
            case 1: {
                positive_minus(type, str1+1, len1-1, str2+1, len2-1, out);
                append(out);
                break;
            }
            default: {
                positive_minus(type, str2+1, len2-1, str1+1, len1-1, out);
                break;
            }
        }
        return;
    }

    if (*str1 != '-' && *str2 != '-') {
        cmp_res = find_larger(str1, len1, str2, len2);
        switch(cmp_res){
            case -1: {
                positive_minus(type, str2, len2, str1, len1, out);
                append(out);
                break;
            }
            default: {
                positive_minus(type, str1, len1, str2, len2, out);
                break;
            }
        }
        return;
    }

    if (*str1 == '-' && *str2 != '-') {
        positive_add(type, str1+1, len1-1, str2, len2, out);
        append(out);
        return;
    }

    if (*str1 != '-' && *str2 == '-') {
        positive_add(type, str1, len1, str2+1, len2-1, out);
        return;
    }
}


void str_add(int type, char* str1, int len1, char* str2, int len2, char* out) {
    int cmp_res;
    if (*str1 == '-' && *str2 != '-') {
        cmp_res = find_larger(str1+1, len1-1, str2, len2);
        switch(cmp_res){
            case -1: {
                positive_minus(type, str2, len2, str1+1, len1-1, out);
                break;
            }
            case 1: {
                positive_minus(type, str1+1, len1-1, str2, len2, out);
                append(out);
                break;
            }
            case 0: {
                positive_minus(type, str1+1, len1-1, str2, len2, out);
                break;
            }
        }
        return;
    }

    if (*str1 != '-' && *str2 == '-') {
        cmp_res = find_larger(str1, len1, str2+1, len2-1);
        switch(cmp_res){
            case -1: {
                positive_minus(type, str2+1, len2-1, str1, len1, out);
                append(out);
                break;
            }
            default: {
                positive_minus(type, str1, len1, str2+1, len2-1, out);
                break;
            }
        }
        return;
    }

    if (*str1 == '-' && *str2 == '-') {
        positive_add(type, str1+1, len1-1, str2+1, len2-1, out);
        append(out);
        return;
    }

    if (*str1 != '-' && *str2 != '-') {
        positive_add(type, str1, len1, str2, len2, out);
        return;
    }
}

int str_cmp(char *str1, size_t len1, char *str2, size_t len2) {
    int res;
    if (*str1 == '-' && *str2 != '-') {    
        return -1;
    }

    if (*str1 != '-' && *str2 == '-') {
       return 1;
    }

    if (*str1 == '-' && *str2 == '-') {
        res = find_larger(str1+1, len1-1, str2+1, len2-1);
        switch(res) {
            case -1: return 1;
            case 0: return 0;
            case 1: return -1;
        }
    }

    if (*str1 != '-' && *str2 != '-') {
        res = find_larger(str1, len1, str2, len2);
        switch(res) {
            case -1: return -1;
            case 0: return 0;
            case 1: return 1;
        }
    }
}

int main(){
    char out[100] = {0};
    char num1[100];
    char num2[100]; 
    int type;
    printf("Please put the first number：");
    scanf("%s",num1);
    printf("Please put the second number：");
    scanf("%s",num2);
    printf("Please put the type：");
    scanf("%d",&type);

    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);
    str_add(type, num1, len1, num2, len2, out);
    printf("The result is：");
    puts(s);
    str_minus(type, num1, len1, num2, len2, out);
    printf("The result is：");
    puts(s);
    int m = str_cmp(num1, len1, num2, len2);
    printf("The compare result is %d\n",m);
}
