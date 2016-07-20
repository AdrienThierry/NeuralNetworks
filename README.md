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

The neural net recognizes handwritten digits from the MNIST database (http://yann.lecun.com/exdb/mnist/). A local copy of the MNIST database is in the *data* folder. After the learning phase, the training error is plotted in a gnuplot window and when you press enter, the test phase is launched. A *result* folder is created and contains the results.

The results are .pgm images (an ASCII image format that you can open with most image viewers) which are images from the test database. The file name gives the recognition result :

```
x_y.pgm
```

**x** is the digit that has been recognized in the image by the neural net (0 to 9).

**y** is the number of the image in the MNIST test database.

Note that not all of the images of the test database are used for test. Only 100 of them are used (one image every 100 images in the MNIST test database).
