# Hide your data

This program is a steganography tool that hides binary data in a ppm image.

Hidden bytes are encoded in the two less significant bits of the native image.

Size of the hidden file is written in the comment.

If the ppm don't have enough data nothing will be done.

The algorithm seem to doesn't fit well with how GDK pixbuf handles integer recognition in files.

Using feh to view the image is a good choice.

# Compile

On linux:
`
$ make
$ ./ppm_steg
`

Usage is printed each time you misuse the executable.
