<h2>TRADING PLATFORM</h2><br>
    - README.md - this document<br>
    - TEST.md   - test library in console<br>
    - note.txt  - collecting troubles<br>

<h3>Target System</h3><br>
    - x64 Windows and Ubuntu<br>
    - Nvidia GPU or CPU support SSE3 or above<br>

<h3>Summary</h3><br>
    - This project focuses on developing libraries rather than trading itself.<br>
    - Libraries work for sql, network, vector computation and etc.<br>

<h3>Libraries</h3><br>
    - exported functions follow C style<br>

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
