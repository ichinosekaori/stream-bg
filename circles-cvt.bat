ffmpeg -framerate 60 -i .\circles-%d.ppm -start_number 0 -c:v libx264 -r 60 circles.mp4 -pix_fmt yuv420p
