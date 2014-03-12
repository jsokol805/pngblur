pngblur
=======
Simple program to blur a png image or xwd dump, with variable box filter kernel size.

Requires zlib to run.
usage
-------
for blurring png
````
$ pngblur <source> <destination> <kernel size>
```
if _kernel size_ is not provided, it is set to 3

for blurring xwd dumps, use "-" as source and provide the dump by pipe e.g.:
````
$ xwd -root | pngblur - <destination> <kernel size>
```
license
-------
_pnglite_ by Daniel Karling, license inside _pnglite.h_
