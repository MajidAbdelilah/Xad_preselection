
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
    - dependencies:
      - msvc
      - a avx, avx2, and fma enabled proccessor(intel Haswell and newer)
      - googleTest(downloaded via CMakeLists.txt)
    - building
      - mkdir build
      - cd build
      - cmake ../
      - open the soulution generated by cmake in visual studio
      - right click on the unit_test project and select properties
      - select c/c++ -> code generation
      - in debug mode change the runtime library to multithreaded debug and hit apply
      - in release mode change the runtime library to multithreaded and hit apply
      - hit save
      - never re-run cmake ../ again or else you need to do the previuse steps again(the problem is that there is a static vs dynamic runtime library conflict between unit_test and gtest that i couldnt fix, thanks for your understanding)
      - cmake --build . --config Release -v --clean-first
    - running
      - unit_test(from project root):
        - cd build
        - cd Release
        - .\unit_test.exe
      - performance_test(from project root):
        - cd build
        - cd Release
        - .\performance_test.exe

