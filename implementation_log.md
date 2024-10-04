# implementation log

- first ive implemented the Matrix Class and i've tried to make the MatrixView class a child of the Matrix class, however this implementation was later discarded due to the complexity and compatibilty issues it has
- second i reimplemented everything this time Matrix and MatrixView are separate classes and to convert from a MatrixView to Matrix i've used a typecast operator overload. this is the implementation i still have till this point.
- third i've implemented the frobenius_norm function with a baseline unoptimized version and the optimized (avx,avx2,fma) version wich is used for the benchmark
- fourth i've observed the cpu utilization of my implementation and noticed it uses only 20% of it wich what led me to the conclosion that the conversion from MatrixView to Matrix is botelneking my programs execution so ive optimized it.
- fifth ive coded the unit tests.
- ive done the documation
- last ive done the video