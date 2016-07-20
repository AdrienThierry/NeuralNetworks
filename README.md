# NeuralNetworks
A basic neural network library and demos : learning sine function and handwritten digits recognition

## Dependencies
* cmake
* gnuplot

## Compilation
```
cmake .
make
```

## Demos

### Learning sine function
```
./NeuralNetworks --learning-sin
```

The neural net tries to approximate the sine function. At the end of the learning phase, a gnuplot window opens in which **true sine** and **sine approximated by the neural net** are plotted.

### Handwritten digits recognition
```
./NeuralNetworks --learning-mnist
```
