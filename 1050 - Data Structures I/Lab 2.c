#include <stdio.h>

int main( void )
{
    int A; /* first integer */
    int B; /* second integer */
    int C; /* third integer */
    int D; /* fourth integer */

    printf( "Enter 4 integers (A,B,C,D) separated by spaces: " ); /* prompt for user */
    scanf( "%d%d%d%d", &A, &B, &C, &D ); /* gathering input from keyboard */

    printf( "*** Initial Values ***\n" );

    printf( "A = %d\n", A ); /* printing values */
    printf( "B = %d\n", B );
    printf( "C = %d\n", C );
    printf( "D = %d\n", D );

    printf( "*** Calculated Values ***\n" );

    int A_plus_B; /* declaring variable */
    A_plus_B = A + B; /* assigns total to A_plus_B */
    printf( "The sum of A and B is %d\n", A_plus_B ); /* printing result of A plus B */

    int C_x_D; 
    C_x_D = C * D; 
    printf( "The product of C and D is %d\n", C_x_D ); 

    int A_divi_C; 
    A_divi_C = A / C; 
    printf( "The integer quotient of A divided by C is %d\n", A_divi_C ); 

    int A_rem_C;
    A_rem_C = A % C;
    printf( "The integer remainder of A divided by C is %d\n", A_rem_C );

    int AC_BD;
    AC_BD = (A * C) / (B + D);
    printf( "The value of the quantity A times C divided by the quantity B plus D is %d\n", AC_BD );


    return 0;

}



