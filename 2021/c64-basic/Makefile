all: _built

_built: ../data/*-sample.txt ../data/*-input.txt
	rm -rf data/
	mkdir -p data/
	cp ../data/*-sample.txt ../data/*-input.txt data/
	unix2mac -q data/*
	sed -i 's/ /,/g' data/02-*
	touch _built

clean:
	rm -rf data/ _built

run:
	@./all.sh

.PHONY: all clean run
