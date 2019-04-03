AM_CFLAGS = \
	$(WARN_CFLAGS)	\
	-I./include		\
	-I${includedir}

libnx_uds_la_LTLIBRARIES = libnx_uds.la
libnx_uds_ladir = ${libdir}
libnx_uds_la_LDFLAGS = -L${libdir} -lm -lpthread

libnx_uds_la_SOURCES = \
	CNX_LocalSocket.cpp \
	NX_UDS_UDP_Server.cpp \
	NX_UDS_UDP_Client.cpp

libnx_uds_includedir = ${includedir}
libnx_uds_include_HEADERS = \
	NX_UDS_UDP_Client.h \
	NX_UDS_UDP_Server.h \
	CNX_LocalSocket.h
