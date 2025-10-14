#!/usr/bin/make -f

.PHONY=build clean manifest

CHANGELOG=usr/share/doc/task-teacats/changelog.Debian.gz
DWM=usr/bin/dwm
DWM_MAN=usr/share/man/man1/dwm.1
DWMSTATUS=usr/bin/dwmstatus
BACKLIGHT=usr/bin/backlight

build: $(CHANGELOG) $(DWM) $(DWM_MAN) $(DWMSTATUS) $(BACKLIGHT)
	@true

clean:
	@rm -vf "$(CHANGELOG)" "$(DWM)" "$(DWM_MAN)" "$(DWMSTATUS)"
	@-rmdir -vp "$(dir $(CHANGELOG))"
	@-rmdir -vp "$(dir $(DWM))"
	@-rmdir -vp "$(dir $(DWM_MAN))"
	@-rmdir -vp "$(dir $(DWMSTATUS))"

manifest:
	@true

../dwm/dwm:
	$(MAKE) -C "$(@D)" "$(@F)"

../dwmstatus/dwmstatus:
	$(MAKE) -C "$(@D)" "$(@F)"

../backlight/backlight:
	$(MAKE) -C "$(@D)" "$(@F)"

$(DWM): ../dwm/dwm
	@mkdir -vp "$(dir $@)"
	@cp -v "$<" "$@"

$(DWM_MAN): ../dwm/dwm.1
	@mkdir -vp "$(dir $@)"
	@cp -v "$<" "$@"

$(DWMSTATUS): ../dwmstatus/dwmstatus
	@mkdir -vp "$(dir $@)"
	@cp -v "$<" "$@"

$(BACKLIGHT): ../backlight/backlight
	@mkdir -vp "$(dir $@)"
	@cp -v "$<" "$@"

usr/share/doc/task-teacats/changelog.Debian.gz: ../CHANGELOG
	@mkdir -vp "$(dir $@)"
	gzip --keep "$^" --stdout >"$@"
