#!/bin/sh

#ONLY RUN FROM REPOSITORY ROOT

testcase_stdout() {
	./builddir/42sh $@ 1> tests/42sh_output
	sh $@ 1> tests/sh_output
	if cmp -s -- tests/42sh_output tests/sh_output; then
		echo passed'!'
	fi

	diff -u --color tests/42sh_output tests/sh_output
}

#Simple commands
echo ----Simple commands----
#Test 1: simple echo
echo test1
testcase_stdout -c "echo foo;"

#Test 2: simple echo semi-colon
echo test2
testcase_stdout -c "echo foo" 

#Test 3: simple ls
echo test3
testcase_stdout -c "ls src"
