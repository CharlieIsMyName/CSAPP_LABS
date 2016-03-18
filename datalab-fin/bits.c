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
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~(~x|~y);
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
	int shift=n<<3;
	return 0xff&(((0xff<<shift)&x)>>shift);

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
  int mask=~((1<<31>>n)<<1);
  return (x>>n)&mask;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
	int result=x;
	int mask1=(0x55<<8)+0x55;
	int mask2=(0x33<<8)+0x33;
	int mask3=(0x0f<<8)+0x0f;
	int mask4=(0xff<<16)+0xff;		//mask4=0x00ff00ff
	int mask5=(0xff<<8)+0xff;		//mask5=0x0000ffff
	mask1=(mask1<<16)+mask1;			//mask1=0x55555555 01010101
	mask2=(mask2<<16)+mask2;			//mask2=0x33333333 00110011
	mask3=(mask3<<16)+mask3;			//mask3=0x0f0f0f0f 00001111

	//16 ops till now
	result=((result>>1)&mask1)+(result&mask1);
	result=((result>>2)&mask2)+(result&mask2);
	result=((result>>4)&mask3)+(result&mask3);
	result=((result>>8)&mask4)+(result&mask4);
	result=((result>>16)&mask5)+(result&mask5);
	return result&0x3f;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1o
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
	x|=x>>16;
	x|=x>>8;
	x|=x>>4;
	x|=x>>2;
	x|=x>>1;
	return (x&1)^1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1<<31;
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
	int mask=(((x&(1<<31))>>31)<<n)>>1;
	x&=((~0)<<n)>>1;
	return !(x^mask)|!(n^32);
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
	int sign=(x&(1<<31))>>31;
	x+=(~(sign<<n))&sign;
    return x>>n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x)+1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  return !(x&(1<<31))&(!!x);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
	int signmask=1<<31;
	int signx=(x&signmask)>>31;
	int signy=(y&signmask)>>31;
	int diff=x+~y+1;
	int diffSign=diff>>31;
	return (((!(signx^signy))& (diffSign|!(diff)))|((signx^0)&(signy^1)))&1;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {		//combining bang and bit count
	int mask1=(0x55<<8)+0x55;
	int mask2=(0x33<<8)+0x33;
	int mask3=(0x0f<<8)+0x0f;
	int mask4=(0xff<<16)+0xff;		//mask4=0x00ff00ff
	int mask5=(0xff<<8)+0xff;		//mask5=0x0000ffff
	int result=x;
	mask1=(mask1<<16)+mask1;			//mask1=0x55555555 01010101
	mask2=(mask2<<16)+mask2;			//mask2=0x33333333 00110011
	mask3=(mask3<<16)+mask3;			//mask3=0x0f0f0f0f 00001111
	result|=result>>16;
	result|=result>>8;
	result|=result>>4;
	result|=result>>2;
	result|=result>>1;
	//16 ops till now
	result=((result>>1)&mask1)+(result&mask1);
	result=((result>>2)&mask2)+(result&mask2);
	result=((result>>4)&mask3)+(result&mask3);
	result=((result>>8)&mask4)+(result&mask4);
	result=((result>>16)&mask5)+(result&mask5);
	return (result&0x3f)+(~0);
  return 2;
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
	unsigned result=uf+0x80000000;
	unsigned exp=uf&0x7f800000;
	unsigned frac=uf&0x007fffff;
	if((exp==0x7f800000)&&(frac!=0)){
		result=uf;
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
	unsigned mag;
	unsigned sign=0;
	unsigned exp=32;
	unsigned frac=0;
	unsigned mask=0x80000000;
	int round=0xffffffff;
	if(x<0){
		mag=-x;	//ignoring the case of tmin for now
		sign=0x80000000;
	}
	else{
		mag=x;
	}
	while(exp){
		if(mag&mask){
			break;
		}
		mask>>=1;
		exp--;
	}
	mag&=~(mask);
	exp--;				//now the first "1" is set to 0 and exp is the position of the next bit (8ops)
	if(exp==0xffffffff){
		return sign;
	}
	else if(exp==0){
		exp=0;
	}
	else if(exp<=23){
		unsigned shift=23-exp;
		frac=(mag<<shift);
	}
	else{
		unsigned shift=exp-23;
		round=~(round<<(shift-1))+((mag>>shift)&1);
		frac=((mag+round)>>shift);
		if(frac>0x7fffff){
			frac&=0x7fffff;
			frac>>=1;
			exp++;
		}
	}
	exp+=127;
	return (sign)|(exp<<23)|(frac);
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
	unsigned result;
	unsigned sign=uf&0x80000000;
	unsigned exp=uf&0x7f800000;
	unsigned frac=uf&0x007fffff;
	if(exp==0x7f800000){
		return uf;
	}
	if(exp==0){
		result=(sign|exp)+(frac<<1);
	}
	else if(exp>0&&exp<0x7f800000){
		result=uf+0x00800000;
	}
	else{
		result=sign|exp;
	}
	return result;
}
