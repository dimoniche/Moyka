/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/TCP-IP is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/TCP-IP in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to help
*               you experience uC/TCP-IP.  The fact that the source code is provided does
*               NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                         NETWORK ASCII LIBRARY
*
* Filename      : net_ascii.c
* Version       : V1.89
* Programmer(s) : ITJ
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    NET_ASCII_MODULE
#include  <net.h>


/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*$PAGE*/
/*
*********************************************************************************************************
*                                        NetASCII_Str_to_MAC()
*
* Description : Convert a MAC address ASCII string to a MAC address.
*
* Argument(s) : paddr_mac_ascii     Pointer to an ASCII string that contains a MAC address (see Note #1).
*
*               paddr_mac           Pointer to a memory buffer that will receive the converted MAC address
*                                       (see Note #2) :
*
*                                       MAC address represented by ASCII string,         if NO errors.
*
*                                       MAC address cleared to all zeros (see Note #2c), otherwise.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_ASCII_ERR_NONE                  MAC address successfully converted.
*                               NET_ASCII_ERR_NULL_PTR              Argument 'paddr_mac_ascii'/'paddr_mac'
*                                                                       passed a NULL pointer.
*                               NET_ASCII_ERR_INVALID_LEN           Invalid ASCII string length.
*                               NET_ASCII_ERR_INVALID_CHAR          Invalid ASCII character.
*                               NET_ASCII_ERR_INVALID_CHAR_LEN      Invalid ASCII character length.
*                               NET_ASCII_ERR_INVALID_CHAR_SEQ      Invalid ASCII character sequence.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*               This function is a network protocol suite application interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : (1) (a) The MAC address ASCII string MUST :
*
*                       (1) Include ONLY hexadecimal values & the colon character (':') ; ALL other 
*                           characters are trapped as invalid, including any leading or trailing
*                           characters.
*
*                       (2) (A) Include EXACTLY six hexadecimal values ...
*                           (B) ... separated ...
*                           (C) ... by  EXACTLY five colon characters.
*
*                       (3) Ensure that each hexadecimal value's number of digits does NOT exceed 
*                           the maximum number of digits, NET_ASCII_CHAR_MAX_OCTET_ADDR_MAC (2).
*
*                   (b) In other words, the MAC address ASCII string separates six hexadecimal octet 
*                       values by the colon character (':').  Each hexadecimal value represents one 
*                       octet of the MAC address starting with the most significant octet in network
*                       order.
*
*                           MAC Address Examples :
*
*                                 MAC ADDRESS ASCII STRING     HEXADECIMAL EQUIVALENT
*
*                                   "00:1A:07:AC:22:09"     =     0x001A07AC2209
*                                   "76:4E:01:D2:8C:0B"     =     0x764E01D28C0B
*                                   "80:Db:fE:0b:34:52"     =     0X80DBFE0B3452
*                                    --             --              --        --
*                                    ^               ^              ^          ^
*                                    |               |              |          |
*                                   MSO             LSO            MSO        LSO
*
*                               where
*
*                                   MSO             Most  Significant Octet in MAC Address
*                                   LSO             Least Significant Octet in MAC Address
*
*
*               (2) (a) The size of the memory buffer that will receive the returned MAC address MUST be
*                       greater than or equal to NET_ASCII_NBR_OCTET_ADDR_MAC.
*
*                   (b) MAC address accessed by octets in memory buffer array.
*
*                   (c) MAC address memory array cleared in case of any error(s).
*********************************************************************************************************
*/
/*$PAGE*/
void  NetASCII_Str_to_MAC (CPU_CHAR    *paddr_mac_ascii,
                           CPU_INT08U  *paddr_mac,
                           NET_ERR     *perr)
{
    CPU_CHAR    *pchar_cur;
    CPU_CHAR    *pchar_prev;
    CPU_INT08U  *paddr_cur;
    CPU_INT16U   addr_octet_val;
    CPU_INT16U   addr_octet_val_dig;
    CPU_INT32U   addr_nbr_octet;
    CPU_INT08U   addr_nbr_octet_dig;


                                                                        /* -------------- VALIDATE PTRS --------------- */
    if (paddr_mac == (CPU_INT08U *)0) {
       *perr = NET_ASCII_ERR_NULL_PTR;
        return;
    }

    if (paddr_mac_ascii == (CPU_CHAR *)0) {
        Mem_Clr((void     *)paddr_mac,                                  /* Clr rtn addr on err (see Note #2c).          */
                (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
       *perr = NET_ASCII_ERR_NULL_PTR;
        return;
    }



/*$PAGE*/
    pchar_cur          = (CPU_CHAR   *)paddr_mac_ascii;
    pchar_prev         = (CPU_CHAR   *)0;
    paddr_cur          = (CPU_INT08U *)paddr_mac;
    addr_octet_val     =  0x0000;
    addr_nbr_octet     =  0;
    addr_nbr_octet_dig =  0;

    while ((pchar_cur != (CPU_CHAR *)0) &&                              /* Parse ALL non-NULL chars in ASCII str.       */
          (*pchar_cur != (CPU_CHAR  )0)) {

        switch (*pchar_cur) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                 addr_nbr_octet_dig++;                                  /* If nbr digs > max (see Note #1a3); ...       */
                 if (addr_nbr_octet_dig > NET_ASCII_CHAR_MAX_OCTET_ADDR_MAC) {
                     Mem_Clr((void     *)paddr_mac,                     /* ... clr rtn addr  (see Note #2c)   ...       */
                             (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
                    *perr = NET_ASCII_ERR_INVALID_CHAR_LEN;             /* ... & rtn err.                               */
                     return;
                 }
                                                                        
                 switch (*pchar_cur) {                                  /* Convert ASCII char into hex val.             */
                     case '0':
                     case '1':
                     case '2':
                     case '3':
                     case '4':
                     case '5':
                     case '6':
                     case '7':
                     case '8':
                     case '9':
                          addr_octet_val_dig = (CPU_INT16U)(*pchar_cur - '0');
                          break;


                     case 'A':
                     case 'B':
                     case 'C':
                     case 'D':
                     case 'E':
                     case 'F':
                          addr_octet_val_dig = (CPU_INT16U)(*pchar_cur - 'A' + 10);
                          break;


                     case 'a':
                     case 'b':
                     case 'c':
                     case 'd':
                     case 'e':
                     case 'f':
                          addr_octet_val_dig = (CPU_INT16U)(*pchar_cur - 'a' + 10);
                          break;


                     default:                                           /* See Note #1a1.                               */
                          Mem_Clr((void     *)paddr_mac,                /* Clr rtn addr on err (see Note #2c).          */
                                  (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
                         *perr = NET_ASCII_ERR_INVALID_CHAR;
                          return;                                       /* Prevent 'break NOT reachable' warning.       */
                 }
                                                                        /* Merge ASCII char into hex val.               */
                 addr_octet_val *= 16;
                 addr_octet_val += addr_octet_val_dig;
                 break;

/*$PAGE*/
            case ':':
                 if (pchar_prev == (CPU_CHAR *)0) {                     /* If NO prev char      (see Note #1a2B); ...   */
                     Mem_Clr((void     *)paddr_mac,                     /* ... clr rtn addr     (see Note #2c)    ...   */
                             (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
                    *perr = NET_ASCII_ERR_INVALID_CHAR_SEQ;             /* ... & rtn err.                               */
                     return;
                 }

                 if (*pchar_prev == (CPU_CHAR)':') {                    /* If prev char a colon (see Note #1a2B); ...   */
                     Mem_Clr((void     *)paddr_mac,                     /* ... clr rtn addr     (see Note #2c)    ...   */
                             (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
                    *perr = NET_ASCII_ERR_INVALID_CHAR_SEQ;             /* ... & rtn err.                               */
                     return;
                 }

                 addr_nbr_octet++;
                 if (addr_nbr_octet >= NET_ASCII_NBR_OCTET_ADDR_MAC) {  /* If nbr octets > max  (see Note #1a2A); ...   */
                     Mem_Clr((void     *)paddr_mac,                     /* ... clr rtn addr     (see Note #2c)    ...   */
                             (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
                    *perr = NET_ASCII_ERR_INVALID_LEN;                  /* ... & rtn err.                               */
                     return;
                 }

                                                                        /* Merge hex octet val into MAC addr.           */
                *paddr_cur++        = (CPU_INT08U)addr_octet_val;
                 addr_octet_val     =  0x0000;
                 addr_nbr_octet_dig =  0;
                 break;


            default:                                                    /* See Note #1a1.                               */
                 Mem_Clr((void     *)paddr_mac,                         /* Clr rtn addr on err (see Note #2c).          */
                         (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
                *perr = NET_ASCII_ERR_INVALID_CHAR;
                 return;                                                /* Prevent 'break NOT reachable' warning.       */
        }

        pchar_prev = pchar_cur;
        pchar_cur++;    
    }

    if (pchar_cur == (CPU_CHAR *)0) {                                   /* If ANY NULL ptr in ASCII str;  ..            */
        Mem_Clr((void     *)paddr_mac,                                  /* .. clr rtn addr (see Note #2c) ..            */
                (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
       *perr = NET_ASCII_ERR_NULL_PTR;                                  /* .. & rtn err.                                */
        return;
    }

    if (*pchar_prev == (CPU_CHAR)':') {                                 /* If last char a colon (see Note #1a2B); ..    */
        Mem_Clr((void     *)paddr_mac,                                  /* .. clr rtn addr     (see Note #2c)     ..    */
                (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
       *perr = NET_ASCII_ERR_INVALID_CHAR_SEQ;                          /* .. & rtn err.                                */
        return;
    }

    addr_nbr_octet++;
    if (addr_nbr_octet != NET_ASCII_NBR_OCTET_ADDR_MAC) {               /* If != nbr MAC addr octets (see Note #1a2A);  */
        Mem_Clr((void     *)paddr_mac,                                  /* .. clr rtn addr           (see Note #2c)  .. */
                (CPU_SIZE_T)NET_ASCII_NBR_OCTET_ADDR_MAC);
       *perr = NET_ASCII_ERR_INVALID_LEN;                               /* .. & rtn err.                                */
        return;
    }

                                                                        /* Merge hex octet val into final MAC addr.     */
   *paddr_cur = (CPU_INT08U)addr_octet_val;

   *perr      =  NET_ASCII_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        NetASCII_MAC_to_Str()
*
* Description : Convert a MAC address into a MAC address ASCII string.
*
* Argument(s) : paddr_mac           Pointer to a memory buffer that contains the MAC address (see Note #2).
*
*               paddr_mac_ascii     Pointer to an ASCII character array that will receive the return MAC
*                                       address ASCII string from this function (see Note #1).
*
*               hex_lower_case      Format alphabetic hexadecimal characters in lower case :
*
*                                       DEF_NO      Format alphabetic hexadecimal characters in upper case.
*                                       DEF_YES     Format alphabetic hexadecimal characters in lower case.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_ASCII_ERR_NONE                  ASCII string successfully formatted.
*                               NET_ASCII_ERR_NULL_PTR              Argument 'paddr_mac'/'paddr_mac_ascii'
*                                                                       passed a NULL pointer.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*               This function is a network protocol suite application interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : (1) (a) (1) The return MAC address ASCII string ...
*
*                           (A) Formats EXACTLY six hexadecimal values ...
*                           (B) ... separated ...
*                           (C) ... by  EXACTLY five colon characters.
*
*                       (2) The size of the ASCII character array that will receive the returned MAC address
*                           ASCII string MUST be greater than or equal to NET_ASCII_LEN_MAX_ADDR_MAC.
*
*                   (b) In other words, the MAC address ASCII string separates six hexadecimal octet values
*                       by the colon character (':').  Each hexadecimal value represents one octet of the
*                       MAC address starting with the most significant octet in network order.
*
*                           MAC Address Examples :
*
*                                 MAC ADDRESS ASCII STRING     HEXADECIMAL EQUIVALENT
*
*                                   "00:1A:07:AC:22:09"     =     0x001A07AC2209
*                                   "76:4E:01:D2:8C:0B"     =     0x764E01D28C0B
*                                   "80:Db:fE:0b:34:52"     =     0X80DBFE0B3452
*                                    --             --              --        --
*                                    ^               ^              ^          ^
*                                    |               |              |          |
*                                   MSO             LSO            MSO        LSO
*
*                               where
*
*                                   MSO             Most  Significant Octet in MAC Address
*                                   LSO             Least Significant Octet in MAC Address
*
*
*               (2) (a) The size of the memory buffer that contains the MAC address MUST be greater than
*                       or equal to NET_ASCII_NBR_OCTET_ADDR_MAC.
*
*                   (b) MAC address accessed by octets in memory buffer array.
*********************************************************************************************************
*/
/*$PAGE*/
void  NetASCII_MAC_to_Str (CPU_INT08U   *paddr_mac,
                           CPU_CHAR     *paddr_mac_ascii,
                           CPU_BOOLEAN   hex_lower_case,
                           NET_ERR      *perr)
{
    CPU_CHAR    *pchar;
    CPU_INT08U  *paddr;
    CPU_INT08U   addr_octet_nbr_shifts;
    CPU_INT08U   addr_octet_val;
    CPU_INT08U   addr_octet_dig_val;
    CPU_INT08U   i;
    CPU_INT08U   j;


                                                                        /* -------------- VALIDATE PTRS --------------- */
    if (paddr_mac == (CPU_INT08U *)0) {
       *perr = NET_ASCII_ERR_NULL_PTR;
        return;
    }
    if (paddr_mac_ascii == (CPU_CHAR *)0) {
       *perr = NET_ASCII_ERR_NULL_PTR;
        return;
    }


    paddr = paddr_mac;
    pchar = paddr_mac_ascii;

    for (i = NET_ASCII_NBR_OCTET_ADDR_MAC; i > 0; i--) {                /* Parse ALL addr octets.                       */

        addr_octet_val = *paddr;

        for (j = NET_ASCII_CHAR_MAX_OCTET_ADDR_MAC; j > 0; j--) {       /* Parse ALL octet's hex digs.                  */
                                                                        /* Calc  cur octet's hex dig val.               */
            addr_octet_nbr_shifts = (j - 1) * DEF_NIBBLE_NBR_BITS;
            addr_octet_dig_val    = (addr_octet_val >> addr_octet_nbr_shifts) & DEF_NIBBLE_MASK;
                                                                        /* Insert octet hex val ASCII dig.              */
            if (addr_octet_dig_val < 10) {
               *pchar++ = (CPU_CHAR)(addr_octet_dig_val + '0');
                
            } else {
                if (hex_lower_case != DEF_YES) {
                   *pchar++ = (CPU_CHAR)(addr_octet_dig_val - 10 + 'A');
                } else {
                   *pchar++ = (CPU_CHAR)(addr_octet_dig_val - 10 + 'a');
                }
            }
        }

        if (i > 1) {                                                    /* If NOT on last octet, append a colon char.   */
           *pchar++ = ':';
        }

        paddr++;
    }

   *pchar = (CPU_CHAR)0;                                                /* Append NULL char.                            */

   *perr  =  NET_ASCII_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        NetASCII_Str_to_IP()
*
* Description : Convert an IPv4 address in ASCII dotted-decimal notation to a network protocol IPv4 address
*                   in host-order.
*
* Argument(s) : paddr_ip_ascii      Pointer to an ASCII string that contains a dotted-decimal IPv4 address
*                                       (see Note #2).
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_ASCII_ERR_NONE                  IPv4 address successfully converted.
*                               NET_ASCII_ERR_NULL_PTR              Argument 'paddr_ip_ascii' passed a
*                                                                       NULL pointer.
*                               NET_ASCII_ERR_INVALID_LEN           Invalid ASCII string length.
*                               NET_ASCII_ERR_INVALID_CHAR          Invalid ASCII character.
*                               NET_ASCII_ERR_INVALID_CHAR_LEN      Invalid ASCII character length.
*                               NET_ASCII_ERR_INVALID_CHAR_VAL      Invalid ASCII character value.
*                               NET_ASCII_ERR_INVALID_CHAR_SEQ      Invalid ASCII character sequence.
*
* Return(s)   : Host-order IPv4 address represented by ASCII string, if NO errors.
*
*               NET_IP_ADDR_NONE,                                    otherwise.
*
* Caller(s)   : Application.
*
*               This function is a network protocol suite application interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : (1) RFC #1983 states that "dotted decimal notation ... refers [to] IP addresses of the
*                   form A.B.C.D; where each letter represents, in decimal, one byte of a four byte IP
*                   address".
*
*                   In other words, the dotted-decimal notation separates four decimal octet values by
*                   the dot, or period, character ('.').  Each decimal value represents one octet of 
*                   the IP address starting with the most significant octet in network order.
*
*                       IP Address Examples :
*
*                              DOTTED DECIMAL NOTATION     HEXADECIMAL EQUIVALENT
*
*                                   127.0.0.1           =       0x7F000001
*                                   192.168.1.64        =       0xC0A80140
*                                   255.255.255.0       =       0xFFFFFF00
*                                   ---         -                 --    --
*                                    ^          ^                 ^      ^
*                                    |          |                 |      |
*                                   MSO        LSO               MSO    LSO
*
*                           where
*
*                               MSO             Most  Significant Octet in Dotted Decimal IP Address
*                               LSO             Least Significant Octet in Dotted Decimal IP Address
*
*
*               (2) The dotted-decimal ASCII string MUST :
*
*                   (a) Include ONLY decimal values & the dot, or period, character ('.') ; ALL other
*                       characters are trapped as invalid, including any leading or trailing characters.
*
*                   (b) (1) Include EXACTLY four decimal values ...
*                       (2) ... separated ...
*                       (3) ... by  EXACTLY three dot characters.
*
*                   (c) Ensure that each decimal value does NOT exceed the maximum octet value (i.e. 255).
*
*                   (d) Ensure that each decimal value's number of decimal digits, including leading zeros,
*                       does NOT exceed the maximum number of digits, NET_ASCII_CHAR_MAX_OCTET_ADDR_IP (3).
*********************************************************************************************************
*/
/*$PAGE*/
NET_IP_ADDR  NetASCII_Str_to_IP (CPU_CHAR  *paddr_ip_ascii,
                                 NET_ERR   *perr)
{
    CPU_CHAR     *pchar_cur;
    CPU_CHAR     *pchar_prev;
    NET_IP_ADDR   addr_ip;
    CPU_INT16U    addr_octet_val;
    CPU_INT32U    addr_nbr_octet;
    CPU_INT08U    addr_nbr_octet_dig;


                                                                        /* --------------- VALIDATE PTR --------------- */
    if (paddr_ip_ascii == (CPU_CHAR *)0) {
       *perr =  NET_ASCII_ERR_NULL_PTR;
        return (NET_IP_ADDR_NONE);
    }



    pchar_cur          = (CPU_CHAR *)paddr_ip_ascii;
    pchar_prev         = (CPU_CHAR *)0;
    addr_ip            =  NET_IP_ADDR_NONE;
    addr_octet_val     =  0x0000;
    addr_nbr_octet     =  0;
    addr_nbr_octet_dig =  0;

    while ((pchar_cur != (CPU_CHAR *)0) &&                              /* Parse ALL non-NULL chars in ASCII str.       */
          (*pchar_cur != (CPU_CHAR  )0)) {

        switch (*pchar_cur) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                 addr_nbr_octet_dig++;                                  /* If nbr digs > max (see Note #2d), ...        */
                 if (addr_nbr_octet_dig > NET_ASCII_CHAR_MAX_OCTET_ADDR_IP) {
                    *perr =  NET_ASCII_ERR_INVALID_CHAR_LEN;            /* ... rtn err.                                 */
                     return (NET_IP_ADDR_NONE);
                 }

                                                                        /* Convert & merge ASCII char into decimal val. */
                 addr_octet_val *= (CPU_INT16U)10;
                 addr_octet_val += (CPU_INT16U)(*pchar_cur - '0');

                 if (addr_octet_val > NET_ASCII_VAL_MAX_OCTET_ADDR_IP) {/* If octet val > max (see Note #2c), ...       */
                    *perr =  NET_ASCII_ERR_INVALID_CHAR_VAL;            /* ... rtn err.                                 */
                     return (NET_IP_ADDR_NONE);
                 }
                 break;

 
            case '.':
                 if (pchar_prev == (CPU_CHAR *)0) {                     /* If NO prev char     (see Note #2b2), ...     */
                    *perr =  NET_ASCII_ERR_INVALID_CHAR_SEQ;            /* ... rtn err.                                 */
                     return (NET_IP_ADDR_NONE);
                 }

                 if (*pchar_prev == (CPU_CHAR)'.') {                    /* If prev char a dot  (see Note #2b2), ...     */
                     *perr =  NET_ASCII_ERR_INVALID_CHAR_SEQ;           /* ... rtn err.                                 */
                      return (NET_IP_ADDR_NONE);
                 }

                 addr_nbr_octet++;
                 if (addr_nbr_octet >= NET_ASCII_NBR_OCTET_ADDR_IP) {   /* If nbr octets > max (see Note #2b1), ...     */
                    *perr =  NET_ASCII_ERR_INVALID_LEN;                 /* ... rtn err.                                 */
                     return (NET_IP_ADDR_NONE);
                 }

                                                                        /* Merge decimal octet val into IP addr.        */
                 addr_ip            <<=  DEF_OCTET_NBR_BITS;
                 addr_ip             += (CPU_INT08U)addr_octet_val;
                 addr_octet_val       =  0x0000;
                 addr_nbr_octet_dig   =  0;
                 break;


            default:                                                    /* See Note #2a.                                */
                *perr =  NET_ASCII_ERR_INVALID_CHAR;
                 return (NET_IP_ADDR_NONE);                             /* Prevent 'break NOT reachable' warning.       */
        }

        pchar_prev = pchar_cur;
        pchar_cur++;    
    }

/*$PAGE*/
    if (pchar_cur == (CPU_CHAR *)0) {                                   /* If ANY NULL ptr in ASCII str, ..             */
       *perr =  NET_ASCII_ERR_NULL_PTR;                                 /* .. rtn err.                                  */
        return (NET_IP_ADDR_NONE);
    }

    if (*pchar_prev == (CPU_CHAR)'.') {                                 /* If last char a dot (see Note #2b2), ..       */
        *perr =  NET_ASCII_ERR_INVALID_CHAR_SEQ;                        /* .. rtn err.                                  */
         return (NET_IP_ADDR_NONE);
    }

    addr_nbr_octet++;
    if (addr_nbr_octet != NET_ASCII_NBR_OCTET_ADDR_IP) {                /* If != nbr IP addr octets (see Note #2b1), .. */
       *perr =  NET_ASCII_ERR_INVALID_LEN;                              /* .. rtn err.                                  */
        return (NET_IP_ADDR_NONE);
    }

                                                                        /* Merge decimal octet val into final IP addr.  */
    addr_ip <<=  DEF_OCTET_NBR_BITS;
    addr_ip  += (CPU_INT08U)addr_octet_val;

   *perr      =  NET_ASCII_ERR_NONE;

    return (addr_ip);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        NetASCII_IP_to_Str()
*
* Description : Convert a network protocol IPv4 address in host-order into a dotted-decimal notation ASCII
*                   string.
*
* Argument(s) : addr_ip             IPv4 address.
*
*               paddr_ip_ascii      Pointer to an ASCII character array that will receive the return IP
*                                       address ASCII string from this function (see Note #2).
*
*               lead_zeros          Prepend leading zeros option  (see Note #3) :
*
*                                       DEF_NO      Do NOT prepend leading zeros to each decimal octet value.
*                                       DEF_YES            Prepend leading zeros to each decimal octet value.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_ASCII_ERR_NONE                  ASCII string successfully formatted.
*                               NET_ASCII_ERR_NULL_PTR              Argument 'paddr_ip_ascii' passed a
*                                                                       NULL pointer.
*                               NET_ASCII_ERR_INVALID_CHAR_LEN      Invalid ASCII character length.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*               This function is a network protocol suite application interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : (1) RFC #1983 states that "dotted decimal notation ... refers [to] IP addresses of the
*                   form A.B.C.D; where each letter represents, in decimal, one byte of a four byte IP
*                   address".
*
*                   In other words, the dotted-decimal notation separates four decimal octet values by
*                   the dot, or period, character ('.').  Each decimal value represents one octet of 
*                   the IP address starting with the most significant octet in network order.
*
*                       IP Address Examples :
*
*                              DOTTED DECIMAL NOTATION     HEXADECIMAL EQUIVALENT
*
*                                   127.0.0.1           =       0x7F000001
*                                   192.168.1.64        =       0xC0A80140
*                                   255.255.255.0       =       0xFFFFFF00
*                                   ---         -                 --    --
*                                    ^          ^                 ^      ^
*                                    |          |                 |      |
*                                   MSO        LSO               MSO    LSO
*
*                           where
*
*                               MSO             Most  Significant Octet in Dotted Decimal IP Address
*                               LSO             Least Significant Octet in Dotted Decimal IP Address
*
*
*               (2) The size of the ASCII character array that will receive the return IP address ASCII 
*                   string MUST be greater than or equal to NET_ASCII_LEN_MAX_ADDR_IP.
*
*               (3) (a) Leading zeros option prepends leading '0's prior to the first non-zero digit
*                       in each decimal octet value.  The number of leading zeros is such that the 
*                       decimal octet's number of decimal digits is equal to the maximum number of 
*                       digits, NET_ASCII_CHAR_MAX_OCTET_ADDR_IP (3).
*
*                   (b) (1) If leading zeros option DISABLED              ...
*                       (2) ... & the decimal value of the octet is zero; ...
*                       (3) ... then one digit of '0' value is formatted.
*
*                           This is NOT a leading zero; but a single decimal digit of '0' value.
*********************************************************************************************************
*/
/*$PAGE*/
void  NetASCII_IP_to_Str (NET_IP_ADDR   addr_ip,
                          CPU_CHAR     *paddr_ip_ascii,
                          CPU_BOOLEAN   lead_zeros,
                          NET_ERR      *perr)
{
    CPU_CHAR    *pchar;
    CPU_INT08U   base_10_val_start;
    CPU_INT08U   base_10_val;
    CPU_INT08U   addr_octet_nbr_shifts;
    CPU_INT08U   addr_octet_val;
    CPU_INT08U   addr_octet_dig_nbr;
    CPU_INT08U   addr_octet_dig_val;
    CPU_INT16U   i;


                                                                        /* -------------- VALIDATE PTR ---------------- */
    if (paddr_ip_ascii == (CPU_CHAR *)0) {
       *perr = NET_ASCII_ERR_NULL_PTR;
        return;
    }
                                                                        /* ------------ VALIDATE NBR CHAR ------------- */
#if ((NET_ASCII_CHAR_MAX_OCTET_ADDR_IP < NET_ASCII_CHAR_MIN_OCTET   ) || \
     (NET_ASCII_CHAR_MAX_OCTET_ADDR_IP > NET_ASCII_CHAR_MAX_OCTET_08))
   *paddr_ip_ascii = (CPU_CHAR)0;
   *perr           =  NET_ASCII_ERR_INVALID_CHAR_LEN;
    return;
#endif


    pchar = paddr_ip_ascii;


    base_10_val_start = 1;    
    for (i = 1; i < NET_ASCII_CHAR_MAX_OCTET_ADDR_IP; i++) {            /* Calc starting dig val.                       */
        base_10_val_start *= 10;
    }


    for (i = NET_ASCII_NBR_OCTET_ADDR_IP; i > 0; i--) {                 /* Parse ALL addr octets.                       */
                                                                        /* Calc  cur addr octet val.                    */
        addr_octet_nbr_shifts = (i - 1) * DEF_OCTET_NBR_BITS;
        addr_octet_val        = (CPU_INT08U)((addr_ip >> addr_octet_nbr_shifts) & DEF_OCTET_MASK);

        base_10_val           =  base_10_val_start;
        while (base_10_val > 0) {                                       /* Parse ALL octet digs.                        */
            addr_octet_dig_nbr = addr_octet_val / base_10_val;

            if ((addr_octet_dig_nbr >  0) ||                            /* If octet dig val > 0,                     .. */
                (base_10_val        == 1) ||                            /* .. OR on least-significant octet dig,     .. */
                (lead_zeros == DEF_YES)) {                              /* .. OR lead zeros opt ENABLED (see Note #3),  */
                                                                        /* .. calc & insert octet val ASCII dig.        */
                 addr_octet_dig_val = (CPU_INT08U)(addr_octet_dig_nbr % 10 );
                *pchar++            = (CPU_CHAR  )(addr_octet_dig_val + '0');
            }

            base_10_val /= 10;                                          /* Shift to next least-significant octet dig.   */
        }

        if (i > 1) {                                                    /* If NOT on last octet, append a dot char.     */
           *pchar++ = '.';
        }
    }

   *pchar = (CPU_CHAR)0;                                                /* Append NULL char.                            */

   *perr  =  NET_ASCII_ERR_NONE;
}

