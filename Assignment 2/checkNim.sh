for i in {3..30}; do 
	./nim $i > nim.txt
	./nim2 $i > nim2.txt
	cmp -s nim.txt nim2.txt
	if [ $? == 0 ]; then
		echo "OK"
	else
		echo "FAIL"
	fi
done

