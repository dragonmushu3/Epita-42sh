#!/bin/sh

#ONLY RUN FROM REPOSITORY ROOT

#Simple commands
echo ----Commands----

./builddir/42sh -c "echo foo; echo hey; echo lala" 1> tests/42sh_output
sh -c "echo foo; echo hey; echo lala" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 1 passed'!'
else
    echo test 1 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

echo ----Echo----

./builddir/42sh -c "echo -n -n -n -n -n -n -n -n foo man" 1> tests/42sh_output
sh -c "echo -n -n -n -n -n -n -n -n foo man" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 1 passed'!'
else
    echo test 1 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

./builddir/42sh -c "echo -n -n -e -n -n -n -e -n foo man" 1> tests/42sh_output
sh -c "echo -n -n -e -n -n -n -e -n foo man" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 2 passed'!'
else
    echo test 2 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

./builddir/42sh -c "echo -n -n -e -n -n -n -e -n /n" 1> tests/42sh_output
sh -c "echo -n -n -e -n -n -n -e -n /n" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 3 passed'!'
else
    echo test 3 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

./builddir/42sh -c "echo -n //n" 1> tests/42sh_output
sh -c "echo -n //n" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 4 passed'!'
else
    echo test 4 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

echo ----IF----

./builddir/42sh -c "if false;then echo hey;else echo lala; fi" 1> tests/42sh_output
sh -c "if false;then echo hey;else echo lala; fi" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 1 passed'!'
else
    echo test 1 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

./builddir/42sh -c "if false;then echo hey;elif false; then echo what;elif true; then echo wassup; elif true; then echo bruh; else echo nope;fi" 1> tests/42sh_output
sh -c "if false;then echo hey;elif false; then echo what;elif true; then echo wassup; elif true; then echo bruh; else echo nope;fi" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 2 passed'!'
else
    echo test 2 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

./builddir/42sh -c "if true; then echo foo; else echo nope;fi" 1> tests/42sh_output
sh -c "if true; then echo foo; else echo nope;fi" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 3 passed'!'
else
    echo test 3 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

echo ----IF and echo in a list----
./builddir/42sh -c "echo whatever;if true; then echo foo; else echo nope;fi" 1> tests/42sh_output
sh -c "echo whatever;if true; then echo foo; else echo nope;fi" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 1 passed'!'
else
    echo test 1 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

./builddir/42sh -c "echo whatever;if true; then echo foo; else echo nope;fi;echo after;" 1> tests/42sh_output
sh -c "echo whatever;if true; then echo foo; else echo nope;fi;echo after;" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 2 passed'!'
else
    echo test 2 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi

echo ----unknown commands----
./builddir/42sh -c "e; hey;" 1> tests/42sh_output
sh -c "e; hey" 1> tests/sh_output
if cmp -s -- tests/42sh_output tests/sh_output; then
    echo test 1 passed'!'
else
    echo test 1 failed'!'
    diff -u --color tests/42sh_output tests/sh_output
fi
