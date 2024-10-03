
#  team name : sword of allah

# team members
  * memeber number 1:
    - name: Abdelilah Majid
    - email: abdelilahmajid012@gmail.com

# build proccess
  * linux & macos:
    - dependencies:
      - gcc or clang
      - a avx, avx2, and fma enabled proccessor(intel Haswell and newer)
      - googleTest(downloaded via CMakeLists.txt)
    - building:
      * mkdir build && cd build 
      * cmake ../ -DCMAKE_BUILD_TYPE=Release
      * cmake --build . -v
    - running:
      - for running the unit_tests(from the root of the project):
        * cd build && ./unit_test
      - for runnign the performance benchmark(from the root of the project):
        * cd build && ./performance_test
  * windows
  // todo: write the windows instructions

