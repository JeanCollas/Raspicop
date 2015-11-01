sudo xauth list | while read -r line; do sudo xauth add $line  ; done;

