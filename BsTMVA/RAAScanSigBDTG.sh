i=0
f=10
step=0.1

RAA1=0.5
RAA2=0.5
RAA3=0.5

while [ $i -lt $f ]
do

	echo "i = " $i
	echo "RAA1 = " $RAA1
	echo "RAA2 = " $RAA2
	echo "RAA3 = " $RAA3
	

	RAA1=$(echo "$RAA1 + $step" | bc)
	RAA2=$(echo "$RAA2 + $step" | bc)
	RAA3=$(echo "$RAA3 + $step" | bc)
	

	
	source doTMVARAAScanBDTG.sh $RAA1 $RAA2 $RAA3



	i=$(($i+1))



done
