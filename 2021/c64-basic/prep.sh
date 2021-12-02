rm -rf data/
mkdir -p data/
cp ../data/*-sample.txt ../data/*-input.txt data/
unix2mac -q data/*
sed -i 's/ /,/g' data/02-*
