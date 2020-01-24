APXS_VER ?= 2
ifeq (1,$(APXS_VER))
APXS=apxs
else
APXS=apxs2
endif
APXSFLAGS = `mysql_config --cflags`
RM = rm -f
LIBS = `mysql_config --libs` -lm -lz -lcrypto
CC = gcc
LD = $(CC)
CFLAGS = -Wall -O2 -fomit-frame-pointer -funroll-loops

all: mod_auth_mysql.la

mod_auth_mysql.la: mod_auth_mysql.c crypt_private.c crypt_scrambled.c crypt_blowfish.c
	$(APXS) -c $(LIBS) $(APXSFLAGS) mod_auth_mysql.c crypt_private.c crypt_scrambled.c crypt_blowfish.c

install: all
	$(APXS) -i mod_auth_mysql.la

crypt-private-test:
	$(CC) $(CFLAGS) $(LIBS) crypt_private.c -DTEST -o crypt-private-test

clean:
	@$(RM) *.o *.lo *.slo *.la
	@$(RM) crypt-private-test
	@$(RM) -r .libs
