/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#include <printf.h>

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif

int logicalShift(int x, int n);

void printBit(int x) {

  int i;
  unsigned int bit = 0x80000000;
  for (i = 0; i < sizeof(int) * 8; i++) {
    if (x & bit) {
      printf("1");
    } else {
      printf("0");
    }
    bit = bit >> 1;
  }
  printf("  ");
}

/*
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~(~x | ~y);
}

/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  return (x >> (n << 3)) & 0xFF;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  //printf("the x is %d, n is %d \n",x,n);
  int completeN = (~n + 1) + 0x20;
  int is32 = completeN >> 5;
  int extractor = (0x1 & ~is32) << completeN;
  int arithmeticShift = x >> n;
  int sign = extractor & arithmeticShift;
  int result = arithmeticShift + sign;
  //printf("the completeN is %d, extractor is %d. sign is %d, arithmeticShift is %d,result is %d \n",completeN,extractor,sign,arithmeticShift,result);
  return result;
}

/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  int all5 = 0x55 + (0x55 << 8) + (0x55 << 16) + (0x55 << 24);
  int all3 = 0x33 + (0x33 << 8) + (0x33 << 16) + (0x33 << 24);
  int all0f = 0x0F + (0x0F << 8) + (0x0F << 16) + (0x0F << 24);
  int all00ff = 0xFF + (0xFF << 16);
  int ffff = 0xFF + (0xFF << 8);
  x = (x & all5) + ((x >> 1) & all5);
  x = (x & all3) + ((x >> 2) & all3);
  x = (x & all0f) + ((x >> 4) & all0f);
  x = (x & all00ff) + ((x >> 8) & all00ff);
  x = (x & ffff) + ((x >> 16) & ffff);
  return x;
}

/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  x = (x >> 16) | x;
  x = (x >> 8) | x;
  x = (x >> 4) | x;
  x = (x >> 2) | x;
  x = (x >> 1) | x;
  return (~x & 0x1);
}

/*
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return (0x1 << 0x1F);
}

/*
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int completeN = 33 + ~n;
  int maskX = (x << completeN) >> completeN;
  int result = !(maskX ^ x);
  return result;
}

/*
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  int sign = (x >> 31) & 0x1;
  int shift = x >> n;
  int remainder = ~(!((shift << n) ^ x));
  int result = shift + (sign & remainder);
  return result;
}

/*
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x + 1);
}

/*
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  return (~((x >> 31) | !x)) & 0x1;
}

/*
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int a = ~(~x & y) >> 31;
  int b = (x & ~y) >> 31;
  int negativeX = ~x + 1;
  int sub = y + negativeX;
  int result = ((~(sub >> 31)) & a | b) & 0x1;
  return result;
}

/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  int tmp = x >> 16;
  int flg1 = (~!tmp << 31) >> 31;
  x = (flg1 & tmp) | (~flg1 & x);
  tmp = x >> 8;
  int flg2 = (~!tmp << 31) >> 31;
  x = (flg2 & tmp) | (~flg2 & x);
  tmp = x >> 4;
  int flg3 = (~!tmp << 31) >> 31;
  x = (flg3 & tmp) | (~flg3 & x);
  tmp = x >> 2;
  int flg4 = (~!tmp << 31) >> 31;
  x = (flg4 & tmp) | (~flg4 & x);
  tmp = x >> 1;
  int flg5 = (~!tmp << 31) >> 31;
  x = (flg5 & tmp) | (~flg5 & x);
  int result = ((0x1 & flg5) + (0x2 & flg4) + (0x4 & flg3) + (0x8 & flg2) + (0x10 & flg1));
  return result;
}

/*
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  int sign = (uf & 0x7FC00000) ^0x7FC00000;
  unsigned result;
  if (!sign) {
    result = uf;
  } else {
    result = (~uf & 0x80000000) + (uf & 0x7FFFFFFF);
  }
  return result;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  int sign = x & 0x80000000;
  int negative;
  if (sign) {
    negative = -1;
  } else {
    negative = 0;
  }
  unsigned absolute = ((~x + 1) & negative) | (~negative & x);
  unsigned tmp = absolute;
  int n = -1;
  while (tmp) {
    n += 1;
    tmp = tmp >> 1;
  }
  unsigned frac;
  int nOverFlow = n - 23;
  if (nOverFlow > 0) {
    frac = (absolute >> nOverFlow) & 0x007FFFFF;
    int shiftBits = 55 - n;
    unsigned shiftAbsolute = absolute << shiftBits;
    if ((shiftAbsolute & 0x80000000) && ((frac & 0x1) || (shiftAbsolute & 0x7FFFFFFF))) {
      frac = frac + 1;
    }
  } else {
    frac = (absolute << -nOverFlow) & 0x007FFFFF;
  }
  unsigned exp;
  if (!x) {
    exp = 0;
  } else {
    exp = (n + 127) << 23;
  }
  unsigned result = sign + exp + frac;
  return result;
}

/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  int sign = (uf & 0x7F800000) ^0x7F800000;
  unsigned result;
  if (!sign || !(uf & 0x7FFFFFFF)) {
    result = uf;
  } else if (!(uf & 0x7F800000)) {
    result = (uf & 0x80000000) + (uf << 1);
  } else {
    result = uf + (0x1 << 23);
  }
  return result;
}
