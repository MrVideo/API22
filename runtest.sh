#!/bin/zsh

# Resets the output folder
rm -rf $HOME/GitHub/FinalProject22/output
mkdir $HOME/GitHub/FinalProject22/output

# Compiles the program as a release build
gcc $HOME/GitHub/FinalProject22/main.c -O2 -Wall -Werror -o $HOME/GitHub/FinalProject22/build/FinalProject22 > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Runs tests
echo "Running test 'Insert'..."
$HOME/GitHub/FinalProject22/build/FinalProject22 < $HOME/GitHub/FinalProject22/tests/insert.txt > $HOME/GitHub/FinalProject22/output/insertout.txt
diff $HOME/GitHub/FinalProject22/output/insertout.txt $HOME/GitHub/FinalProject22/tests/insert.output.txt > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Test 'Insert' passed"
else
    echo "Test 'Insert' failed"
fi

echo "Running test 'Slide'..."
$HOME/GitHub/FinalProject22/build/FinalProject22 < $HOME/GitHub/FinalProject22/tests/slide.txt > $HOME/GitHub/FinalProject22/output/slideout.txt
diff $HOME/GitHub/FinalProject22/output/slideout.txt $HOME/GitHub/FinalProject22/tests/slide.output.txt > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Test 'Slide' passed"
else
    echo "Test 'Slide' failed"
fi

echo "Running test 1..."
$HOME/GitHub/FinalProject22/build/FinalProject22 < $HOME/GitHub/FinalProject22/tests/test1.txt > $HOME/GitHub/FinalProject22/output/test1o.txt
diff $HOME/GitHub/FinalProject22/output/test1o.txt $HOME/GitHub/FinalProject22/tests/test1.output.txt > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Test 1 passed"
else
    echo "Test 1 failed"
fi

echo "Running test 2..."
$HOME/GitHub/FinalProject22/build/FinalProject22 < $HOME/GitHub/FinalProject22/tests/test2.txt > $HOME/GitHub/FinalProject22/output/test2o.txt
diff $HOME/GitHub/FinalProject22/output/test2o.txt $HOME/GitHub/FinalProject22/tests/test2.output.txt > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Test 2 passed"
else
    echo "Test 2 failed"
fi

echo "Running test 3..."
$HOME/GitHub/FinalProject22/build/FinalProject22 < $HOME/GitHub/FinalProject22/tests/test3.txt > $HOME/GitHub/FinalProject22/output/test3o.txt
diff $HOME/GitHub/FinalProject22/output/test3o.txt $HOME/GitHub/FinalProject22/tests/test3.output.txt > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Test 3 passed"
else
    echo "Test 3 failed"
fi