#!/usr/bin/make -f

.PHONY=build clean manifest

CHANGELOG=usr/share/doc/task-teacats/changelog.Debian.gz

build: $(CHANGELOG)
	@true

clean:
	@rm -vf "$(CHANGELOG)"
	@-rmdir -vp "$(dir $(CHANGELOG))"

manifest:
	@true

usr/share/doc/task-teacats/changelog.Debian.gz: CHANGELOG
	@mkdir -vp "$(dir $@)"
	gzip --keep "$^" --stdout >"$@"
