#!/usr/bin/make -f

.PHONY=build clean manifest

build: usr/share/doc/task-teacats/changelog.Debian.gz
	@true

clean:
	@rm -vf usr/share/doc/task-teacats/changelog.Debian.gz

manifest:
	@true

usr/share/doc/task-teacats/changelog.Debian.gz: CHANGELOG
	gzip --keep "$^" --stdout >"$@"
