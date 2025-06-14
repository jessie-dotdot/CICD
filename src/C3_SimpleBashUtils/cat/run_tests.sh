#!/bin/bash

GREEN='\e[32m'
NC='\e[0m'
RED='\e[31m'
EXE=s21_cat.c

printf "${GREEN}-----RUNNING TESTS-----${NC}\n"

cat test.txt >a
gcc $EXE -lm && ./a.out test.txt >b
result=$(diff a b)
failed=0
i=1

# TEST 1
if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

((i++))

# TEST 2
cat -b test1.txt >a
gcc $EXE -lm && ./a.out -b test1.txt >b
result=$(diff a b)

if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

((i++))

# TEST 4
cat -s test3.txt >a
gcc $EXE -lm && ./a.out -s test3.txt >b
result=$(diff a b)

if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

((i++))

# TEST 5
cat -t test.txt >a
gcc $EXE -lm && ./a.out -t test.txt >b
result=$(diff a b)

if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

((i++))

# TEST 6
cat -e test.txt >a
gcc $EXE -lm && ./a.out -e test.txt >b
result=$(diff a b)

if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

((i++))

# TEST 7
FILE=ASCII.txt
FLAGS=
cat $FILE >a
gcc $EXE -lm && ./a.out $FILE >b
result=$(diff a b)

if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

((i++))

# TEST 8
FILE=ASCII.txt
FLAGS=-b
cat $FLAGS $FILE >a
gcc $EXE -lm && ./a.out $FLAGS $FILE >b
result=$(diff a b)

if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

((i++))

# TEST 9
FILE=ASCII.txt
FLAGS=-n
cat $FLAGS $FILE >a
gcc $EXE -lm && ./a.out $FLAGS $FILE >b
result=$(diff a b)

if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

((i++))

# TEST 10
FILE=ASCII.txt
FLAGS=-s
cat $FLAGS $FILE >a
gcc $EXE -lm && ./a.out $FLAGS $FILE >b
result=$(diff a b)

if [ $? -eq 0 ]; then
	printf " TEST #$i ${GREEN}PASSED${NC}\n"
else
	printf " TEST #$i ${RED}FAILED${NC}\n"
	printf "$result"
	((failed++))
fi

printf " ${GREEN}-----DONE[$((i - failed))/$((i))]-----${NC}\n"

rm a.out a b
