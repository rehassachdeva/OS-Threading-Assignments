#!/bin/bash

i=0

mergeSort() {
        
        local num=`wc -l < $1`

        local num2=$((num/3))
        
        if [ $((num%3)) -ne 0 ]
        then
                local num2=$((num2+2))
        fi

        split -l $num2 $1
        
        if [ -e "xaa" ] 
        then
                local f1="$i"
                mv "xaa" "$f1"
                i=$((i+1))
        fi
        
        if [ -e "xab" ]
        then
                local f2="$i"
                mv "xab" "$f2"
                i=$((i+1))
        fi
        
        if [ -e "xac" ]
        then
                local f3="$i"
                mv "xac" "$f3"
                i=$((i+1))
        fi
        
        if [ `du $f1 | cut -f 1` -le 10240 ]
        then
                sort -no "$f1" "$f1"
        else
                mergeSort $f1
        fi

        if [ `du $f2 | cut -f 1` -le 10240 ] 
        then
                sort -no "$f2" "$f2"
        else
                mergeSort $f2
        fi  
        
        if [ `du $f3 | cut -f 1` -le 10240 ] 
        then
                sort -no "$f3" "$f3"
        else
                mergeSort $f3
        fi  
        
        sort -nm "$f1" "$f2" "$f3"> "$1"
}

mergeSort $1
        
