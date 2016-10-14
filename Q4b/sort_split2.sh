#!/bin/bash

i=0

mergeSort() { 
        if [ `du -b "$1" | cut -f 1` -le 100000000 ]
        then
                sort -no "$1" "$1"
        else
                local num=`wc -l < $1`
                local num2=$((($num+1)/2))
                split -l $num2 "$1"
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

                if [ -e "$f1" ]
                then 
                        mergeSort $f1
                fi
                if [ -e "$f2" ]
                then 
                        mergeSort $f2
                fi
                
                if [ -e "$f2" ]
                then
                        sort -nm "$f1" "$f2" > "$1"
                elif [ -e "$f1" ]
                then
                        sort -nm "$f1" > "$1"
                fi
        fi
}
mergeSort $1
