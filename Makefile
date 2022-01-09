
VER ?= 111
# TITLE_IDS = 0100000011D90000 010018E011D92000
TITLE_IDS = 0100000011D90000
TARGET      = $(notdir $(CURDIR))

DIST        = $(CURDIR)/dist/$(VER)
BUILD       = $(CURDIR)/build/$(VER)/%
EXEFS_PATCH = $(DIST)/atmosphere/exefs_patches/pokemon_bdsp_%
EXEFS       = $(DIST)/atmosphere/contents/%/exefs
 
.PHONY: all clean $(TITLE_IDS)

all: $(TITLE_IDS)

$(TITLE_IDS): %: $(EXEFS)/subsdk1 $(EXEFS_PATCH) $(BUILD)/$(TARGET).nso

$(BUILD)/$(TARGET).nso: source/* include/*
	$(MAKE) all -f nso.mk VER=$(VER) TITLE_ID=$*

$(EXEFS_PATCH): patches/$(VER)/%/patch.ini patches/$(VER)/%/*.json $(BUILD)/$(TARGET).nso
	@mkdir -p $@
	python3 scripts/gen_patch.py $(VER) $* $@

$(EXEFS)/subsdk1: $(BUILD)/$(TARGET).nso
	@mkdir -p $(dir $@)
	@cp $< $@

clean:
	$(MAKE) clean -f nso.mk
	@rm -fr $(DIST) 
