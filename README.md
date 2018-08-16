# Shitty DPMS

Simple and crude DPMS implementation relying only on mice and keyboard.
Created in hopes to fix Xorg's DPMS that often stops working.

# Compilation

`gcc -lpthread main.c -o dpms`

# Usage

`# dpms OffTime path_to_keyboard_dev`
