<h2>TRADING PLATFORM</h2>
    - README.md - this document<br>
    - TEST.md   - test library in console<br>
    - note.txt  - collecting troubles<br>

<h3>Summary</h3>
    - This project focuses on developing libraries rather than trading itself.<br>
    - Libraries work for sql, network, vector computation and etc.<br>

<h3>Libraries</h3>
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


![Architecture](https://github.com/frogkim/pictures/blob/main/trading_platform_00.png)
<h3>Target System</h3>
    1. Server <br>
    - Ubuntu<br>
    - Nvidia GPU or CPU support SSE3 or above<br>
    2. Client <br>
    - x64 Windows<br>
    - IA64 architecture CPU supporting SSE3 or above<br>
<h3>1. Broker Server and Server</h3>
    - libnetwork, libfixprotocol<br>
    - Between this relationship, the server will be a client for Broker's server.<br>
    - Broker's server request its client to follow FIX 4.4<br>
