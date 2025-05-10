# About the Project
This project was developed as part of the System Programming class. It combines the concepts seen throughout the class.
The project details can be found in Project_Details.pdf file.

# 1. Directory Structure of the Project
```bash
RCS/
├── include/
│   ├── GyroLib.h
│   ├── KeypadLib.h
│   ├── DigSensorLib.h
│   ├── RelayLib.h
│   └── SocketConLib.h
├── src/
│   ├── GyroLib.cpp
│   ├── KeypadLib.cpp
│   ├── DigSensorLib.cpp
│   ├── RelayLib.cpp
│   └── SocketConLib.cpp
├── ClientNode.cpp
├── ServerNode.cpp
├── GyroSensorNode.cpp
├── DigitalIONode.cpp
└── CMakeLists.txt
```
# 2. Build the Project
> mkdir build
> cd build
> cmake ..
> make


# 3. Run the Application
- On the Raspberry Pi, start these three applications in order:
> ./GyroSensorNode
> ./DigitalIONode
> ./ServerNode

- On the computer, run the client application and pass the Raspberry Pi's IP Adress:
> ./ClientNode XXX.XXX.XXX.XXXX

