for i in 0 1 2 3 4 ; do gpio mode $i out; done
for i in 0 1 2 3 4 ; do gpio write $i 1; done
for i in 0 1 2 3 4 ; do gpio write $i 0; done
