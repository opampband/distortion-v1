# Distortion

Tube-modelling distortion.

## Build Instructions

First, you must clone this repo with submodules since we rely on the `jack-wrapper` library.

``` bash
git clone --recurse-submodules https://github.com/opampband/distortion.git
```

Then build with make:

``` bash
make
```

## Running Instructions

First, ensure a JACK server is running on your system.

Then:

``` bash
./bin/distortion
```

## TODO

* [ ] Add LPF at the end
* [ ] Make it parametric (adjustable gain)
* [ ] Implement tone stack

## References

* https://www.mitpressjournals.org/doi/pdf/10.1162/comj.2009.33.2.85
