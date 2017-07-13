/* 
 * loancalc.c - Loan Calculator
 *
 * Brett Lee <brett@brettlee.com>
 */

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

main( int argc, char *argv[] ) { 

  char answer, filename;
  FILE *cfptr, *fstdout;

  /*
  printf("\nARGC: %d \n", argc);
  printf("ARGV0: %s \n", argv[0]);
  printf("ARGV1: %s \n", argv[1]);
  printf("ARGV2: %s \n", argv[2]);
  */

  if (( argc != 1 ) && ( argc != 3 ))
  {
    printf("\nUsage: %s [ -f <filename> ]\n", argv[0]);
    return 0;
  }

  if ( argv[1] == NULL )
  { 
    fstdout = stdout;
  } 
  else if ( strcmp (argv[1], "-f" ))
  { 
    printf("\nUsage: %s [ -f <filename> ]\n", argv[0]);
    return 0;

  } else { 

    if (( cfptr = fopen( argv[2], "w" )) == NULL ) 
    {
      printf("\nCould not open file: %s \n", argv[2]);
      return 0;
    }
    fstdout = cfptr;
  }

  system ("tput clear");

  printf("                                                                   \n\
                                                                             \n\
Amortization Schedules                                                       \n\
-------------------------------------------                                  \n\
                                                                             \n\
Which of the following loans would you like to consider?                     \n\
                                                                             \n\
 (N) Non-Amortized         ::  Pay Interest Only - Big Balloon Due at Term   \n\
 (P) Partially Amortized   ::  Pay Some of the Principle - Balloon Due at Term \n\
 (A) Fully Amortized       ::  Pay All of the Principle and Interest         \n\
 (E) Fully Amortized +     ::  Make Extra Payments Toward the Principle      \n\
                                                                             \n\
 (Q) Quit                                                                    \n\
                                                                             \n\
Please make your selection: ");

  scanf("%c", &answer);

  switch ( answer ) {

    case 'N': case 'n':
      system ("tput clear");
      do_nonamort( fstdout );
      break;

    case 'P': case 'p':
      system ("tput clear");
      do_partialamort( fstdout );
      break;

    case 'A': case 'a':
      system ("tput clear");
      do_amort( fstdout );
      break;

    case 'E': case 'e':
      system ("tput clear");
      do_amort_extra( fstdout );
      break;

    default:
      break;
  }

} /* End of Main */

/* -------------------------------------------------------------- */
/* -------------------------------------------------------------- */
/* -------------------------------------------------------------- */


double round (double dollars) 
{
   double pennies;

   pennies = floor (dollars * 100 + .5);
   return pennies / 100.;
}


int do_amort( FILE *fstdout )
{
   int n, years;
   double loanamt, intyr;
   double npmts, intmo, pmt, bal, intpmt, prinpmt;
   double principlepaid, interestpaid, totalpaid;

   double pow (double, double);

   printf("\nYou will need to enter the following:\n                   \n\
      Principal                                                        \n\
      Interest Rate                                                    \n\
      Term of the Loan\n\n");

   printf("Enter principal (e.g. 119000) : ");
   scanf("%lf", &loanamt);
   printf("Enter annual interest rate (e.g. 8.375) : ");
   scanf("%lf", &intyr);
   printf("Enter number of years (e.g. 30): ");
   scanf("%d", &years);

   fprintf(fstdout, "principal = %7.2f  interest = %6.4f%%  years = %d \n",
	loanamt, intyr, years);
   fprintf(fstdout, "                                                  \n\
   Payment     Total   Interest  Principal       Balance               \n\
   Number    Payment    Portion    Portion                             \n\
   ------------------------------------------------------\n");
   fprintf(fstdout, " %55.2f\n", loanamt);

   npmts = years * 12.0;
   intmo = intyr / 12.0 / 100;
   pmt = loanamt * intmo / (1 - (pow (1 + intmo, -1 * npmts)));
   pmt = round (pmt);
   bal = loanamt;

   principlepaid = 0;
   interestpaid  = 0;
   totalpaid     = 0;

   for (n = 1; n <= npmts; n++) {
     intpmt = round (bal * intmo);
     if (n < npmts)
       prinpmt = pmt - intpmt;
     else
       prinpmt = bal;
     bal -= prinpmt;
     fprintf(fstdout, "%8d %11.2f %10.2f %10.2f %13.2f\n",
       n, intpmt + prinpmt, intpmt, prinpmt, bal);

     principlepaid += prinpmt;
     interestpaid  += intpmt;
     totalpaid     += (prinpmt + intpmt);
   }

   fprintf(fstdout, "                                                  \n\
   Payment     Total   Interest  Principal       Balance               \n\
   Number    Payment    Portion    Portion                             \n\
   -----------------------------------------------------\n\n");

   fprintf(fstdout, "Terms of the Loan:\n");
   fprintf(fstdout, "------------------\n");
   fprintf(fstdout, 
     "\tPrincipal: %10.2f \n\tInterest:  %10.3f%% \n\tYears: %14d \n\n",
     loanamt, intyr, years);

   fprintf(fstdout, "Truth in Lending:\n");
   fprintf(fstdout, "-----------------\n");
   fprintf(fstdout, 
     "\tPrinciple: %10.2f \n\tInterest:  %10.2f \n\tTotal:     %10.2f \n",
     principlepaid, interestpaid, totalpaid);

   return 1;
}


int do_amort_extra( FILE *fstdout )
{
   int n, years;
   double loanamt, intyr, extrapmt;
   double npmts, intmo, pmt, bal, intpmt, prinpmt;
   double principlepaid, interestpaid, totalpaid;

   double pow (double, double);

   printf("\nYou will need to enter the following:                     \n\n\
      Principal                                                        \n\
      Interest Rate                                                    \n\
      Term of the Loan                                                 \n\
      Any extra payments                                               \n\n");

   printf("Enter principal (e.g. 119000) : ");
   scanf("%lf", &loanamt);
   printf("Enter annual interest rate (e.g. 8.375) : ");
   scanf("%lf", &intyr);
   printf("Enter number of years (e.g. 30): ");
   scanf("%d", &years);
   printf("Extra payment each month: (e.g. 200): ");
   scanf("%lf", &extrapmt);

   fprintf(fstdout, "principal = %7.2f  interest = %6.4f%%  years = %d \n",
	loanamt, intyr, years);

   fprintf(fstdout, "                                                        \n\
-----------------------------------------------------------------------------\n\
Payment  Required     Extra      Total    Interest    Principal      Balance \n\
Number    Payment   Payment    Payment     Portion      Portion              \n\
-----------------------------------------------------------------------------\n\n");

   fprintf(fstdout, " %75.2f\n", loanamt);

   npmts = years * 12.0;
   intmo = intyr / 12.0 / 100;
   pmt = loanamt * intmo / (1 - (pow (1 + intmo, -1 * npmts)));
   pmt = round (pmt);
   bal = loanamt;
   n = 1;
   principlepaid = 0;
   interestpaid  = 0;
   totalpaid     = 0;

   while ( bal > 0 ) {
     intpmt = round (bal * intmo);

     if (pmt > (bal + intpmt)) {
       pmt = (bal + intpmt);
       extrapmt = 0;
       prinpmt = bal;
     }
     else if ((pmt + extrapmt) > (bal + intpmt)) {
       extrapmt = (bal + intpmt - pmt);
       prinpmt = bal;
     }
     else
       prinpmt = (pmt + extrapmt - intpmt);

     bal -= prinpmt;

     fprintf(fstdout, "%4d %12.2f %9.2f %10.2f %11.2f %12.2f %12.2f\n",
       n, pmt, extrapmt, pmt + extrapmt, intpmt, prinpmt, bal);

     principlepaid += (prinpmt);
     interestpaid  += intpmt;
     totalpaid     += (prinpmt + intpmt);
     n++;
   }

   fprintf(fstdout, "                                                        \n\
-----------------------------------------------------------------------------\n\
Payment  Required     Extra      Total    Interest    Principal      Balance \n\
Number    Payment   Payment    Payment     Portion      Portion              \n\
-----------------------------------------------------------------------------\n\n");


   fprintf(fstdout, "Terms of the Loan:\n");
   fprintf(fstdout, "------------------\n");
   fprintf(fstdout, 
     "\tPrincipal: %10.2f \n\tInterest:  %10.3f%% \n\tYears: %14d \n\n",
     loanamt, intyr, years);

   fprintf(fstdout, "Truth in Lending:\n");
   fprintf(fstdout, "-----------------\n");
   fprintf(fstdout, 
     "\tPrinciple: %10.2f \n\tInterest:  %10.2f \n\tTotal:     %10.2f \n",
     principlepaid, interestpaid, totalpaid);

   return 1;
}


int do_nonamort( FILE *fstdout )
{
   int n, years;
   double loanamt, intyr;
   double npmts, intmo, pmt, bal, intpmt, prinpmt;
   double principlepaid, interestpaid, totalpaid;

   double pow (double, double);

   printf("\nYou will need to enter the following:\n                   \n\
     Principal                                                         \n\
     Interest Rate\n\n");

   printf("Enter principal (e.g. 119000) : ");
   scanf("%lf", &loanamt);
   printf("Enter annual interest rate (e.g. 8.375) : ");
   scanf("%lf", &intyr);
   fprintf(fstdout, "principal = %7.2f  interest = %6.4f%%\n",
     loanamt, intyr);
   fprintf(fstdout, "                                                     \n\
                                                                          \n\
   Non-Amoritized Calculations                                            \n\
   ------------------------------------------------------\n");
   intmo = intyr / 12.0 / 100;
   pmt = loanamt * intmo;
   pmt = round (pmt);
   bal = loanamt;

   intpmt = round (bal * intmo);
   prinpmt = pmt - intpmt;
   bal -= prinpmt;

   fprintf(fstdout, " \n\
   Loan Amount:      %35.2f\n \n\
   Monthly Payments: %35.2f\n \n\
   Principal Paid:   %35.2f\n \n\
   Balance Due:      %35.2f\n\n", loanamt, intpmt, prinpmt, bal);

   return 1;
}


int do_partialamort( FILE *fstdout )
{
   int n, years;
   double loanamt, intyr;
   double npmts, intmo, pmt, bal, intpmt, prinpmt;
   double principlepaid, interestpaid, totalpaid;

   double pow (double, double);

   printf("\nYou will need to enter the following:\n                      \n\
     Principal                                                            \n\
     Interest Rate                                                        \n\
     Term of the Loan                                                     \n\
     Payment Amount\n\n");

   printf("Enter the loan amount (e.g. 119000) : ");
   scanf("%lf", &loanamt);
   printf("Enter the annual interest rate as a percentage (e.g. 5.375) : ");
   scanf("%lf", &intyr);
   printf("Enter the term of the loan - in years (e.g. 30): ");
   scanf("%d", &years);
   printf("Enter the payment amount - per month (e.g. 1500): ");
   scanf("%lf", &pmt);
   fprintf(fstdout, 
     "principal = %7.2f  interest = %6.4f%%  years = %d  payment = %7.2f\n",
      loanamt, intyr, years, pmt);
   fprintf(fstdout, "                                                     \n\
   Payment     Total   Interest  Principal       Balance                  \n\
   Number    Payment    Portion    Portion                                \n\
   ------------------------------------------------------\n");
   fprintf(fstdout, " %55.2f\n", loanamt);

   npmts = years * 12.0;
   intmo = intyr / 12.0 / 100;
   bal = loanamt;

   principlepaid = 0;
   interestpaid  = 0;
   totalpaid     = 0;

   for (n = 1; n <= npmts; n++) {
     intpmt = round (bal * intmo);
     if (n < npmts)
       prinpmt = pmt - intpmt;
     else
       prinpmt = bal;
     bal -= prinpmt;
     fprintf(fstdout, "%8d %11.2f %10.2f %10.2f %13.2f\n",
       n, intpmt + prinpmt, intpmt, prinpmt, bal);

     principlepaid += prinpmt;
     interestpaid  += intpmt;
     totalpaid     += (prinpmt + intpmt);
   }

   fprintf(fstdout, "                                                     \n\
   Payment     Total   Interest  Principal       Balance                  \n\
   Number    Payment    Portion    Portion                                \n\
   -----------------------------------------------------\n\n");

   fprintf(fstdout, "Terms of the Loan:\n");
   fprintf(fstdout, "------------------\n");
   fprintf(fstdout, 
     "\tPrincipal: %10.2f \n\tInterest:  %10.3f%% \n\tYears: %14d \n\n",
     loanamt, intyr, years);

   fprintf(fstdout, "Truth in Lending:\n");
   fprintf(fstdout, "-----------------\n");
   fprintf(fstdout, 
     "\tPrinciple: %10.2f \n\tInterest:  %10.2f \n\tTotal:     %10.2f \n",
     principlepaid, interestpaid, totalpaid);

   return 1;
}
