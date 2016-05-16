for i in {3..30}; do 
	./nim $i > nim.out
	./nimNegamax $i > nimNegamax.out
	cmp -s nim.out nimNegamax.out
	if [ $? != 0 ]; then
		echo "FAIL: n = $i"
	fi
done
rm nim.out
rm nimNegamax.out

