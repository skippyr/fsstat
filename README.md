<h1 align="center">fsstat</h1>

## About

It is a system utility for UNIX-like operating systems that stats different
types of info about file system entries, such as: their type, size, permissions,
ownership and modified date.

It is designed to be a convenient alternative for building shell scripts.

## Install

### Dependencies

The following dependencies are required to install it:

1. **git**: it will be used to clone this repository.
2. **gcc**, **make**: they will be used to compile and install it.

### Procedures

Using a terminal, follow these steps:

1. Clone this repository using git:

```sh
git clone --depth 1 https://github.com/skippyr/fsstat;
```

2. Access the repository directory using cd:

```sh
cd fsstat;
```

3. Use make to compile and install it:

```sh
sudo make install clean;
```

## Uninstall

Using a terminal, follow these steps:

1. Go back to the repository directory.
2. Use make to uninstall it:

```sh
sudo make uninstall;
```

## Documentation

After installed, you can refer to its manual pages by using man:

```sh
man fsstat.1;
```

## Help

If you need help about this project, open a new issue in its
[issues page](https://github.com/skippyr/fsstat/issues) or send me an
[e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

## Contributing

This project is open to review and possibly accept contributions, specially
fixes and suggestions. If you are interested, send your contribution to its
[pull requests page](https://github.com/skippyr/fsstat/pulls) or to my
[e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you must agree to license your work under the
same license that the project uses.

## License

This project is licensed under the BSD-3-Clause License. Refer to the LICENSE
file that comes in its source code for license and copyright details.
