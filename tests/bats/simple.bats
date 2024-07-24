#!/usr/bin/env bats

load "bats-support/load"
load "bats-assert/load"

function setup(){
	 ADMIN_KT="/krb5-keytabs/auks_admin.keytab"
	 ADMIN_PRINCIPAL="auks_admin@EXAMPLE.COM"
	 ADMIN_UID=2000
	 USER_KT="/krb5-keytabs/auks_user.keytab"
	 USER_PRINCIPAL="auks_user@EXAMPLE.COM"
	 USER_UID=2001
	 GUEST_KT="/krb5-keytabs/auks_guest.keytab"
	 GUEST_PRINCIPAL="auks_guest@EXAMPLE.COM"
	 GUEST_UID=2002
	 UNKNOWN_USER_KT="/krb5-keytabs/auks_unknown_user.keytab"
	 UNKNOWN_USER_PRINCIPAL="auks_unknown_user@EXAMPLE.COM"
	 PATH="/auks/bin:${PATH}"
	 AUKS_CONFIG_FILE="/auks/etc/auks_client.conf"
	 unset KRB5_TRACE
}

function teardown() {
    test -e "${AUKS_CONFIG_FILE}".orig && mv "${AUKS_CONFIG_FILE}".orig "${AUKS_CONFIG_FILE}"
    # Flush everything from auks
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" --remove --uid "${ADMIN_UID}" &>/dev/null;
    auks -f "${AUKS_CONFIG_FILE}" --remove --uid "${USER_UID}" &>/dev/null;
    test -e /tmp/renewed && rm /tmp/renewed
    kdestroy || true
}

@test "Ping as user" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -p
    auks -f "${AUKS_CONFIG_FILE}" --ping
}

@test "Ping as regular user" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -p
    auks -f "${AUKS_CONFIG_FILE}" --ping
}

@test "Complete Ccache life cyle as regular user (short options)" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -p
    run auks -f "${AUKS_CONFIG_FILE}" -g -u "${USER_UID}"
    [ "$status" -ne 0 ]
    auks -f "${AUKS_CONFIG_FILE}" -a
    auks -f "${AUKS_CONFIG_FILE}" -g -u "${USER_UID}"
    sleep 1
    auks -f "${AUKS_CONFIG_FILE}" -R once
    test -e /tmp/renewed; rm /tmp/renewed
    auks -f "${AUKS_CONFIG_FILE}" -r -u "${USER_UID}"
    run auks -f "${AUKS_CONFIG_FILE}" -g -u "${USER_UID}"
    [ "$status" -ne 0 ]
}

@test "Complete Ccache life cyle as regular user (long options)" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    auks --config "${AUKS_CONFIG_FILE}" --ping
    run auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    [ "$status" -ne 0 ]
    auks --config "${AUKS_CONFIG_FILE}" --add
    auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    sleep 1
    auks -f "${AUKS_CONFIG_FILE}" --renew once
    test -e /tmp/renewed; rm /tmp/renewed
    auks --config "${AUKS_CONFIG_FILE}" --remove --uid "${USER_UID}"
    run auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    [ "$status" -ne 0 ]
}

@test "Ping/Add/Get own cred as admin user" {
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -p
    auks -f "${AUKS_CONFIG_FILE}" -a
    auks -f "${AUKS_CONFIG_FILE}" -g -u "${ADMIN_UID}"
}

@test "Add/Get user cred as admin user (short options)" {
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    kinit -k -t "${USER_KT}" -c /tmp/krb5cc_bats "${USER_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -p
    auks -f "${AUKS_CONFIG_FILE}" -a -C /tmp/krb5cc_bats
    rm /tmp/krb5cc_bats
    auks -f "${AUKS_CONFIG_FILE}" -g -u "${USER_UID}" -C /tmp/krb5cc_bats
    auks -f "${AUKS_CONFIG_FILE}" -r -u "${USER_UID}"
}

@test "Add/Get user cred as admin user (long options)" {
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    kinit -k -t "${USER_KT}" -c /tmp/krb5cc_bats "${USER_PRINCIPAL}"
    auks --config "${AUKS_CONFIG_FILE}" --ping
    auks --config "${AUKS_CONFIG_FILE}" --add --ccache /tmp/krb5cc_bats
    rm /tmp/krb5cc_bats
    auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}" --ccache /tmp/krb5cc_bats
    auks --config "${AUKS_CONFIG_FILE}" --remove --uid "${USER_UID}"
}

@test "Add/Get admin cred as user user" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    kinit -k -t "${ADMIN_KT}" -c /tmp/krb5cc_bats "${ADMIN_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -p
    run auks -f "${AUKS_CONFIG_FILE}" -a -C /tmp/krb5cc_bats
    [ "$status" -ne 0 ]
    rm /tmp/krb5cc_bats
    run auks -f "${AUKS_CONFIG_FILE}" -g -u "${ADMIN_UID}" -C /tmp/krb5cc_bats
    [ "$status" -ne 0 ]
    run bash -c 'klist -fanc /tmp/krb5cc_bats | grep admin'
    [ "$status" -eq 1 ]
    run test -f /run/krb5cc_bats
    [ "$status" -eq 1 ]
}

@test "Cannot dump creds as regular user" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    run auks -f "${AUKS_CONFIG_FILE}" -D
    [ "$status" -ne 0 ]
    run auks -f "${AUKS_CONFIG_FILE}" --dump
    [ "$status" -ne 0 ]
}

@test "Able to dump creds as a admin user" {
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -D
    auks -f "${AUKS_CONFIG_FILE}" --dump
}

@test "Able to remove my own credential" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -a -u "${USER_UID}"
    auks -f "${AUKS_CONFIG_FILE}" -r -u "${USER_UID}"
    run auks -f "${AUKS_CONFIG_FILE}" -D
    [ "$status" -ne 0 ]
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -D
    refute_output -p "${USER_PRINCIPAL}"
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -a -u "${ADMIN_UID}"
    auks -f "${AUKS_CONFIG_FILE}" -r -u "${ADMIN_UID}"
    auks -f "${AUKS_CONFIG_FILE}" -D
    refute_output -p "${ADMIN_PRINCIPAL}"
}

@test "Admins are able to remove all credentials" {
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    kinit -k -t "${USER_KT}" -c /tmp/krb5cc_bats "${USER_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -a -u "${USER_UID}" -C /tmp/krb5cc_bats
    auks -f "${AUKS_CONFIG_FILE}" -a -u "${ADMIN_UID}"
    run auks -f "${AUKS_CONFIG_FILE}" -D
    assert_output -p "${USER_PRINCIPAL}"
    assert_output -p "${ADMIN_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" -r -u "${USER_UID}"
    auks -f "${AUKS_CONFIG_FILE}" -r -u "${ADMIN_UID}"
    run auks -f "${AUKS_CONFIG_FILE}" -D
    refute_output -p "${USER_PRINCIPAL}"
    refute_output -p "${ADMIN_PRINCIPAL}"
}

@test "Auks client is able to dump a credential to a local file" {
    run bash -c "rm /tmp/auks_cred"
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    auks -f "${AUKS_CONFIG_FILE}" --add
    auks -f "${AUKS_CONFIG_FILE}" --send --uid "${ADMIN_UID}" > /tmp/auks_cred
    test -s /tmp/auks_cred
    auks -f "${AUKS_CONFIG_FILE}" --remove --uid "${ADMIN_UID}"
    rm /tmp/auks_cred
}

@test "Auks client is able to receive a credential from a pipe" {
    run bash -c "rm /tmp/auks_cred"
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    run test -e /tmp/auks_cred
    [ "$status" -eq 1 ]
    auks -f "${AUKS_CONFIG_FILE}" --add
    auks -f "${AUKS_CONFIG_FILE}" --send -u "${ADMIN_UID}" | auks -f "${AUKS_CONFIG_FILE}" --receive -C /tmp/auks_cred
    auks -f "${AUKS_CONFIG_FILE}" --remove --uid "${ADMIN_UID}"
    run bash -c "auks -f "${AUKS_CONFIG_FILE}" --dump | grep -w ${ADMIN_PRINCIPAL}"
    [ "$status" -eq 1 ]
    klist -fnac /tmp/auks_cred
    rm /tmp/auks_cred
}

@test "Auks client is able to receive a credential with no krb5 tkt" {
    run bash -c "rm /tmp/auks_cred"
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    run test -e /tmp/auks_cred
    [ "$status" -eq 1 ]
    auks -f "${AUKS_CONFIG_FILE}" --add
    auks -f "${AUKS_CONFIG_FILE}" --send -u "${ADMIN_UID}" > /tmp/auks_msg
    test -s /tmp/auks_msg
    auks -f "${AUKS_CONFIG_FILE}" --remove --uid "${ADMIN_UID}"
    KRB5CCNAME=/dev/null auks -f "${AUKS_CONFIG_FILE}" --receive -C /tmp/auks_cred < /tmp/auks_msg
    klist -fnac /tmp/auks_cred
    rm /tmp/auks_msg /tmp/auks_cred
}

@test "Auks does not fail when a cross-realm is not available" {
    kinit -k -t "${ADMIN_KT}" "${ADMIN_PRINCIPAL}"
    sed -i.orig 's/CrossRealm.*;$/CrossRealm = \"CROSS.EXAMPLE.COM\";/' "${AUKS_CONFIG_FILE}"
    auks -f "${AUKS_CONFIG_FILE}" --add
    mv "${AUKS_CONFIG_FILE}".orig "${AUKS_CONFIG_FILE}"
}

@test "Fail when renewal script fails" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    auks --config "${AUKS_CONFIG_FILE}" --ping
    sed -i.orig 's/HelperScript.*/HelperScript=\"\/bin\/false\";/' "${AUKS_CONFIG_FILE}"
    run auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    [ "$status" -ne 0 ]
    auks --config "${AUKS_CONFIG_FILE}" --add
    auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    sleep 1
    run auks -f "${AUKS_CONFIG_FILE}" --renew once -vvv -ddd
    [ "$status" -ne 0 ]
    test -e /tmp/renewed && rm /tmp/renewed
    auks --config "${AUKS_CONFIG_FILE}" --remove --uid "${USER_UID}"
    run auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    [ "$status" -ne 0 ]
}

@test "Not fail when renewal script is not executable" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    auks --config "${AUKS_CONFIG_FILE}" --ping
    sed -i.orig 's/HelperScript.*/HelperScript=\"\/not\/existing\/script\";/' "${AUKS_CONFIG_FILE}"
    run auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    [ "$status" -ne 0 ]
    auks --config "${AUKS_CONFIG_FILE}" --add
    auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    sleep 1
    auks -f "${AUKS_CONFIG_FILE}" --renew once -vvv -ddd
    test -e /tmp/renewed && rm /tmp/renewed
    auks --config "${AUKS_CONFIG_FILE}" --remove --uid "${USER_UID}"
    run auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    [ "$status" -ne 0 ]
}

@test "Not fail when renewal script is not set" {
    kinit -k -t "${USER_KT}" "${USER_PRINCIPAL}"
    auks --config "${AUKS_CONFIG_FILE}" --ping
    sed -i.orig 's/HelperScript.*/HelperScript=\"\";/' "${AUKS_CONFIG_FILE}"
    run auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    [ "$status" -ne 0 ]
    auks --config "${AUKS_CONFIG_FILE}" --add
    auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    sleep 1
    auks -f "${AUKS_CONFIG_FILE}" --renew once -vvv -ddd
    test -e /tmp/renewed && rm /tmp/renewed
    auks --config "${AUKS_CONFIG_FILE}" --remove --uid "${USER_UID}"
    run auks --config "${AUKS_CONFIG_FILE}" --get --uid "${USER_UID}"
    [ "$status" -ne 0 ]
}

@test "Fail to add a cred when the user is not known" {
    kinit -k -t "${UNKNOWN_USER_KT}" "${UNKNOWN_USER_PRINCIPAL}"
    run auks --config "${AUKS_CONFIG_FILE}" --add
    [ "$status" -ne 0 ]
}
