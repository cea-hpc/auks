FROM quay.io/centos/centos:stream8

RUN yum -y update; yum install -y autoconf automake libtool\
        libtirpc libtirpc-devel krb5-devel krb5-workstation\
        make gcc diffutils file

RUN dnf config-manager --set-enabled powertools; dnf install -y epel-release
RUN dnf install -y bats

COPY . auks

WORKDIR auks

RUN autoreconf -fvi && ./configure && make clean && make && make install

COPY fixtures/krb5.conf /etc/krb5.conf
COPY fixtures/auks* /conf/

COPY fixtures/entrypoint_*.sh /usr/local/bin
RUN chmod 0750 /usr/local/bin/entrypoint_*.sh

RUN mkdir /var/cache/auks

RUN useradd -M -u 1234 user; useradd -M -u 4321 admin

EXPOSE 12345/tcp
