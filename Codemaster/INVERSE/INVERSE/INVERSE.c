//#include <stdio.h>
short signed int inverse (int a);



 // Процедура, обрабатывающая знаковый бит
short signed int inverse (int a)
{
int indicator=0x2000;
int add=0xE000;
int res=0;
 if ((a&indicator)!=0  )
 {
  res=~(a|add)+1;
 return res*(-1);
 }
 else
 return a;
}

