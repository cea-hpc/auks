#!/bin/bash -xe

AUKS_PRIV_SYSLOG_PRIO="none" AUKS_PRIV_PRINC="${KRB5_PRINCIPAL}" AUKS_PRIV_KEYTAB="${KRB5_KTNAME}" /auks/sbin/aukspriv -v &

/auks/sbin/auksd -F -f /auks/etc/auks.conf $@
