# Sobel
Uses a less complex non-gaussian convolution and threshold to find edges in an image file

Note: This is a Code blocks project, however the C file will work just fine when compiled like this:

> gcc sobel.c infile.pgm outfile1.pgm outfile2.pgm

The first outfile will show the gradient magnitude of the image, the second will show the actual line trace of the image outline

Images should be 256 pixels by 256 pixels. LOW_THRESHOLD and HIGH_THRESHOLD values can be changed to get better results based on the image.
