/*
 * convtoa.h
 *
 * Created: 4/25/2020 11:15:35 AM
 *  Author: Tim
 */ 


#ifndef CONVTOA_H_
#define CONVTOA_H_

int __ftoa(float val, char *buf, int prec, int maxdgs);
int __dtoa(double val, char *buf, int prec, int maxdgs);

/* '__ftoa_engine' return next flags (in buf[0]):	*/
#define	FTOA_MINUS	1
#define	FTOA_ZERO	2
#define	FTOA_INF	4
#define	FTOA_NAN	8
#define	FTOA_CARRY	16	/* Carry was to master position.	*/


#endif /* CONVTOA_H_ */