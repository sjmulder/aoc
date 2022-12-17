all: _built

_built: **/*.cs
	dotnet build -v m --nologo -c release aoc2022-cs.sln
	touch _built

clean:
	dotnet clean -v m --nologo -c release aoc2022-cs.sln
	rm -f _built

run: all
	@./all.sh

.PHONY: all clean run
