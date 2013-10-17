/********************************************************************\
* Axel -- A lighter download accelerator for Linux and other Unices. *
*                                                                    *
* Copyright 2001 Wilmer van der Gaast                                *
\********************************************************************/

/* TCP control include file                     */

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

#ifndef _TCP_H_
#define _TCP_H_

typedef struct {
  /* Address family, one of AF_INET or AF_INET6. */
  int family;

  /* The actual data, in the form of struct in_addr or in6_addr: */
  union {
    struct in_addr d4;		/* IPv4 address */
#ifdef ENABLE_IPV6
    struct in6_addr d6;		/* IPv6 address */
#endif
  } data;

  /* Under IPv6 getaddrinfo also returns scope_id.  Since it's
     IPv6-specific it strictly belongs in the above union, but we put
     it here for simplicity.  */
#if defined ENABLE_IPV6 && defined HAVE_SOCKADDR_IN6_SCOPE_ID
  int ipv6_scope;
#endif
} ip_address;

#define XDIGIT_TO_NUM(h) ((h) < 'A' ? (h) - '0' : toupper (h) - 'A' + 10)

int tcp_connect(char *hostname, int port, char *local_if);
int get_if_ip(char *iface, char *ip);
int is_valid_ip_address (const char *name);

#endif // _TCP_H_
