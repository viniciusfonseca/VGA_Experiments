# VGA Experiments

This is a repository with a few VGA rendering exercises.

The code is compiled with Turbo C 1.0 in a DOSBox 0.74 VM.

References:
http://www.brackeen.com/vga/index.html
http://www3.telus.net/alexander_russell/course/introduction.htm

I recommend taking Brackeen's code samples when you're starting with VGA. But attention for the samples given for Turbo C: He declares a `byte *` pointer to the VGA memory (`0xA0000000L`). This didn't worked for me, so I declared it as `byte far *` instead, and it worked.