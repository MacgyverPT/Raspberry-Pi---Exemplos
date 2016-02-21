for i in 0 1 2 ; do gpio mode $i out; done
for i in 0 1 2 ; do gpio write $i 1; done
for i in 0 1 2 ; do gpio write $i 0; done
