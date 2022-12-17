all: _built

_built: day08-cs.csproj *.cs ../util-cs/*.cs
	dotnet build -v m --nologo -c release day08-cs.csproj
	touch _built

clean:
	rm -rf bin/ obj/
	rm -f _built

.PHONY: all clean
