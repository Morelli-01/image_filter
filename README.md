# Image Filter

Image Filter is a C++ command-line tool that scans a directory of images, extracts text using Optical Character Recognition (OCR), and copies images containing a specified text string to a designated output folder.

## Features

- **OCR Integration:** Utilizes Tesseract OCR (via OpenCV’s text module) to extract text from images.
- **Parallel Processing:** Employs OpenMP to process multiple images concurrently for improved performance.
- **Command-Line Interface:** Uses Boost.Program_options to provide a flexible interface for specifying input/output directories, image file extension, and search text.
- **Customizable File Extension:** Allows filtering for images with a specific extension (default is `.bmp`).

## Dependencies

To build and run this project, ensure you have the following installed:

- **C++ Compiler:** Supporting C++17 (or later) for `<filesystem>` and other modern features.
- **Tesseract OCR:** For text extraction.
- **Leptonica:** Required by Tesseract for image processing.
- **OpenCV:** Used for image reading and integration with OCR (specifically, the text module).
- **Boost.Program_options:** For parsing command-line arguments.
- **OpenMP:** For parallel processing support.

## Installation

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/Morelli-01/image_filter.git
   cd image_filter

