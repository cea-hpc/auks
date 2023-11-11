#!/usr/bin/env bats
function setup() {
    KADMIN="kadmin -p kadmin/admin -w password"
    if ! $KADMIN list_principals | grep -w '^user@EXAMPLE.COM$'; then
        $KADMIN add_principal -randkey user
    fi  
    if ! $KADMIN list_principals | grep -e '^admin@EXAMPLE.COM$'; then
        $KADMIN add_principal -randkey admin
    fi  
    test -e /user.keytab && rm /user.keytab
    test -e /admin.keytab && rm /admin.keytab
    $KADMIN ktadd -k /user.keytab user
    $KADMIN ktadd -k /admin.keytab admin

}

@test "Ping as host" {
    kinit -k
    auks -f /conf/auks.conf -p
}

@test "Ping/Add/Get own cred as regular user" {
    kinit -k -t /user.keytab user
    auks -f /conf/auks.conf -p
    auks -f /conf/auks.conf -a
    auks -f /conf/auks.conf -g -p 1234
}

@test "Ping/Add/Get own cred as admin user" {
    kinit -k -t /admin.keytab admin
    auks -f /conf/auks.conf -p
    auks -f /conf/auks.conf -a
    auks -f /conf/auks.conf -g -p 4321
}

@test "Add/Get user cred as admin user" {
    kinit -k -t /admin.keytab admin
    kinit -k -t /user.keytab -c /tmp/krb5cc_bats user
    auks -f /conf/auks.conf -p
    auks -f /conf/auks.conf -a -C /tmp/krb5cc_bats
    rm /tmp/krb5cc_bats
    auks -f /conf/auks.conf -g -p 1234 -C /tmp/krb5cc_bats
}

@test "Add/Get admin cred as user user" {
    kinit -k -t /user.keytab user
    kinit -k -t /admin.keytab -c /tmp/krb5cc_bats admin
    auks -f /conf/auks.conf -p
    run auks -f /conf/auks.conf -a -C /tmp/krb5cc_bats
    [ "$status" -ne 0 ]
    rm /tmp/krb5cc_bats
    auks -f /conf/auks.conf -g -p 4321 -C /tmp/krb5cc_bats
    run bash -c 'klist -fanc /tmp/krb5cc_bats | grep admin'
    [ "$status" -eq 1 ]
    run test -f /run/krb5cc_bats
    [ "$status" -eq 1 ]
}

@test "Cannot dump creds as regular user" {
    kinit -k -t /user.keytab user
    run auks -f /conf/auks.conf -D
    [ "$status" -ne 0 ]
}

@test "Able to dump creds as a admin user" {
    kinit -k -t /admin.keytab admin
    auks -f /conf/auks.conf -D
}

@test "Able to remove my own credential" {
    kinit -k -t /user.keytab user
    auks -f /conf/auks.conf -a -u 1234
    auks -f /conf/auks.conf -r -u 1234
    run bash -c 'auks -f /conf/auks.conf -D | grep user'
    [ "$status" -eq 1 ]
    kinit -k -t /admin.keytab admin
    auks -f /conf/auks.conf -a -u 4321
    auks -f /conf/auks.conf -r -u 4321
    run bash -c 'auks -f /conf/auks.conf -D | grep user'
    [ "$status" -eq 1 ]
}

@test "Admins are able to remove all credentials" {
    kinit -k -t /admin.keytab admin
    kinit -k -t /user.keytab -c /tmp/krb5cc_bats user
    auks -f /conf/auks.conf -a -u 1234 -C /tmp/krb5cc_bats
    auks -f /conf/auks.conf -a -u 4321
    auks -f /conf/auks.conf -D | grep -w user@EXAMPLE.COM
    auks -f /conf/auks.conf -D | grep -w admin@EXAMPLE.COM
    auks -f /conf/auks.conf -r -u 1234
    auks -f /conf/auks.conf -r -u 4321
    run bash -c 'auks -f /conf/auks.conf -D | grep -w user@EXAMPLE.COM'
    [ "$status" -eq 1 ]
    run bash -c 'auks -f /conf/auks.conf -D | grep -w admin@EXAMPLE.COM'
    [ "$status" -eq 1 ]
}

