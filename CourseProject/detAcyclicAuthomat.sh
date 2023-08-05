#!/bin/bash


input_string=$1

result=$(c++ CourseProject.cpp $input_string)

if [ $result == "True" ]; then
    echo "Input string is accepted by the DFA"
else
    echo "Input string is not accepted by the DFA"
fi
