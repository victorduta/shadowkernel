
typedef enum enumeration{
      FIRST,
      SECOND,
      THIRD
} what_type;

int __attribute__ ((noinline)) long_function(int *v, int length, what_type type)
{
    int i, j, times, increment;
    increment = 0;
    switch(type)
    {
       case FIRST:
                     times = 3;
                     break;
       case SECOND:
                     increment = 4;
                     times = 10;
                     break;
       case THIRD:
                     increment = 6;
                     times = 2;
                     break;
    }
    j =0;
    while(j < times)
    {
     for(i = 0; i < length-1; i++)
     {
          if(v[i] < v[i+1])
          {
             v[i+1]= v[i+1]+v[i];
             v[i] = v[i+1] - v[i];
             v[i+1] = v[i+1] - v[i];
          }
     }
     j++;
    }
    return v[3]+increment;
}
int normal_function_s_complex(int y)
{
    int z = y+y;
    return 2*y+z;
}
int normal_function_not_complex(int y)
{
   return y-10;
}
static inline int inlinable_function(int y)
{
   return y+7;
}
int static_function(int y)
{
   int z;
   z = y-20;
   z = (y+21)*z;
   return z-9;
}

void tail_call_function()
{
    printf("tail_call_function\n");
}
void call_a_tail_call()
{
    tail_call_function();
}
static inline int just_inline_function(int y)
{
    int z = y*y;
    if(z < 10)
        return y+9;
    else
        return y;
}

void we_call_tail()
{
     int z = 2;
     call_a_tail_call();
     printf("wooop\n");
     return; 
}

void we_call_tail_40()
{
     int z = 2;
     call_a_tail_call();
     printf("wooop\n");
     return; 
}
/* Shows only the last tail call of a basic block will be translated */
void we_call_tail_1()
{
     call_a_tail_call();
     call_a_tail_call();
     return; 
}

/* Shows that tail calls are not translate in case function returns something */
int we_call_tail_2()
{
    call_a_tail_call();
    return 2;
}

void more_complex_tail_call(int x)
{
    if (x == 2)
    {
        call_a_tail_call();
    }
    call_a_tail_call();
    return;
}


void more_complex_tail_call_2(int x)
{
    if (x == 2)
    {
        call_a_tail_call();
        return;
    }
    call_a_tail_call();
    return;
}

int more_complex_tail_call_3(int x)
{
    if (x == 2)
    {
        call_a_tail_call();
        return 4;
    }
    call_a_tail_call();
    return 4;
}


int more_complex_tail_call_4(int x)
{
    if (x == 2)
    {
        call_a_tail_call();
        return 5;
    }
    call_a_tail_call();
    return 4;
}

/* Shows that a tail call will be translated to a jmpq if it is the last instruction of a basic block
   and the return basic block has no other calling function */
void more_complex_tail_call_5(int x)
{
    if (x == 2)
    {
        call_a_tail_call();
    }
    else
    {
        printf("smth\n");
    }
}
int v = 0;
void more_complex_tail_call_6(int x)
{
    if (x == 2)
    {
        call_a_tail_call();
    }
    else
    {
        printf("smth\n");
    }
    v = normal_function_not_complex(3);
}

void more_complex_tail_call_7(int x)
{
    if (x == 2)
    {
        call_a_tail_call();
    }
    else
    {
        v = normal_function_not_complex(3);
    }
   
}
extern int offset_test;
void test_alloca_var()
{
   int x;
   char c = 'a';
   volatile int *p = (int *)&c;

   p[0] = 45;
   printf("%c", c);
   printf("%d",*p);
}
extern unsigned int us;
void test_alloca_array()
{
   unsigned int v[100][200];
   unsigned int mor;
   v[2][3] = 0;
   memset(v, 2, us);

   for (int i = 0 ; i < 23; i++)
   {
      mor+= v[i][2];
   }
   if (mor != us)
   {
       return;
   }
   printf("%d", v[2][4]);
   
   if (v[2][5] != 3)
   {
      int mog[78];
      mog[4] = 7;
      printf("%d\n", mog[4]);
   }
}

int (*foo)(int) = normal_function_s_complex;
void (*mem[100])(int);
int main()
{
  int result;
  int start_value = 1000;
  //mem[99] = normal_function_not_complex;
  int (*foos)(int) = normal_function_s_complex;
  int ve[10] = {1 , 3, 4 , 9, 7 , 19, 3, 4, 5, 10};
  start_value = normal_function_not_complex(start_value);
  start_value = normal_function_s_complex(start_value);
  start_value = inlinable_function(start_value);
  start_value = just_inline_function(start_value);
  result = static_function(start_value);
  tail_call_function();
  call_a_tail_call();
  result = long_function((int *)ve, 10, SECOND);
  result++;
  printf("Result is %d\n", result);
  foo(5);
  mem[99](5);

  we_call_tail();
  we_call_tail_1();
  we_call_tail_2();
  more_complex_tail_call(2);
  more_complex_tail_call_2(3);
  more_complex_tail_call_3(4);
  more_complex_tail_call_4(5);
  more_complex_tail_call_5(6);
  more_complex_tail_call_6(7);
  more_complex_tail_call_7(8);
  we_call_tail_40();

  test_alloca_var();
  test_alloca_array();

  foo(5);
  //foos(6);

  return result;
}
