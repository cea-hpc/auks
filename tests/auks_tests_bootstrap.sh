#!/bin/bash

cat $KRB5_CONF

gen_keytab(){
    local principal=$1
    local dest_kt=$2
    KADMIN="kadmin -p kadmin/admin -w password "
    if ! $KADMIN list_principals | grep  -w "${principal}"; then
	$KADMIN add_principal -randkey "${principal}"
	test -f "${dest_kt}" && rm "${dest_kt}"
	$KADMIN ktadd -k "${dest_kt}" "${principal}"
    fi
}

gen_keytab auksd@EXAMPLE.COM /krb5-keytabs/auksd.keytab
gen_keytab auks_guest@EXAMPLE.COM /krb5-keytabs/auks_guest.keytab
gen_keytab auks_user@EXAMPLE.COM /krb5-keytabs/auks_user.keytab
gen_keytab auks_unknown_user@EXAMPLE.COM /krb5-keytabs/auks_unknown_user.keytab
gen_keytab auks_admin@EXAMPLE.COM /krb5-keytabs/auks_admin.keytab

exec $@

