BEGIN	{ FS = "-|," }

$1>=$3 && $2<=$4 || $3>=$1 && $4<=$2	{ p1++ }
$1<=$4 && $2>=$3			{ p2++ }

END	{ print("04:", p1, p2) }
