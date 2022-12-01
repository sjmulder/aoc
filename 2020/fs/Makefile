all: _built

_built: **/*.fs
	dotnet build -v m --nologo -c release aoc2020-fs.sln
	touch _built

clean:
	dotnet clean -v m --nologo -c release aoc2020-fs.sln
	rm -f _built

run: all
	@./all.sh

.PHONY: all clean run
