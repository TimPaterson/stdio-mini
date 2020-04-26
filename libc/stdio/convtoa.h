/*
 * convtoa.h
 *
 * Created: 4/25/2020 11:15:35 AM
 *  Author: Tim
 */ 


#ifndef CONVTOA_H_
#define CONVTOA_H_

 int __ftoa(float val, char *buf, int prec, int maxdgs);

// IEEE double
#define DOUBLE_EXP_BITS			11
#define DOUBLE_MANTISSA_BITS	52
#define DOUBLE_EXP_BIAS			1023

typedef union 
{
	double		d;
	long long	ll;
	struct  
	{
		long long	mant:DOUBLE_MANTISSA_BITS;
		long long	exp:DOUBLE_EXP_BITS;
		long long	sign:1;
	} bits;
} double_struct;



#endif /* CONVTOA_H_ */