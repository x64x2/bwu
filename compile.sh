#!/usr/bin/env bash

main () {
    cd $(dirname $0)

    case "$1" in
        clean) _reset ;;
        run)   _comp && _comp_testprog && _run ;;
        debug) _comp && _comp_testprog && _debug ;;
        install) _install ;;
        *)     _comp  ;;
    esac
}

_install () {
    cp -r include/ray /usr/include/
    cp bin/libray.dll /usr/lib/
}

_reset () {
    rm -rf bin
}

_comp () {
    [ -d 'bin' ] || meson bin
    meson compile -C bin
}

_comp_testprog () {
    cd testprog
    gcc -L test.c -lm `sdl2-config --cflags --libs` -ray -I../include -g -o ../bin/game || return 1
    cd ..
}

_run () {
    cd testprog
    LD_LIBRARY_PATH=../bin/:$LD_LIBRARY_PATH ./game
    cd ..
}

_debug () {
    cd testprog
    LD_LIBRARY_PATH=../bin/:$LD_LIBRARY_PATH gdb game
    cd ..
}

main "$@"
