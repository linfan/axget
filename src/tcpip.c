/********************************************************************\
 * Axel -- A lighter download accelerator for Linux and other Unices. *
 *                                                                    *
 * Copyright 2001 Wilmer van der Gaast                                *
 \********************************************************************/

/* TCP control file                         */

/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License with
   the Debian GNU/Linux distribution in file /usr/doc/copyright/GPL;
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA  02111-1307  USA
   */

#include <ctype.h>
#include "axel.h"

/* Get a TCP connection */
int tcp_connect(char *hostname, int port, char *local_if)
{
    struct hostent *host = NULL;
    struct sockaddr_in addr;
    struct sockaddr_in local;
    int fd;

    AXGET_FUN_BEGIN

    IF_DEBUG
    {
        fprintf(stderr, "tcp_connect( %s, %i ) = ", hostname, port);
    }

    /* Why this loop? Because the call might return an empty record.
       At least it very rarely does, on my system...        */
    for(fd = 0; fd < 5; fd ++)
    {
        if((host = gethostbyname(hostname)) == NULL)
        {
            AXGET_FUN_LEAVE
                return(-1);
        }

        if(*host->h_name) break;
    }

    if(!host || !host->h_name || !*host->h_name)
    {
        AXGET_FUN_LEAVE
            return(-1);
    }

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        AXGET_FUN_LEAVE
            return(-1);
    }

    if(local_if && *local_if)
    {
        local.sin_family = AF_INET;
        local.sin_port = 0;
        local.sin_addr.s_addr = inet_addr(local_if);

        if(bind(fd, (struct sockaddr *) &local, sizeof(struct sockaddr_in)) == -1)
        {
            close(fd);

            AXGET_FUN_LEAVE
                return(-1);
        }
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((struct in_addr *) host->h_addr);

    if(connect(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1)
    {
        close(fd);

        AXGET_FUN_LEAVE
            return(-1);
    }

    IF_DEBUG
    {
        socklen_t i = sizeof(local);
        getsockname(fd, (struct sockaddr*) &local, &i);
        fprintf(stderr, "%i\n", ntohs(local.sin_port));
    }

    AXGET_FUN_LEAVE
        return(fd);
}

/* get ip from name of network interface card */
int get_if_ip(char *iface, char *ip)
{
    struct ifreq ifr;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    AXGET_FUN_BEGIN

        memset(&ifr, 0, sizeof(struct ifreq));
    strcpy(ifr.ifr_name, iface);
    ifr.ifr_addr.sa_family = AF_INET;

    if(ioctl(fd, SIOCGIFADDR, &ifr) == 0)
    {
        struct sockaddr_in *x = (struct sockaddr_in *) &ifr.ifr_addr;
        strcpy(ip, inet_ntoa(x->sin_addr));

        AXGET_FUN_LEAVE
            return(1);
    }
    else
    {
        AXGET_FUN_LEAVE
            return(0);
    }

    AXGET_FUN_LEAVE
}

/* Return a textual representation of ADDR, i.e. the dotted quad for
   IPv4 addresses, and the colon-separated list of hex words (with all
   zeros omitted, etc.) for IPv6 addresses.  */

const char* print_address (const ip_address *addr)
{
#ifdef ENABLE_IPV6
    static char buf[64];
    if (!inet_ntop (addr->family, IP_INADDR_DATA (addr), buf, sizeof buf))
        snprintf (buf, sizeof buf, "<error: %s>", strerror (errno));
    return buf;
#else
    return inet_ntoa (addr->data.d4);
#endif
}

/* The following two functions were adapted from glibc's
   implementation of inet_pton, written by Paul Vixie. */

static int is_valid_ipv4_address (const char *str, const char *end)
{
    bool saw_digit = false;
    int octets = 0;
    int val = 0;

    AXGET_FUN_BEGIN

    while (str < end)
    {
        int ch = *str++;

        if (ch >= '0' && ch <= '9')
        {
            val = val * 10 + (ch - '0');

            if (val > 255)
            {
                AXGET_FUN_LEAVE
                return false;
            }
            if (!saw_digit)
            {
                if (++octets > 4)
                {
                    AXGET_FUN_LEAVE
                    return false;
                }
                saw_digit = true;
            }
        }
        else if (ch == '.' && saw_digit)
        {
            if (octets == 4)
            {
                AXGET_FUN_LEAVE
                return false;
            }
            val = 0;
            saw_digit = false;
        }
        else
        {
            AXGET_FUN_LEAVE
            return false;
        }
    }
    if (octets < 4)
    {
        AXGET_FUN_LEAVE
        return false;
    }

    AXGET_FUN_LEAVE
    return true;
}

#ifdef ENABLE_IPV6
static int is_valid_ipv6_address (const char *str, const char *end)
{
    /* Use lower-case for these to avoid clash with system headers.  */
    enum {
        ns_inaddrsz  = 4,
        ns_in6addrsz = 16,
        ns_int16sz   = 2
    };

    const char *curtok;
    int tp;
    const char *colonp;
    bool saw_xdigit;
    unsigned int val;

    AXGET_FUN_BEGIN

    tp = 0;
    colonp = NULL;

    if (str == end)
    {
        AXGET_FUN_LEAVE
        return false;
    }

    /* Leading :: requires some special handling. */
    if (*str == ':')
    {
        ++str;
        if (str == end || *str != ':')
        {
            AXGET_FUN_LEAVE
            return false;
        }
    }

    curtok = str;
    saw_xdigit = false;
    val = 0;

    while (str < end)
    {
        int ch = *str++;

        /* if ch is a number, add it to val. */
        if (isxdigit(ch))
        {
            val <<= 4;
            val |= XDIGIT_TO_NUM (ch);
            if (val > 0xffff)
            {
                AXGET_FUN_LEAVE
                return false;
            }
            saw_xdigit = true;
            continue;
        }

        /* if ch is a colon ... */
        if (ch == ':')
        {
            curtok = str;
            if (!saw_xdigit)
            {
                if (colonp != NULL)
                {
                    AXGET_FUN_LEAVE
                    return false;
                }
                colonp = str + tp;
                continue;
            }
            else if (str == end)
            {
                AXGET_FUN_LEAVE
                return false;
            }
            if (tp > ns_in6addrsz - ns_int16sz)
            {
                AXGET_FUN_LEAVE
                return false;
            }
            tp += ns_int16sz;
            saw_xdigit = false;
            val = 0;
            continue;
        }

        /* if ch is a dot ... */
        if (ch == '.' && (tp <= ns_in6addrsz - ns_inaddrsz)
                && is_valid_ipv4_address (curtok, end) == 1)
        {
            tp += ns_inaddrsz;
            saw_xdigit = false;
            break;
        }

        AXGET_FUN_LEAVE
        return false;
    }

    if (saw_xdigit)
    {
        if (tp > ns_in6addrsz - ns_int16sz)
        {
            AXGET_FUN_LEAVE
            return false;
        }
        tp += ns_int16sz;
    }

    if (colonp != NULL)
    {
        if (tp == ns_in6addrsz)
        {
            AXGET_FUN_LEAVE
            return false;
        }
        tp = ns_in6addrsz;
    }

    if (tp != ns_in6addrsz)
    {
        AXGET_FUN_LEAVE
        return false;
    }

    AXGET_FUN_LEAVE
    return true;
}
#endif

int is_valid_ip_address (const char *name)
{
    const char *endp;

    endp = name + strlen(name);
    if (is_valid_ipv4_address (name, endp))
        return true;
#ifdef ENABLE_IPV6
    if (is_valid_ipv6_address (name, endp))
        return true;
#endif
    return false;
}

