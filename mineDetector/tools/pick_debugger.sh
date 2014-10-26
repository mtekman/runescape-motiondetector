#!/bin/bash

bin=../mineDetector-build/mineDetector

for img in $(ls test_imgs/*.jpg); do
	res=$($bin $img $img --debug | awk '{print $2}')
	echo -e "$img\t$res"
done
