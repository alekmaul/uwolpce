# path to mddev root directory 
#export DEVKITPCE := /c/mddev
#export DEVKITPCE := /f/DropboxSvnClient/trunk/pcedev
export DEVKITPCE := /z/vboxshared/DropboxSvnClient/trunk/pcedev

export DEVKITPCEBIN := $(DEVKITPCE)/bin
export DEVKITPCEINC := $(DEVKITPCE)/include/pce

# change global path with SGDK
export PATH	:=	/c/msys/bin:$(DEVKITPCEBIN):$(PATH)
#export PATH	:=	/e/ndsdev/msys/bin:$(DEVKITPCEBIN):$(PATH)

export GFXCONV := ./tools/bmp2pce
export MODCONV := $(DEVKITPCEBIN)/mod2mml
export MMLCONV := ./tools/mml2pce
export PCMCONV := $(DEVKITPCE)/tools/sox/sox

#---------------------------------------------------------------------------------
# the compiler executables
#---------------------------------------------------------------------------------

#export CC	:=	$(DEVKITPCEBIN)/huc
#export PCE_INCLUDE := $(DEVKITPCEINC) 
#export PCE_PCEAS := $(DEVKITPCEBIN)/pceas

export CC := PCE_INCLUDE=$(DEVKITPCEINC) PCE_PCEAS=$(DEVKITPCEBIN)/pceas $(DEVKITPCEBIN)/huc

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
	
#---------------------------------------------------------------------------------
# TARGET is the name of the output
#---------------------------------------------------------------------------------
TARGET		:=	uwolpce

SRC = main.c uwol_fct.c uwol_lvl.c uwol_snd.c 
LIBS = -lst
#-lst
CFLAGS = -v -msmall -fno-recursive
#-fno-recursive -msmall

#---------------------------------------------------------------------------------
# Name of text file containing build number.
BUILD_NUMBER_FILE=buildnumber.txt
BUILD_INCLUD_FILE=buildnum.h
BUILD_MAIN_FILE=main.c

#---------------------------------------------------------------------------------
.PHONY: bitmaps musics all
 
#---------------------------------------------------------------------------------
all	:	bitmaps musics uwolpce.pce
	mv $(TARGET).pce $(TARGET)$$(date +'%y%m%d')"B"$$(cat $(BUILD_NUMBER_FILE)).pce
	@echo Generate new build number ... $(notdir $@)
	@if ! test -f $(BUILD_NUMBER_FILE); then echo 0 > $(BUILD_NUMBER_FILE); fi
	@echo $$(($$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE)
	@echo "#define BUILDID \""$$(date +'%y%m%d')"B"$$(cat $(BUILD_NUMBER_FILE))\" > $(BUILD_INCLUD_FILE)

uwolpce.pce: $(SRC)
	$(CC) $(CFLAGS) main.c $(LIBS)
	sleep 3
	cp main.pce $(TARGET).pce

clean:
	rm -f *.pce *.lst *.sym *.s 
	rm -f snds/*.raw snds/allMML.asm sngInit.c snds/titlescreen.asm snds/titlescreen.mml snds/gamemap.mml
	rm -f gfxs/*.gfx gfxs/*.map gfxs/*.pal

#---------------------------------------------------------------------------------
gfxs/credits.gfx: gfxs/credits.bmp
	$(GFXCONV) BG $<

gfxs/mojon.gfx: gfxs/mojon.bmp
	$(GFXCONV) BG $<

gfxs/ubhres.gfx: gfxs/ubhres.bmp
	$(GFXCONV) BG $<

gfxs/title.gfx: gfxs/title.bmp
	$(GFXCONV) BG $<

gfxs/titlehs.gfx: gfxs/titlehs.bmp
	$(GFXCONV) SPR $< 
	
gfxs/titleb1.gfx: gfxs/titleb1.bmp
	$(GFXCONV) SPR $<

gfxs/finbad.gfx: gfxs/finbad.bmp
	$(GFXCONV) BG $<
	
gfxs/fingoo.gfx: gfxs/fingoo.bmp
	$(GFXCONV) BG $<
	
gfxs/gover.gfx: gfxs/gover.bmp
	$(GFXCONV) BG $<

gfxs/finend.gfx: gfxs/finend.bmp
	$(GFXCONV) BG $<

gfxs/borders.gfx: gfxs/borders.bmp
	$(GFXCONV) SPR $<
#---------------------------------------------------------------------------------
gfxs/tilesa.gfx: gfxs/tilesa.bmp
	$(GFXCONV) BGRAW $<
	
gfxs/tiles0.gfx: gfxs/tiles0.bmp
	$(GFXCONV) BGRAW $<

gfxs/tilesb.gfx: gfxs/tilesb.bmp
	$(GFXCONV) BGRAW $<

gfxs/tilesc.gfx: gfxs/tilesc.bmp
	$(GFXCONV) BGRAW $<

#---------------------------------------------------------------------------------
gfxs/sprites.gfx: gfxs/sprites.bmp
	$(GFXCONV) SPR $<

bitmaps : gfxs/credits.gfx gfxs/mojon.gfx gfxs/ubhres.gfx gfxs/title.gfx gfxs/titlehs.gfx gfxs/titleb1.gfx \
	gfxs/finbad.gfx gfxs/fingoo.gfx gfxs/gover.gfx gfxs/finend.gfx \
	gfxs/tilesa.gfx gfxs/tiles0.gfx gfxs/tilesb.gfx gfxs/tilesc.gfx \
	gfxs/sprites.gfx gfxs/borders.gfx

#---------------------------------------------------------------------------------
snds/titlescreen.mml: snds/titlescreen.mod
#	$(MODCONV) -d 3=4,5=2 -p 5 -m 1=43 -a -n -s -f 3=.25 -o $@ $<
	$(MODCONV) -d 3=4,5=2 -p 5 -a -n -s -f 3=.25 -o $@ $<
	@cat mod2mml.log

snds/gamemap.mml: snds/gamemap.mod
	$(MODCONV) -d 3=3,5=5 -a -n -s -o $@ $<
	@cat mod2mml.log
	
snds/allMML.asm: snds/titlescreen.mml snds/gamemap.mml 
	$(MMLCONV) snds/allMML.mml

musics: snds/allMML.asm
	
#	../../bin/mod2mml -d 11=4,12=3,13=1,15=2 -a -n -v 3=28,1=28 -f 3=.25 -o $@ $<

#---------------------------------------------------------------------------------
snds/jump.raw: snds/jump.wav
#	$(PCMCONV) -v 3 $< -b 8 -r 6992 $@
#	cp $@ $@.8b
	cp snds/jump.wav snds/jump.raw.8b
	./conv5bit -16 $@.8b $@
	./pcmpack_1.3 $@
	rm $@.8b

sounds: 
#snds/jump.raw
