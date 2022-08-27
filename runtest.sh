#!/bin/bash

# Resets the output folder
rm -rf /workspaces/FinalProject22/.output
mkdir /workspaces/FinalProject22/.output

# Compiles the program as a release build
gcc /workspaces/FinalProject22/main.c -O2 -Wall -Werror -o /workspaces/FinalProject22/build/FinalProject22 > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Runs tests
echo "Running test 1..."
/workspaces/FinalProject22/build/FinalProject22 < /workspaces/FinalProject22/.tests/test1.txt > /workspaces/FinalProject22/.output/test1o.txt
diff /workspaces/FinalProject22/.output/test1o.txt /workspaces/FinalProject22/.tests/test1.output.txt > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Test 1 passed"
else
    echo "Test 1 failed"
fi

echo "Running test 2..."
/workspaces/FinalProject22/build/FinalProject22 < /workspaces/FinalProject22/.tests/test2.txt > /workspaces/FinalProject22/.output/test2o.txt
diff /workspaces/FinalProject22/.output/test2o.txt /workspaces/FinalProject22/.tests/test2.output.txt > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Test 2 passed"
else
    echo "Test 2 failed"
fi

echo "Running test 3..."
/workspaces/FinalProject22/build/FinalProject22 < /workspaces/FinalProject22/.tests/test3.txt > /workspaces/FinalProject22/.output/test3o.txt
diff /workspaces/FinalProject22/.output/test3o.txt /workspaces/FinalProject22/.tests/test3.output.txt > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Test 3 passed"
else
    echo "Test 3 failed"
fi