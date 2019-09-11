# /*//. Makefile to compile Q3 on Linux         /
# ../ Bob 'tIKi_mAn' Majdak Jr                  /
# ../ http://www.opsat.net - bob@opsat.net  .//*/

# first thing, this will not compile right out of the zip because of windows
# line endings apparently, so the first thing you should do after unzipping
# is `make fix`

# `make all` will build the engine and the game, whereas
# `make binary` will build the game engine binaries, and
# `make vm` will build the game code.
# `make sweep` will dump both the build and output directories.

# oh, by the way. this file goes in the code directory.
#    example: /home/bob/src/quake3-1.32b/code/Makefile
#    (yes overwrite the one there, lol)

# updated instructions from the ones above (after fixing the directory of course).
#	`make binary` builds debug version (the default).
#	`make binary-release` builds release version.
#	`make fresh` cleans cleans things up.
#	`make install-bin` will copy the compiled binary to the Q3_DIR/Q3_BIN.BUILD_TYPE.
#		so basicly /path/quake3.x86.dbg or /path/quake3.x86.rel.
#	`make final` takes the installed bin and moves it to its final resting place.
#		so basicly /path/quake3.x86.dbg -> /path/quake3.x86

# backup your original q3 binary before running final-bin.

# a typical make command you will see me type.
#	`make fresh binary install-bin`
#	boom boom boom.

# :D

# edit these.
Q3_DIR="/home/games/quake3"
Q3_BIN="quake3.x86.cel"

# leave this one be.
BUILD_TYPE=`cat build.type`

all:
	perl unix/cons

all-release:
	perl unix/cons -- release

fix:
	perl -pi.bk -e 's/\r//' game/*.c game/*.h
	perl -pi.bk -e 's/\r//' cgame/*.c cgame/*.h
	perl -pi.bk -e 's/\r//' ui/*.c ui/*.h
	perl -pi.bk -e 's/\r//' q3_ui/*.c q3_ui/*.h

binary:
	echo "dbg" > build.type
	perl unix/cons -- nosmp novm noso

binary-release:
	echo "rel" > build.type
	perl unix/cons -- release nosmp novm noso

vm:
	perl unix/cons -- nosmp noso
	
vm-release:
	perl unix/cons -- release nosmp noso

fresh:
	rm -rf install
	rm -rf debug-x86-Linux-2.3
	rm -rf release-x86-Linux-2.3
	rm -rf build.type

install-bin:
	@case $(BUILD_TYPE) in \
		*dbg*) \
			echo "Debug Build"; \
			cp -vf install/linuxquake3 $(Q3_DIR)/$(Q3_BIN).$(BUILD_TYPE);; \
		*rel*) \
			echo "Release Build"; \
			cp -vf install/linuxquake3 $(Q3_DIR)/$(Q3_BIN).$(BUILD_TYPE);; \
		*) \
			echo "No Build Detected.";; \
	esac

final:
	@case $(BUILD_TYPE) in \
		*dbg*) \
			cp -vf $(Q3_DIR)/$(Q3_BIN).$(BUILD_TYPE) $(Q3_DIR)/$(Q3_BIN);; \
		*rel*) \
			cp -vf $(Q3_DIR)/$(Q3_BIN).$(BUILD_TYPE) $(Q3_DIR)/$(Q3_BIN);; \
		*) \
			echo "No Build Detected.";; \
	esac
