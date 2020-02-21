CFLAGS=-Wall -Wextra -Werror -std=c99 -pedantic

run: view-image
	./view-image test.bmp

view-image: view-image.c
	$(CC) $(CFLAGS) -oview-image view-image.c -lSDL2
install: view-image
	cp view-image /usr/bin
	cp view-image.desktop /usr/share/applications/
uninstall:
	rm /usr/bin/view-image
	rm /usr/share/applications/view-image.desktop
clean:
	-rm view-image
