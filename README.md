# IMAGE_FILTER 
## NAME  
image_filter – filters images by extracting and searching for text using OCR  

## SYNOPSIS  
`image_filter` **-i** *input_path* **-o** *output_path* **-t** *text* [**-e** *ext*]  

## DESCRIPTION  
`image_filter` scans a directory of images, extracts text using Tesseract OCR, and copies images containing a specified text string to an output folder.  

## OPTIONS  
**-i**, **--input_path** *DIR*  
:   Directory containing image files to process (recursively).  

**-o**, **--output_path** *DIR*  
:   Directory where matching images will be copied.  

**-t**, **--text** *STRING*  
:   Text to search for in the extracted OCR result.  

**-e**, **--ext** *EXT* (default: `.bmp`)  
:   File extension of images to process (e.g., `.png`, `.jpg`).  

**-h**, **--help**  
:   Show usage information.  

## EXAMPLES  
Filter `.png` images containing "example" in `/path/to/images` and copy them to `/path/to/output`:  
```sh
image_filter -i /path/to/images -o /path/to/output -t "example" -e ".png"
