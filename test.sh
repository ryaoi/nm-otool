search_dir="./ft_nm-ft_otool_binaire_de_test/hell"
RED='\033[0;31m'
NC='\033[0m'
echo "NM!"
for entry in "$search_dir"/*
do
	nm $entry > real.txt 2>&-
	./ft_nm $entry > mine.txt 2>&-
	difference=$(diff mine.txt real.txt)
	if [[ -n "$difference" ]]
	then
		echo "$RED$entry$NC"
	fi
done
echo "OTOOL!"
for entry in "$search_dir"/*
do
	otool -t $entry > real.txt 2>&-
	./ft_otool $entry > mine.txt 2>&-
	difference=$(diff mine.txt real.txt)
	if [[ -n "$difference" ]]
	then
		echo "$RED$entry$NC"
	fi
done
