/********************************************************************\
* Axel -- A lighter download accelerator for Linux and other Unices. *
*                                                                    *
* Copyright 2001 Wilmer van der Gaast                                *
\********************************************************************/

/* Configuration handling include file                  */

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

#ifndef _CONF_H_
#define _CONF_H_

typedef struct
{
    char default_filename[MAX_STRING];
    char http_proxy[MAX_STRING];
    char https_proxy[MAX_STRING];
    char no_proxy[MAX_STRING];
    int strip_cgi_parameters;
    int save_state_interval;
    int connection_timeout;
    int reconnect_delay;
    int num_connections;
    int buffer_size;
    int max_speed;
    int verbose;
    int alternate_output;

#ifdef WITH_OPENSSL
    enum {
        secure_protocol_auto,
        secure_protocol_sslv2,
        secure_protocol_sslv3,
        secure_protocol_tlsv1
    } secure_protocol;        /* type of secure protocol to use. */
    int check_cert;           /* whether to validate the server's cert */
    char *cert_file;          /* external client certificate to use. */
    char *private_key;        /* private key file (if not internal). */
    enum keyfile_type {
        keyfile_pem,
        keyfile_asn1
    } cert_type,              /* type of client certificate file */
      private_key_type;       /* type of private key file */
    char *ca_directory;       /* CA directory (hash files) */
    char *ca_cert;            /* CA certificate file to use */
    char *random_file;        /* file with random data to seed the PRNG */
    char *egd_file;           /* file name of the egd daemon socket */
#endif /* WITH_OPENSSL */

    if_t *interfaces;

    int search_timeout;
    int search_threads;
    int search_amount;
    int search_top;

    int add_header_count;
    char add_header[MAX_ADD_HEADERS][MAX_STRING];

    char user_agent[MAX_STRING];
} conf_t;

int conf_loadfile(conf_t *conf, char *file);
int conf_init(conf_t *conf);

#endif // _CONF_H_
