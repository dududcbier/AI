for i in {3..30}; do 
	./nim $i > nim.txt
	./nimNegamax $i > nimNegamax.txt
	cmp -s nim.txt nimNegamax.txt
	if [ $? == 0 ]; then
		echo "OK"
	else
		echo "FAIL"
	fi
done
rm nim.txt
rm nimNegamax.txt

