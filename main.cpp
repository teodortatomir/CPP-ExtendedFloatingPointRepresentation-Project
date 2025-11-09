// Code created by Teodor Tatomir on 11/09/2025!


#include <iostream>

using namespace std;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define MAX_LENGTH_INPUT 310
#define MAX_LENGTH_OUTPUT 1024
#define MAX_BITS_EXPONENT 11
#define mantissa32Bit 119
#define mantissa64Bit 1012


void cannotRepresentNumber() {
  cout<<"The number given can't be represented!"<<endl;
}

void printTheMenu() {
  cout<<"Please input a rational number between [-10^310, 10^310]!"<<endl;
}

void printInfo32Bit() {
  cout<<endl<<ANSI_COLOR_RED<<"We have one bit to represent the sign ( 0 for positive, 1 for negative ) !"<<endl;

  cout<<endl<<ANSI_COLOR_GREEN<<"We have 8 bits that represent the exponent in Excess-127 !"<<endl;

  cout<<endl<<ANSI_COLOR_BLUE<<"We have 119 bits that represent the Mantissa !"<<endl;
  cout<<ANSI_COLOR_RESET;

}

void printInfo64Bit() {
  cout<<endl<<ANSI_COLOR_RED<<"We have one bit to represent the sign ( 0 for positive, 1 for negative ) !"<<endl;

  cout<<endl<<ANSI_COLOR_GREEN<<"We have 11 bits that represent the exponent in Excess-1023 !"<<endl;

  cout<<endl<<ANSI_COLOR_BLUE<<"We have 1012 bits that represent the Mantissa !"<<endl;
  cout<<ANSI_COLOR_RESET;
}

short int integerPart[MAX_LENGTH_INPUT + 1];
short int fractionalPart[MAX_LENGTH_INPUT + 1];
short int periodFractional[MAX_LENGTH_INPUT + 1];
bool fixedPointNotationInteger[MAX_LENGTH_OUTPUT + 1];
bool fixedPointNotationFractional[MAX_LENGTH_OUTPUT + 1];
bool bitsForExponent[MAX_BITS_EXPONENT];
int main(){
  int signBit = 0;
  int lengthIntegerPart = 0;
  int lengthFractionalPart = 0;
  int lengthPeriod = 0;
  int lengthFixPointNotationInteger = 0;
  int lengthFixPointNotationFraction = 0;
  int pozPeriod = 0;
  int period = 0;
  int sumOfLengths = 0;
  int alreadyOutputed = 0;
  int index = 0;
  int totalBitsExponent = 0;
  int left, right, aux, t, finishFlag, pozition;
  char ch;

  printTheMenu();

  cin.get(ch);
  if(ch == '-') {  // sign bit verification
    signBit = 1;
    cin.get(ch);
  }

  lengthIntegerPart = 0;
  while(lengthIntegerPart < MAX_LENGTH_INPUT && ch != '.' && ch != '\n') {
    integerPart[lengthIntegerPart] = ch - '0';
    lengthIntegerPart++;
    cin.get(ch);
  }
  if(lengthIntegerPart == MAX_LENGTH_INPUT)
    cannotRepresentNumber();
  else {
    left = 0;
    right = lengthIntegerPart - 1;
    while(left < right) {
      aux = integerPart[left];
      integerPart[left] = integerPart[right];
      integerPart[right] = aux;
      left++;
      right--;
    }
    while(lengthIntegerPart > 0 && lengthFixPointNotationInteger < MAX_LENGTH_OUTPUT) {
      t = 0;
      for(int i = lengthIntegerPart - 1; i >= 0; i--) {
        t = t * 10 + integerPart[i];
        integerPart[i] = t / 2;
        t %= 2;
      }
      while(lengthIntegerPart > 0 && integerPart[lengthIntegerPart - 1] == 0)
        lengthIntegerPart--;
      fixedPointNotationInteger[lengthFixPointNotationInteger++] = t;
    }

    left = 0;
    right = lengthFixPointNotationInteger - 1;
    while(left < right) {
      aux = fixedPointNotationInteger[left];
      fixedPointNotationInteger[left] = fixedPointNotationInteger[right];
      fixedPointNotationInteger[right] = aux;
      left++;
      right--;
    }

    if(ch == '.') {
      cin.get(ch);
      lengthFractionalPart = 0;
      lengthPeriod = 0;
      while(ch != '\n' && ch != ')' && ch != EOF) {
        if(ch == '(') {
          period = 1;
          cin.get(ch);
        }
        else if(period == 1) {
          periodFractional[lengthPeriod] = ch - '0';
          lengthPeriod++;
          cin.get(ch);
        }
        else {
          fractionalPart[lengthFractionalPart] = ch - '0';
          lengthFractionalPart++;
          cin.get(ch);
        }
      }

      if(period == 1) {
        pozPeriod = 0;
        for(int i = lengthFractionalPart; i < MAX_LENGTH_INPUT; i++) {
          fractionalPart[i] = periodFractional[pozPeriod];
          pozPeriod = (pozPeriod + 1) % lengthPeriod;
        }
        lengthFractionalPart = MAX_LENGTH_INPUT;
      }

      left = 0;
      right = lengthFractionalPart - 1;
      while(left < right) {
        aux = fractionalPart[left];
        fractionalPart[left] = fractionalPart[right];
        fractionalPart[right] = aux;
        left++;
        right--;
      }
      finishFlag = 0;
      while((!finishFlag) && lengthFixPointNotationFraction < MAX_LENGTH_OUTPUT) {
        t = pozition = 0;

        while(pozition < lengthFractionalPart || t > 0) {
          t = t + 2 * fractionalPart[pozition];
          fractionalPart[pozition] = t % 10;
          t /= 10;
          pozition++;
        }
        if(pozition > lengthFractionalPart) {
          fixedPointNotationFractional[lengthFixPointNotationFraction++] = 1;
          fractionalPart[lengthFractionalPart] = 0;
          int i = 0;
          while(i < lengthFractionalPart && fractionalPart[i] == 0)
            i++;
          if(i == lengthFractionalPart)
            finishFlag = 1;
        }
        else
          fixedPointNotationFractional[lengthFixPointNotationFraction++] = 0;
      }
    }
    cout<<ANSI_COLOR_RED<<signBit;
    cout<<" ";
    sumOfLengths = lengthFixPointNotationInteger + lengthFixPointNotationFraction - 1;

    if(sumOfLengths <= 119) {

      if(lengthFixPointNotationInteger == 1 && fixedPointNotationInteger[0] == 0) {
        index = 0;
        while(index < lengthFixPointNotationFraction && fixedPointNotationFractional[index] == 0)
          index++;
        totalBitsExponent = 127 - index;
      }
      else {
        totalBitsExponent = lengthFixPointNotationInteger - 1;
        totalBitsExponent += 127;
      }

      int pow = 128;
      for(int i = 0; i < 8; i++) {
        if(pow <= totalBitsExponent){
          totalBitsExponent -= pow;
          bitsForExponent[i] = 1;
        }
        else
          bitsForExponent[i] = 0;
        pow /= 2;
      }

      for(int i = 0; i < 8; i++)
        cout<<ANSI_COLOR_GREEN<<bitsForExponent[i];
      cout<<" ";
      for(int i = 1; i < lengthFixPointNotationInteger; i++)
        cout<<ANSI_COLOR_BLUE<<fixedPointNotationInteger[i];
      for(int i = 0; i < lengthFixPointNotationFraction; i++)
        cout<<ANSI_COLOR_BLUE<<fixedPointNotationFractional[i];
      for(int i = 0; i < 119 - sumOfLengths; i++)
        cout<<ANSI_COLOR_BLUE<<"0";
      cout<<ANSI_COLOR_RESET<<'\n';
      printInfo32Bit();
    }
    else {
      if(lengthFixPointNotationInteger == 1 && fixedPointNotationInteger[0] == 0) {
        index = 0;
        while(index < lengthFixPointNotationFraction && fixedPointNotationFractional[index] == 0)
          index++;
        totalBitsExponent = 1023 - index;
      }
      else {
        totalBitsExponent = lengthFixPointNotationInteger - 1;
        totalBitsExponent += 1023;
      }

      int pow = 1024;
      for(int i = 0; i < 11; i++) {
        if(pow <= totalBitsExponent){
          totalBitsExponent -= pow;
          bitsForExponent[i] = 1;
        }
        else
          bitsForExponent[i] = 0;
        pow /= 2;
      }
      for(int i = 0; i < 11; i++)
        cout<<ANSI_COLOR_GREEN<<bitsForExponent[i];
      cout<<" ";

      for(int i = 1; i < lengthFixPointNotationInteger; i++) {
        if(alreadyOutputed < mantissa64Bit){
          cout<<ANSI_COLOR_BLUE<<fixedPointNotationInteger[i];
          alreadyOutputed++;
        }
      }

      for(int i = 0; i < lengthFixPointNotationFraction; i++) {
        if(alreadyOutputed < mantissa64Bit){
          cout<<ANSI_COLOR_BLUE<<fixedPointNotationFractional[i];
          alreadyOutputed++;
        }
      }
      for(int i = 0; i < mantissa64Bit - alreadyOutputed; i++)
        cout<<ANSI_COLOR_BLUE<<"0";
      cout<<ANSI_COLOR_RESET<<'\n';

      printInfo64Bit();
    }
  }
  return 0;
}
