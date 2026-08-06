/* test_pure.c - TRDOS 386 Native Toolchain Standalone PP Test */

#define MAX_VAL 100
#define ADD_FIVE(x) ((x) + 5)

int main()
{
    int a;
    int b;
    
    a = MAX_VAL;
    b = ADD_FIVE(a);
    
    if (b > 10) {
        a = 0;
    } else {
        a = 1;
    }
    
    return a;
}
