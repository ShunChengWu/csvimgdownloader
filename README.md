# CSV file downloader
This script is desgined for downloading images from the given website to jpg file. The downloading process will run in multi-thread manner. The script will detect all the available threads on your device and use all of them.

The format of the csv file should be

filename, website,
filename, website,
...

The file name colum will be the filename of the download images.

## How to Build
Run
`sh build.sh`
or
`mkdir build; cd build; cmake ..; make;`

## Dependencies
CURL library [link](https://curl.haxx.se/download.html)
    - for downloading images from websites
    
    
[Optional] OpenCV [link](https://opencv.org/)
    - with OpenCV you can set the scaling factor



## How to Use
`./csvdownloader [path_to_csv] [path_to_output_dir] [scale_factor]`
[path_to_csv]:  The path to the input csv file
[path_to_output_dir]: The path to the output folder.
[scale_factor]: (optional) scaling the image. (e.g. 0.5 )


