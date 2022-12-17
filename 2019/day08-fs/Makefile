all: _built

_built: day08-fs.fsproj *.fs
	dotnet build -v m --nologo -c release
	touch _built

clean:
	rm -rf bin/ obj/
	rm -f _built

.PHONY: all clean
