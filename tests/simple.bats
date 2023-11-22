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
    test -e /config/auks.conf.orig && mv /config/auks.conf.orig /config/auks.conf
    true
}

@test "Ping as host" {
    kinit -k
    auks -f /conf/auks.conf -p
    auks -f /conf/auks.conf --ping
}

@test "Ping as regular user" {
    kinit -k -t /user.keytab user
    auks -f /conf/auks.conf -p
    auks -f /conf/auks.conf --ping
}

@test "Complete Ccache life cyle as regular user (short options)" {
    kinit -k -t /user.keytab user
    auks -f /conf/auks.conf -p
    run auks -f /conf/auks.conf -g -u 1234
    [ "$status" -ne 0 ]
    auks -f /conf/auks.conf -a
    auks -f /conf/auks.conf -g -u 1234
    auks -f /conf/auks.conf -r -u 1234
    run auks -f /conf/auks.conf -g -u 1234
    [ "$status" -ne 0 ]
}

@test "Complete Ccache life cyle as regular user (long options)" {
    kinit -k -t /user.keytab user
    auks --config /conf/auks.conf --ping
    run auks --config /conf/auks.conf --get --uid 1234
    [ "$status" -ne 0 ]
    auks --config /conf/auks.conf --add
    auks --config /conf/auks.conf --get --uid 1234
    auks --config /conf/auks.conf --remove --uid 1234
    run auks --config /conf/auks.conf --get --uid 1234
    [ "$status" -ne 0 ]
}

@test "Ping/Add/Get own cred as admin user" {
    kinit -k -t /admin.keytab admin
    auks -f /conf/auks.conf -p
    auks -f /conf/auks.conf -a
    auks -f /conf/auks.conf -g -u 4321
}

@test "Add/Get user cred as admin user (short options)" {
    kinit -k -t /admin.keytab admin
    kinit -k -t /user.keytab -c /tmp/krb5cc_bats user
    auks -f /conf/auks.conf -p
    auks -f /conf/auks.conf -a -C /tmp/krb5cc_bats
    rm /tmp/krb5cc_bats
    auks -f /conf/auks.conf -g -u 1234 -C /tmp/krb5cc_bats
    auks -f /conf/auks.conf -r -u 1234
}

@test "Add/Get user cred as admin user (long options)" {
    kinit -k -t /admin.keytab admin
    kinit -k -t /user.keytab -c /tmp/krb5cc_bats user
    auks --config /conf/auks.conf --ping
    auks --config /conf/auks.conf --add --ccache /tmp/krb5cc_bats
    rm /tmp/krb5cc_bats
    auks --config /conf/auks.conf --get --uid 1234 --ccache /tmp/krb5cc_bats
    auks --config /conf/auks.conf --remove --uid 1234
}

@test "Add/Get admin cred as user user" {
    kinit -k -t /user.keytab user
    kinit -k -t /admin.keytab -c /tmp/krb5cc_bats admin
    auks -f /conf/auks.conf -p
    run auks -f /conf/auks.conf -a -C /tmp/krb5cc_bats
    [ "$status" -ne 0 ]
    rm /tmp/krb5cc_bats
    run auks -f /conf/auks.conf -g -u 4321 -C /tmp/krb5cc_bats
    [ "$status" -ne 0 ]
    run bash -c 'klist -fanc /tmp/krb5cc_bats | grep admin'
    [ "$status" -eq 1 ]
    run test -f /run/krb5cc_bats
    [ "$status" -eq 1 ]
}

@test "Cannot dump creds as regular user" {
    kinit -k -t /user.keytab user
    run auks -f /conf/auks.conf -D
    [ "$status" -ne 0 ]
    run auks -f /conf/auks.conf --dump
    [ "$status" -ne 0 ]
}

@test "Able to dump creds as a admin user" {
    kinit -k -t /admin.keytab admin
    auks -f /conf/auks.conf -D
    auks -f /conf/auks.conf --dump
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

@test "Auks client is able to dump a credential to a local file" {
    run bash -c "rm /tmp/auks_cred"
    kinit -k -t /admin.keytab admin
    auks -f /conf/auks.conf --add
    auks -f /conf/auks.conf --send --uid 4321 > /tmp/auks_cred
    test -s /tmp/auks_cred
    auks -f /conf/auks.conf --remove --uid 4321
    rm /tmp/auks_cred
}

@test "Auks client is able to receive a credential from a pipe" {
    run bash -c "rm /tmp/auks_cred"
    kinit -k -t /admin.keytab admin
    run test -e /tmp/auks_cred
    [ "$status" -eq 1 ]
    auks -f /conf/auks.conf --add
    auks -f /conf/auks.conf --send -u 4321 | auks -f /conf/auks.conf --receive -C /tmp/auks_cred
    auks -f /conf/auks.conf --remove --uid 4321
    run bash -c "auks -f /conf/auks.conf --dump | grep -w admin@EXAMPLE.COM"
    [ "$status" -eq 1 ]
    klist -fnac /tmp/auks_cred
    rm /tmp/auks_cred
}

@test "Auks client is able to receive a credential with no krb5 tkt" {
    run bash -c "rm /tmp/auks_cred"
    kinit -k -t /admin.keytab admin
    run test -e /tmp/auks_cred
    [ "$status" -eq 1 ]
    auks -f /conf/auks.conf --add
    auks -f /conf/auks.conf --send -u 4321 > /tmp/auks_msg
    test -s /tmp/auks_msg
    auks -f /conf/auks.conf --remove --uid 4321
    KRB5CCNAME=/dev/null auks -f /conf/auks.conf --receive -C /tmp/auks_cred < /tmp/auks_msg
    klist -fnac /tmp/auks_cred
    rm /tmp/auks_msg /tmp/auks_cred
}

@test "Auks does not fail when a cross-realm is not available" {
    kinit -k -t /admin.keytab admin
    sed -i.orig 's/CrossRealm.*;$/CrossRealm = \"CROSS.EXAMPLE.COM\";/' /conf/auks.conf
    auks -f /conf/auks.conf --add
    mv /conf/auks.conf.orig /conf/auks.conf
}
