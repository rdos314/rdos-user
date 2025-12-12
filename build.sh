echo "Building classlib"
ide2make -p classlib 1>nul
wmake -f classlib.mk -h -e 1>nul

