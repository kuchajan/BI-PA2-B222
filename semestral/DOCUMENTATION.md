# Ascii art

A tool to convert images to ascii art using shades of gray.
Use following arguments to specify input:

- -i path : defines a path to an image to convert, may be used multiple times
- -cs [characters] [indexes] : defines a character set from which to generate an ascii art, cannot be more than 256 characters long
- -os [value] : selects the type of output to use (only one can be used)
	- 1 : prints out all the results to the console
	- 2 [path]: prints each individual result to files
	- 3 : starts a presentation of the images (space to start/stop, left to go to previous, right to go to next, q to exit)
- -fs [width] [height] [indexes] : resizes images to given width and height
- -fcr [fromX] [fromY] [width] [height] [indexes] : crops images
- -fb [value] [indexes] : changes the brightness by given value
- -fct [value] [indexes] : changes the contrast by given value
- -frr [indexes] : rotates the image by 90 degrees to the right
- -frl [indexes] : rotates the image by 90 degrees to the left
- -fmh [indexes] : mirrors the image horizontally
- -fmv [indexes] : mirrors the image vertically
- -ffh [indexes] : flips the image horizontally
- -ffv [indexes] : flips the image vertically
- -fi [indexes] : inverts the image