# FCB Series Menu

## Table of Contents
1. [Purpose/Overview](#purposeoverview)
2. [Features](#features)
3. [Special Features](#special-feature)
4. [Running the Program](#running-the-program)
5. [Specifications](#specifications)
6.  [Disclaimer](#disclaimer)
7. [Contact Information](#contact-information)

## Purpose/Overview
The **FCB Series Menu** is a terminal-based interface designed to facilitate control over FCB security block cameras. This program, written in C, allows users to interact with various camera functions and settings efficiently.

## Features
- **Camera**: Control and configure camera settings.
- **Lens**: Adjust lens parameters for optimal viewing.
- **Exp/WB**: Manage exposure and white balance settings.
- **Spot-Focus**: Focus on specific areas of the camera feed.
- **Motion Detection**: Detect motion within the camera’s field of view. (This feature is combined with spot focus)
- **IQ**: Adjust image quality settings.
- **Extend**: Access additional functionalities or extensions.

## Special Feature
The FCB Series Menu includes advanced functionality with motion detection combined with spot focus. The program captures and analyzes four distinct frames—top left, top right, bottom left, and bottom right of the screen. When motion is detected in any of these frames, an alert is sent to the terminal indicating which frame detected the motion. The frame with detected motion is then focused in on using the spot focus feature, ensuring detailed observation of the area of interest.

## Running the Program
To run the FCB Series Menu program:
1. Make sure you are in the correct Directory
   ``` shell
   $ cd Desktop/visca
2. Clean your Directory
   ``` shell
   $ make clean
3. run the menu program
   ``` shell
   $ ./visca

## Specifications
- **Operating Systems**: Designed to run on both Linux 64-bit Desktop and Embedded 64-bit ARM NVIDIA Jetson operating systems.

## Disclaimer
This menu has been tested on 1 FCB camera model (E9500H) and is assumed to be compatible with every FCB camera based on the similarities between all camera models.


## Contact Information

For any questions or support, please reach out to:

**School Email:** cherresb@grinnell.edu
