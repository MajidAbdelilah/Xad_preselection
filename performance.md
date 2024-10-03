# Performance achived:
  * ## platforms:
    - platform number 1:
      - os: macos version 10.15.7
      - proccessor:  Intel(R) Core(TM) i5-7500 CPU @ 3.40GHz
      - compiler: Apple clang version 12.0.0 (clang-1200.0.32.29)
      - performance metrics:
          - init time = 1135.29ms
          - run time = 546.181ms
          - sum = 261191
    - platform number 2:
      - os: windows 11
      - proccessor: 12th Gen Intel(R) Core(TM) i7-1255U   1.70 GHz
      - compiler MSVC 17 2022 64-bit
      - performance metrics:
        - init time = 638.964ms
        - run time = 1285.93ms
        - sum = 251742

# implementation:
- i used avx and avx2 and fma to hardware accelerate the dot product inside of the frobenius_norm formula.
and i also used std::thread to multithread the dot product calculation.
wich make this frobenius_norm function uses every cpu thread each with its full simd width.

- i also used std::thread for the conversion from MatrixView to Matrix wich make it uses every cpu thread.
also i've used memcpy for copying the data in the conversion proccess from MatrixView to Matrix.
and as we know memcpy uses the full simd width of the thread to copy data.
wich in turn makes the proccess of converting MatrixView to Matrix uses every cpu thread each with its full simd width.

- in my observations of the computation proccesss this implementation uses 80% to 90% of the cpu's computing power wich is almost the peak of what it can be.

# why this aproach is chosed
- to use the full capabilities of modern hardware
