#!/bin/bash

KADMIN="kadmin -p kadmin/admin -w password"

if ! $KADMIN list_principals | grep -w host/$(hostname -f)@EXAMPLE.COM
then
$KADMIN add_principal -randkey host/$(hostname -f)@EXAMPLE.COM
fi

$KADMIN ktadd -k /etc/krb5.keytab host/$(hostname -f)@EXAMPLE.COM

if ! $KADMIN list_principals | grep -w auks/auks.example.com@EXAMPLE.COM
then
$KADMIN add_principal -randkey auks/auks.example.com@EXAMPLE.COM
fi

$KADMIN ktadd -k /etc/krb5.keytab auks/auks.example.com@EXAMPLE.COM

klist -kt
kinit -k host/$(hostname -f)@EXAMPLE.COM

export KRB5_TRACE=/dev/stderr
exec /usr/local/sbin/auksd -F -ddd -vvv -f /conf/auks.conf
