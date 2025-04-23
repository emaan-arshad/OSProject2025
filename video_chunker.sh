#!/bin/bash

# Define input and output directories
INPUT_DIR="input_videos"
OUTPUT_DIR="video_chunks"

# Define resolutions for each quality (240p, 480p, 720p)
declare -A RESOLUTIONS
RESOLUTIONS["240"]="426x240"
RESOLUTIONS["480"]="854x480"
RESOLUTIONS["720"]="1280x720"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Loop through all input videos (v1.mp4, v2.mp4, etc.)
for VIDEO_PATH in "$INPUT_DIR"/v{1..5}.mp4; do
VIDEO_NAME=$(basename "$VIDEO_PATH")  # Get the video file name (e.g., v1.mp4)
VIDEO_BASE="${VIDEO_NAME%.*}"  # Extract the base name without extension (e.g., v1)

echo "Processing $VIDEO_NAME..."

# Loop through all defined resolutions (240, 480, 720)
for QUALITY in "${!RESOLUTIONS[@]}"; do
    RES=${RESOLUTIONS[$QUALITY]}
    OUTPUT_PATH="$OUTPUT_DIR/$VIDEO_BASE/$QUALITY"  # Output path for this quality

    # Create the directory for this quality if it doesn't exist
    mkdir -p "$OUTPUT_PATH"

    echo "  -> Creating $QUALITYp chunks for $VIDEO_NAME into $OUTPUT_PATH"

    # Use ffmpeg to split the video into chunks with the specified resolution
    ffmpeg -i "$VIDEO_PATH" \
        -vf "scale=$RES" \
        -c:v libx264 -preset fast -crf 23 \
        -c:a aac -b:a 128k \
        -f segment -segment_time 2 -reset_timestamps 1 \
        "$OUTPUT_PATH/chunk_%03d.ts"
done
done

echo "Video chunking done"
