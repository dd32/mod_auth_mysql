APXS_VER ?= 2
ifeq (1,$(APXS_VER))
APXS=apxs
else
APXS=apxs2
endif
APXSFLAGS = -I/usr/include/mysql
RM = rm -f
LIBS = -lmysqlclient -lm -lz -lcrypto -L/usr/lib/mysql
CC = gcc
LD = $(CC)
CFLAGS = -Wall -O2 -fomit-frame-pointer -funroll-loops

all: mod_auth_mysql.la

mod_auth_mysql.la: mod_auth_mysql.c crypt_private.c
	$(APXS) -c $(LIBS) $(APXSFLAGS) mod_auth_mysql.c crypt_private.c

install: all
	$(APXS) -i mod_auth_mysql.la

crypt-private-test:
	$(CC) $(CFLAGS) $(LIBS) crypt_private.c -DTEST -o crypt-private-test

clean:
	@$(RM) *.o *.lo *.slo *.la  
	@$(RM) crypt-private-test 
	@$(RM) -r .libs
