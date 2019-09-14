echo $0" :0: gs before foo";
echo $1" :1: gs before foo";
echo $2" :2: gs before foo";
echo $3" :3: gs before foo";
echo $4" :4: gs before foo";
echo $5" :5: gs before foo";

function	foo()
{
	echo $0" :0: foo";
	echo $1" :1: foo";
	echo $2" :2: foo";
	echo $3" :3: foo";
	echo $4" :4: foo";
	echo $5" :5: foo";
}

foo f1 f2 f3 f4 f5;

echo $0" :0: gs after foo";
echo $1" :1: gs after foo";
echo $2" :2: gs after foo";
echo $3" :3: gs after foo";
echo $4" :4: gs after foo";
echo $5" :5: gs after foo";