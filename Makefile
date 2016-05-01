GCC = nspire-gcc
LD = nspire-ld
GCCFLAGS = -Os -Wall -W -marm -D USE_FDLIBM -D TOUCHPAD_SUPPORT -D NSPIRE
LDFLAGS = -lm
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
EXE = sdlpal.tns
OBJS = text.o font.o itemmenu.o scene.o palcommon.o script.o \
	util.o play.o getopt.o input.o uibattle.o game.o magicmenu.o map.o \
	ending.o uigame.o rngplay.o ui.o global.o main.o fight.o \
	video.o palette.o res.o battle.o yj1.o touchpad.o

DISTDIR = .
vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

$(EXE): $(OBJS)
	$(LD) $^ -o $(@:.tns=.elf) $(LDFLAGS)
	mkdir -p $(DISTDIR)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $(DISTDIR)/$@

clean:
	rm -f *.o *.elf
	rm -f $(DISTDIR)/$(EXE)
