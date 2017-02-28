# Manuscrito
Terminal text editor written in C++. Written as an exercise to learn more of C++ and ncurses.
This text editor is still in development.

The text editor uses buffers for text editing. There are several modes, just like in vim.

## Usage
Initial mode: normal mode.

Actions available from all modes
- Save file (Ctrl-R), entering prompt mode

In normal mode you can
- Navigate throughout the text
- Enter insert mode (i)
- Exit the editor (Esc)

In normal mode you can
- Edit text
- Exit the insert mode, and enter normal mode again (Esc)

In prompt mode you can
- Exit the prompt mode, thus cancelling the current action (Esc)
- Navigate left or right around the text you have entered so far, 
while in prompt mode

<!-- ## Getting Started -->
 <!-- ### Prerequisites -->
 <!-- You need to have ncurses installed. -->
<!-- ### Installing (TBA) -->

## Development

### Building

```
make clean # optional, to clean the project
make
```

### Running
```
./manuscrito.out # to edit a new file
./manuscrito.out myText.txt # or edit an existing file
```

## Contributing
 
 Suggestions are welcome. Filing of issues is appreciated.

## How it works
TBA
<!--  post control flow and sequence diagrams-->

## Authors
Anny Gakhokidze

## License

## Acknowledgements
