<h>TRADING PLATFORM</h>
    - README.md - this document<br>
    - TEST.md   - test library in console<br>
    - note.txt  - collecting troubles<br>

<h3>Target System</h3>
    - x64 Windows and Ubuntu<br>
    - Nvidia GPU or CPU support SSE3 or above<br>

Summary
    - This project focuses on developing libraries rather than trading itself.
    - Libraries work for sql, network, vector computation and etc.

Libraries
    - exported functions follow C style

    1. libmath
        - target cuda toolkit's version is 12.2
        - support CUDA, AVX, SSE3
        - Assembler is MASM for Windows and NASM for Linux
        - focuses on vector calculation
            - Complete:
            - Plan:
                add, subtract, multiply, divide
                average, stadard deviation, regression
                gradient descent

    2. libnetwork
        - On Windows environment, IOCP is used
        - On Linux environtment, epoll is used

    3. libsql
        - DB is supposed to MS-SQL
        - manages connection with DB

    4. libfixprotocol
        - converts orders to the fix protocol structure

    5. libutils
        - writes log asynchronously.
        - manages shared memory inter processes
