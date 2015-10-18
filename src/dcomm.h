#ifndef _DCOMM_H_
#define _DCOMM_H_
/* ASCII Const */
#define SOH 1 /* Start of Header Character */
#define STX 2 /* Start of Text Character */
#define ETX 3 /* End of Text Character */
#define ENQ 5 /* Enquiry Character */
#define ACK 6 /* Acknowledgement */
#define BEL 7 /* Message Error Warning */
#define CR 13 /* Carriage Return */
#define LF 10 /* Line Feed */
#define NAK 21 /* Negative Acknowledgement */
#define Endfile 26 /* End of file character */
#define ESC 27 /* ESC key */

/* XON/XOFF protocol */
#define XON (0x11)
#define XOFF (0x13)

/* SLIDING WINDOW Properties*/
#define WINDOW_SIZE 5
#define bufsize 4096

/* SENDER Properties */
#define ACKTIMEOUTSECONDS 0.100
#define PREFERREDFRAMEDATASIZE 5


#endif
