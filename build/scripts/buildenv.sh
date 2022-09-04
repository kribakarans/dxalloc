#!/bin/bash

set -eu

source ./build/scripts/buildspecs.in

echo "${BOLD}Building ${PACKAGE} ${DISTNAME} build environment ...${NC}"

if [ "$PLATFORM" == "TERMUX" ] || [ $(id -u) -eq 0 ]; then
	SUDO=""
else
	SUDO="sudo"
fi

if [ "$PLATFORM" == "TERMUX" ]; then
	PACKAGES+="" #Libraries
else
	PACKAGES+="libbacktrace" #Libraries
fi

$SUDO apt update && \
$SUDO apt install $PACKAGES

exit $?

#EOF
