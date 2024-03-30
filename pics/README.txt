Logo is from the breeze-icons repo, with the icon name: accessories-character-map
Images are generated with the following bash command after you get the SVG file from breeze-icons repo:
for s in 16 32 64 128 256 512 1024 ; do rsvg-convert -w${s} -h${s} accessories-character-map.svg > ${s}-apps-kcharselect.png ; done