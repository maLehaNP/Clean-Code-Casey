My handwritten implementation of Casey's Muratori code examples from ["Clean" Code, Horrible Performance video](https://youtu.be/tD5NrevFtbU?si=K0zaIRWLtG9tryUG) ([article](https://www.computerenhance.com/p/clean-code-horrible-performance)).

Got the idea to write from [this rust implementation](https://github.com/interpol-kun/clean_code_rust).

## Build
Run `make` to build `total_area` and `corner_area` executables:
```bash
make
```
or specify target to build only one.

Compiler optimization flags: `-O3 -mavx512f`

## Usage
Do
```bash
make run
```
to test all routines.

`total_area` takes 1 argument, which can be:
- TotalAreaVTBL4
- TotalAreaSwitch4
- TotalAreaTable4

`corner_area` takes argument as `total_area` with "Corner" prefix or no argument to run all routines.

## Benchmark on my PC (AMD Ryzen 5 8400F)

**Repeat Count: 1**

**Function**|**Performance (cycles/shape)**|**Times Faster**
:-----:|:-----:|:-----:
CornerAreaVTBL    | 0.003004 |  1.000x
CornerAreaVTBL4   | 0.001978 |  1.519x
CornerAreaSwitch  | 0.000624 |  4.817x
CornerAreaSwitch4 | 0.000628 |  4.780x
CornerAreaTable   | 0.000622 |  4.831x
CornerAreaTable4  | 0.000321 |  9.347x
CornerAreaSSE     | 0.000319 |  9.431x
CornerAreaSSE4    | 0.000340 |  8.824x
CornerAreaAVX     | 0.000449 |  6.688x
CornerAreaAVX4    | 0.000326 |  9.211x
CornerAreaSSE_2   | 0.000365 |  8.225x
CornerAreaSSE4_2  | 0.000367 |  8.182x
CornerAreaAVX4_2  | 0.000366 |  8.203x

**Repeat Count: 1000**

**Function**|**Performance (cycles/shape)**|**Times Faster**
:-----:|:-----:|:-----:
CornerAreaVTBL    | 0.005337 |  1.000x
CornerAreaVTBL4   | 0.002021 |  2.640x
CornerAreaSwitch  | 0.000620 |  8.604x
CornerAreaSwitch4 | 0.000621 |  8.594x
CornerAreaTable   | 0.000620 |  8.610x
CornerAreaTable4  | 0.000320 | 16.672x
CornerAreaSSE     | 0.000322 | 16.570x
CornerAreaSSE4    | 0.000345 | 15.484x
CornerAreaAVX     | 0.000450 | 11.862x
CornerAreaAVX4    | 0.000330 | 16.175x
CornerAreaSSE_2   | 0.000366 | 14.574x
CornerAreaSSE4_2  | 0.000371 | 14.403x
CornerAreaAVX4_2  | 0.000365 | 14.606x
