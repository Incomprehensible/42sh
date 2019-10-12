PS="(42lib)42sh "
PRAY_TEXT_RUS="Отче наш, сущий на небесах! "
PRAY_TEXT_RUS+="Да святится имя твое, "
PRAY_TEXT_RUS+="Да приидет Царствие твое, "
PRAY_TEXT_RUS+="Да будет воля твоя, и на земле, как на небе! "
PRAY_TEXT_RUS+="Хлеб наш насущный дай нам на сей день, "
PRAY_TEXT_RUS+="и прости нам долги наши, как и мы прощаем должникам нашим, "
PRAY_TEXT_RUS+="и не введи нас в искушение, но избавь нас от лукавого. "

PRAY_TEXT_ENG="Our Father, who art in heaven"
PRAY_TEXT_ENG+="Hallowed be thy Name,"
PRAY_TEXT_ENG+="Thy kingdom come,"
PRAY_TEXT_ENG+="Thy will be done, on earth as it is in heaven."
PRAY_TEXT_ENG+="Give us this day our daily bread"
PRAY_TEXT_ENG+="And forgive us our trespasses, as we forgive those who trespass against us."
PRAY_TEXT_ENG+="And lead us not into temptation, but deliver us from evil."
PRAY_TEXT_ENG+="For thine is"
PRAY_TEXT_ENG+="The kingdom,"
PRAY_TEXT_ENG+="And the power,"
PRAY_TEXT_ENG+="And the glory,"
PRAY_TEXT_ENG+="For ever and ever."
PRAY_TEXT_ENG+="Amen."

PRAY_TEXT_ORIG="GOD SAVE THE QUEEN!!!"

function ps_my_cpids
{
	IN_shsh=$1
	if [ -z $IN_shsh ] ; then IN_shsh=$$; fi
	P_PID_shsh=$(ps -p $IN_shsh -o pid,pgid,state,comm | grep -v PID)
	if [ -n "$P_PID_shsh" ] ; then
		ps -p $IN_shsh -o pid,pgid,state,comm
		ps_children_of_cpid $IN_shsh
	else
		echo NO SUCH PID LIKE $IN_shsh
	fi
}

function ps_children_of_cpid
{
	C_PIDS_shsh=$(ps -ax -o ppid,pgid,pid,state,comm | grep -E "^\s*$1\s" | awk '{ print $3 }')
	for C_P_shsh in $C_PIDS_shsh
	do
		if [ -z $C_P_shsh ] ; then continue ; fi
		for a in $2;
		do
			echo -n "   ";
		done
		echo -n "|- "
		a_aaa="$2 k"
		echo $(ps -p $C_P_shsh -o pid,pgid,state,comm | grep -v PID | sed 's/\/.*\//...\//')
		ps_children_of_cpid $C_P_shsh $a_aaa
	done
}

function molitva
{
	if [ "$1" = "ru" ] ; then say -v Yuri $PRAY_TEXT_RUS ; fi
	if [ "$1" = "en" ] ; then say -v Daniel $PRAY_TEXT_ENG ; fi
	if [ -z $1 ] ; then say -v Fiona $PRAY_TEXT_ORIG ; fi
}
