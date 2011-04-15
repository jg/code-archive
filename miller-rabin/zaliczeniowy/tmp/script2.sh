

for i in `seq 1 100`
do
	if [ `./a.out -p "$i"` == "1" ]
	then
		echo "$i"
	fi

done
