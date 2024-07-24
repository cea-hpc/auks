FROM quay.io/almalinuxorg/almalinux:8 as auks_server

RUN dnf install -y epel-release; crb enable; dnf makecache
RUN dnf -y update; dnf install -y autoconf automake libtool\
        libtirpc libtirpc-devel krb5-devel krb5-workstation kstart\
        make gcc diffutils file strace gdb &&\
	dnf clean all

COPY . /auks_src/

WORKDIR /auks_src/

RUN autoreconf -fvi && ./configure --prefix=/auks/ && make clean && make -j 8 && make -j 8 install && rm -Rf /auks_src
WORKDIR /auks

RUN mkdir /var/cache/auks

VOLUME /auks/etc
EXPOSE 12345/tcp
COPY entrypoint.sh /entrypoint.sh
RUN chmod a+x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["-v"]

FROM auks_server AS auks_test

RUN dnf install -y bats && dnf clean all
ENTRYPOINT ["bash"]
