/*
 * convtoa.h
 *
 * Created: 4/25/2020 11:15:35 AM
 *  Author: Tim
 */ 


#ifndef CONVTOA_H_
#define CONVTOA_H_

 int __ftoa(float val, char *buf, int prec, int maxdgs);

/* '__ftoa_engine' return next flags (in buf[0]):	*/
#define	FTOA_MINUS	1
#define	FTOA_ZERO	2
#define	FTOA_INF	4
#define	FTOA_NAN	8
#define	FTOA_CARRY	16	/* Carry was to master position.	*/

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