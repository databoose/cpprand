#include <fstream>
#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <string>       
#include <string.h>
#include <vector>
bool exitt = false;

/*
   Note: If random numbers are generated with rand() without first calling srand(), your program will create the same sequence of numbers each time it runs.

   Tested, true
*/

template<typename T>
void cint(T* x) {
   std::cout << ">";
   std::cin >> *x;
   std::cout << "\n";
}

void list_shuffle() {
   std::string list_input;
   std::cout << "Type the list of items, seperated by commas" << std::endl;
   std::cout << "Example : Apple,Bannana,Orange" << std::endl;
   cint(&list_input);
   int len = list_input.length();
   unsigned int arrsize;
   for (int i=0; i < len; i++) {
      if (list_input[i] == ',') {
         arrsize++;
      }
      if (list_input[i] != ',') {

      }
   }
   std::cout << arrsize << std::endl;
}

void lobby() {
   int input;
   
   std::cout << "\n";
   std::cout << "1 : Coinflip" << std::endl;
   std::cout << "2 : Range Randomization" << std::endl;
   std::cout << "3 : Password Generator" << std::endl;
   std::cout << "4 : List Shuffler" << std::endl;
   std::cout << "5 : Exit Program" << std::endl;
   std::cout << "\n";

   cint(&input);
   switch (input) {
      case 1:
         std::cout << "Result : "  << rand()%2+1 << std::endl;
         break;
      case 2: {
         int max;
         std::cout << "Max number : ";
         cint(&max);
         std::cout << "\n";
         std::cout << "Result : " << rand()%max + 1;
         std::cout << "\n";
         break;
      }
      case 3: { // explicit isolated block
         int n;
         std::cout << "Enter length of password : ";
         cint(&n);
         const int MAX = 77;
         char ctable [MAX] = {
            'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', 
            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','O','R','S','T','U','V','W','X','Y','Z',
            '!','@','#','$','%','^','&','*','(',')','-','_','+','=',';',':','1','2','3','4','5','6','7','8','9'
         };
         std::string res = "";
         for (int i = 0; i < n; i++) {
            res = res + ctable[rand() % MAX];
         }
         std::cout << res << std::endl;
         break;
      }
      case 4:
         list_shuffle();
         break;
      case 5:
         exitt = true;
         break;
      default:
         std::cout << "Incorrect input" << std::endl;
         exitt = true; // prevents looping from broken std::cin
         break;
   }
}

unsigned long int get_bytes_kernel() {
   unsigned long int random_value = 0; // 8 bytes
   size_t size = sizeof(random_value);

   std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary); //Open stream
   if (urandom) {// if stream is open
      urandom.read(reinterpret_cast<char*>(&random_value), size); //Read from urandom
      urandom.close();
   }
   else {
        std::cerr << "Failed to open /dev/urandom" << std::endl;
   }
   return random_value;
}

int main() {
   unsigned long int kseed = get_bytes_kernel();
   srand(kseed);
   std::cout << "krand : " << kseed << std::endl;
   while (exitt == false) {
      lobby();
   }
   
   //int random = rand() % 10 + 1; // between 1 and 10, +1 for starting from zero
   //std::cout << random << std::endl;
   
   return 0;
}
