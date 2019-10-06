# HDRI Merging Software

This is a very, very simple HDRI merging program, designed to fit into a linear HDRI-Merging workflow.

## Why Write your own HDRI merging software?

I wrote this because most of the available solutions did not fully fit my requirements.

The goals of this project:

 * Handle large images. While all the images are still loaded into ram (this program can use 10s of GB of ram while running on 16k panoramas), the program will chew through them in a reasonable amount of time. Many of the off the shelf programs I attempted to use would crash or hang for hours in massive images, even when ample ram was available. 
 * Batch workflow friendly. This program does not have a GUI and can easily be slotted into an automated pipeline.
 * Does not mess up colors. Many programs that claim to do HDRI merging make bad assumptions about colors and color management (i.e. assume TIFFs are in sRGB). This will break the results, and hurt physical accuracy.
 * Easy to reason about. The code is stupid simple. This makes it easy to diagnose issues in the HDRI production pipeline.
 * Proving ground for new algorithms. By writing my software, I can easily test and develop algorithms not available in existing software.
 * Educate the author on color management and HDRI merging. scene referred color management is difficult, especially when dealing with consumer photo editing software. By writing my own solution I gained a better understanding of how this component in the pipeline worked.

## Building

This program depends on OpenImageIO (OIIO). For users on windows, I recommend that you use `vcpkg` to build OIIO. The windows build instructions provided on OpenImageIO's website do not work.

The build command looks something like this:

(run inside developer command prompt)
```
cl.exe /EHsc /MTd /Zi /I [vpkg include directory] \
src/hdri_merge.cpp \
src/merge_alg.cpp \
src/debug_exp.cpp \
/link \
[/path/to/debug/OpenImageIO.lib]
```
To build for release, use `/MT` instead of `/MTd`