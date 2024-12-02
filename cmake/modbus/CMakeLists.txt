cmake_minimum_required(VERSION 3.14)

project(modbus LANGUAGES C VERSION 3.1.11)

include(CheckIncludeFile)
check_include_file(arpa/inet.h HAVE_ARPA_INET_H)
check_include_file(byteswap.h HAVE_BYTESWAP_H)
check_include_file(errno.h HAVE_ERRNO_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(linux/serial.h HAVE_LINUX_SERIAL_H)
check_include_file(netdb.h HAVE_NETDB_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(netinet/ip.h HAVE_NETINET_IP_H)
check_include_file(netinet/tcp.h HAVE_NETINET_TCP_H)
check_include_file(sys/ioctl.h HAVE_SYS_IOCTL_H)
check_include_file(sys/params.h HAVE_SYS_PARAMS_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(termios.h HAVE_TERMIOS_H)
check_include_file(time.h HAVE_TIME_H)
check_include_file(unistd.h HAVE_UNISTD_H)

include(CheckFunctionExists)
check_function_exists(accept4 HAVE_ACCEPT4)
check_function_exists(gai_strerror HAVE_GAI_STRERROR)
check_function_exists(getaddrinfo HAVE_GETADDRINFO)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(inet_pton HAVE_INET_PTON)
check_function_exists(inet_ntop HAVE_INET_NTOP)
check_function_exists(select HAVE_SELECT)
check_function_exists(socket HAVE_SOCKET)
check_function_exists(strerror HAVE_STRERROR)
check_function_exists(strlcpy HAVE_STRLCPY)

include(CheckSymbolExists)
check_symbol_exists(TIOCSRS485 "sys/ioctl.h" HAVE_DECL_TIOCSRS485)
check_symbol_exists(TIOCM_RTS "sys/ioctl.h" HAVE_DECL_TIOCM_RTS)

set(modbus_SOURCES
    src/modbus-data.c
    src/modbus-rtu.c
    src/modbus-tcp.c
    src/modbus.c
)

add_library(modbus STATIC ${modbus_SOURCES})

target_include_directories(modbus
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR} src
)

set_target_properties(modbus PROPERTIES
    VERSION 6.0.1
)

target_compile_definitions(modbus PRIVATE
    HAVE_CONFIG_H
    SYSCONFDIR="/etc"
    LIBEXECDIR="/libexec"
)

target_compile_options(modbus PRIVATE
    -include ${CMAKE_CURRENT_BINARY_DIR}/config.h
    -Wall -Wmissing-declarations -Wmissing-prototypes
    -Wnested-externs -Wpointer-arith
    -Wsign-compare -Wchar-subscripts -Wstrict-prototypes
    -Wshadow -Wformat-security
)

set(MODBUS_NAME "${PROJECT_NAME}")
set(MODBUS_VERSION "${PROJECT_VERSION}")
set(MODBUS_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(MODBUS_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(MODBUS_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/modbus-version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/modbus-version.h)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/modbus)
file(COPY
    src/modbus.h
    src/modbus-rtu.h
    src/modbus-tcp.h
    src/modbus-version.h
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/modbus)
