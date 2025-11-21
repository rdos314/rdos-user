@echo off

echo "Building Win32 lib"
ide2make -p win32/win32 1>nul
wmake -f win32/win32.mk -h -e 1>nul

echo "Building User gate conversion tool"
ide2make -p kernel/convuser 1>nul
wmake -f kernel/convuser.mk -h -e 1>nul

echo "Building classlib"
ide2make -p classlib 1>nul
wmake -f classlib.mk -h -e 1>nul
